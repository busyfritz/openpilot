import time

from opendbc.car import get_safety_config, structs, uds
from opendbc.car.carlog import carlog
from opendbc.car.isotp_parallel_query import IsoTpParallelQuery
from opendbc.car.volkswagen.values import CanBus, NetworkLocation, RADAR_DISABLE_STATE, TransmissionType, \
                                          VolkswagenFlags, VolkswagenSafetyFlags


def configure_meb_fingerprint(ret, fingerprint):
  if ret.flags & VolkswagenFlags.MEB:
    safety_configs = [get_safety_config(structs.CarParams.SafetyModel.volkswagenMeb)]
  else:
    safety_configs = [get_safety_config(structs.CarParams.SafetyModel.volkswagenMqbEvo)]

  if ret.flags & VolkswagenFlags.MEB_GEN2:
    safety_configs[0].safetyParam |= VolkswagenSafetyFlags.ALT_CRC_VARIANT_1.value
  if ret.flags & VolkswagenFlags.MQB_EVO:
    safety_configs[0].safetyParam |= VolkswagenSafetyFlags.NO_GAS_OFFSET.value

  ret.enableBsm = 0x24C in fingerprint[0]  # MEB_Side_Assist_01
  ret.transmissionType = TransmissionType.direct
  ret.steerControlType = structs.CarParams.SteerControlType.curvatureDEPRECATED
  ret.steerAtStandstill = True

  if any(msg in fingerprint[1] for msg in (0x520, 0x86, 0xFD, 0x13D)):  # Airbag_02, LWI_01, ESP_21, QFK_01
    ret.networkLocation = NetworkLocation.gateway
  else:
    ret.networkLocation = NetworkLocation.fwdCamera

  if ret.networkLocation == NetworkLocation.gateway:
    ret.radarUnavailable = False

  if ret.networkLocation == NetworkLocation.fwdCamera:
    ret.flags |= VolkswagenFlags.DISABLE_RADAR.value
    safety_configs[0].safetyParam |= VolkswagenSafetyFlags.DISABLE_RADAR.value

  if 0x30B in fingerprint[0]:  # Kombi_01
    ret.flags |= VolkswagenFlags.KOMBI_PRESENT.value
  if 0x25D in fingerprint[0]:  # KLR_01
    ret.flags |= VolkswagenFlags.STOCK_KLR_PRESENT.value
  if all(msg in fingerprint[1] for msg in (0x462, 0x463, 0x464)):  # PSD_04, PSD_05, PSD_06
    ret.flags |= VolkswagenFlags.STOCK_PSD_PRESENT.value
  if 0x464 in fingerprint[0]:  # PSD_06
    ret.flags |= VolkswagenFlags.STOCK_PSD_06_PRESENT.value
  if 0x6B2 in fingerprint[0]:  # Diagnose_01
    ret.flags |= VolkswagenFlags.STOCK_DIAGNOSE_01_PRESENT.value
  if 0x3DC in fingerprint[0]:  # Gateway_73
    ret.flags |= VolkswagenFlags.ALT_GEAR.value

  return safety_configs


def configure_meb_tuning(ret):
  ret.steerActuatorDelay = 0.3
  ret.minSteerSpeed = 0.  # steer down to standstill (steerAtStandstill=True); MEB curvature rack supports it
  if ret.flags & VolkswagenFlags.MEB:
    ret.longitudinalActuatorDelay = 0.5
    ret.radarDelay = 0.8
    ret.longitudinalTuning.kiBP = [0., 30.]
    ret.longitudinalTuning.kiV = [0.4, 0.]


def configure_meb_starting(ret):
  ret.startingState = True
  ret.startAccel = 0.8
  ret.vEgoStarting = 0.5
  ret.vEgoStopping = 0.1
  ret.stopAccel = -0.55


def init_radar(CP, can_recv, can_send):
  if CP.openpilotLongitudinalControl and (CP.flags & VolkswagenFlags.DISABLE_RADAR):
    RADAR_DISABLE_STATE["error"] = False
    if CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      if is_engine_state_allowed_meb(can_recv):
        carlog.warning("VW MEB/MQBevo: disabling radar for longitudinal control")
        if not radar_communication_control(CP, can_recv, can_send):
          RADAR_DISABLE_STATE["error"] = True
      else:
        RADAR_DISABLE_STATE["error"] = True
        carlog.warning("VW MEB/MQBevo: radar cannot be disabled — engine is on")


def deinit_radar(CP, can_recv, can_send):
  if CP.openpilotLongitudinalControl and (CP.flags & VolkswagenFlags.DISABLE_RADAR):
    if CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      radar_communication_control(CP, can_recv, can_send, disable=False)


def radar_communication_control(CP, can_recv, can_send, disable=True) -> bool:
  # Send UDS commands to put the radar (addr 0x757) into programming session,
  # which silences its CAN TX so openpilot can send replacement messages.
  bus = CanBus(CP).pt
  addr_radar = 0x757
  addr_diag = 0x700       # Functional address for TesterPresent broadcast
  vw_rx_offset = 0x6A

  tp_req       = bytes([uds.SERVICE_TYPE.TESTER_PRESENT, 0x00])
  tp_resp      = bytes([uds.SERVICE_TYPE.TESTER_PRESENT + 0x40, 0x00])
  ext_diag_req  = bytes([uds.SERVICE_TYPE.DIAGNOSTIC_SESSION_CONTROL, uds.SESSION_TYPE.EXTENDED_DIAGNOSTIC])
  ext_diag_resp = bytes([uds.SERVICE_TYPE.DIAGNOSTIC_SESSION_CONTROL + 0x40, uds.SESSION_TYPE.EXTENDED_DIAGNOSTIC])
  flash_req    = bytes([uds.SERVICE_TYPE.DIAGNOSTIC_SESSION_CONTROL, uds.SESSION_TYPE.PROGRAMMING])
  empty_resp   = b''

  txt = "disable" if disable else "enable"

  for attempt in range(3):
    try:
      if disable:
        # Step 1: TesterPresent — wake up the radar
        query = IsoTpParallelQuery(can_send, can_recv, bus, [(addr_radar, None)],
                                   [tp_req], [tp_resp], vw_rx_offset, functional_addrs=[addr_diag])
        if not query.get_data(0.5):
          carlog.warning(f"VW radar {txt}: TesterPresent no response on attempt {attempt + 1}")
          continue

        # Step 2: Extended diagnostic session
        query = IsoTpParallelQuery(can_send, can_recv, bus, [(addr_radar, None)],
                                   [ext_diag_req], [ext_diag_resp], vw_rx_offset)
        if not query.get_data(0.5):
          carlog.warning(f"VW radar {txt}: ExtendedDiagSession no response on attempt {attempt + 1}")
          continue

        # Step 3: Programming session — radar stops transmitting
        query = IsoTpParallelQuery(can_send, can_recv, bus, [(addr_radar, None)],
                                   [flash_req], [empty_resp], vw_rx_offset)
        query.get_data(0)   # fire-and-forget, no wait needed
        carlog.warning(f"VW radar {txt}: programming session sent on attempt {attempt + 1}")

      return True

    except Exception as e:
      carlog.error(f"VW radar {txt}: exception on attempt {attempt + 1}: {repr(e)}")
      continue

  carlog.error(f"VW radar {txt}: all attempts failed")
  return False


def is_engine_state_allowed_meb(can_recv, timeout: float = 0.5) -> bool:
  # Read Motor_54 (0x14C) to check Engine_On bit before attempting radar disable.
  # Programming session is rejected by radar when engine is running.
  end_time = time.monotonic() + timeout
  while time.monotonic() < end_time:
    packets = can_recv(wait_for_one=True) or []
    for packet in packets:
      for msg in packet:
        if msg.address != 0x14C:
          continue
        engine_on = bool((msg.dat[9] >> 5) & 0x01)
        if engine_on:
          carlog.warning("VW radar disable: engine is on, skipping")
          return False
        else:
          carlog.warning("VW radar disable: engine is off, proceeding")
          return True
  carlog.warning("VW radar disable: Motor_54 not seen within timeout, assuming allowed")
  return True
