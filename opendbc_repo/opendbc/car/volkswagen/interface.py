from opendbc.car.common.conversions import Conversions as CV
from opendbc.car import get_safety_config, structs
from opendbc.car.interfaces import CarInterfaceBase
from opendbc.car.volkswagen.carcontroller import CarController
from opendbc.car.volkswagen.carstate import CarState
from opendbc.car.volkswagen.values import CanBus, CAR, DashcamOnlyReason, NetworkLocation, TransmissionType, VolkswagenFlags, VolkswagenSafetyFlags, VolkswagenFlagsIQ
from opendbc.car.volkswagen.meb import interface as meb_interface
from opendbc.car.volkswagen.radar_interface import RadarInterface
import sys
import os
iqpilot_path = os.path.join(os.path.dirname(__file__), '..', '..', '..')
sys.path.insert(0, iqpilot_path)
try:
  from openpilot.common.params import Params
except ImportError:
  pass


class CarInterface(CarInterfaceBase):
  CarState = CarState
  CarController = CarController
  RadarInterface = RadarInterface

  DRIVABLE_GEARS = (structs.CarState.GearShifter.eco, structs.CarState.GearShifter.sport,
                    structs.CarState.GearShifter.manumatic, structs.CarState.GearShifter.neutral)

  @staticmethod
  def _get_params(ret: structs.CarParams, candidate: CAR, fingerprint, car_fw, alpha_long, is_release, docs) -> structs.CarParams:
    ret.brand = "volkswagen"
    ret.radarUnavailable = True
    _params = Params()
    angle_lat_enabled = _params.get_bool("AngleLateralControl")
    joystick_mode = _params.get_bool("JoystickDebugMode")

    if ret.flags & VolkswagenFlags.PQ:
      # Set global PQ35/PQ46/NMS parameters
      safety_configs = [get_safety_config(structs.CarParams.SafetyModel.volkswagenPq)]
      if angle_lat_enabled:
        ret.flags |= VolkswagenFlagsIQ.IQ_LVBS_ALC_MODULE.value
        safety_configs[0].safetyParam |= VolkswagenSafetyFlags.PQ_ALC_MODULE.value
      ret.enableBsm = 0x3BA in fingerprint[0]  # SWA_1

      if 0x440 in fingerprint[0] or docs:  # Getriebe_1
        ret.transmissionType = TransmissionType.automatic
      else:
        ret.transmissionType = TransmissionType.manual

      # Auto-detect CC only mode by checking for ACC / AWV presence
      # ACC_System = 0x368, ACC_GRA_Anzeige = 0x56A, AWV = 0x366
      has_acc = 0x368 in fingerprint[0] or 0x56A in fingerprint[0]
      if not has_acc:
        has_radar = 0x366 in fingerprint[0]  # AWV for FCW/AEB
        if has_radar:
          ret.flags |= VolkswagenFlagsIQ.IQ_CC_ONLY.value
        else:
          ret.flags |= VolkswagenFlagsIQ.IQ_CC_ONLY_NO_RADAR.value

      if any(msg in fingerprint[1] for msg in (0x1A0, 0xC2)):  # Bremse_1, Lenkwinkel_1
        ret.networkLocation = NetworkLocation.gateway
      else:
        ret.networkLocation = NetworkLocation.fwdCamera

      ret.dashcamOnly = False

    elif ret.flags & VolkswagenFlags.MLB:
      # Set global MLB parameters
      safety_configs = [get_safety_config(structs.CarParams.SafetyModel.volkswagenMlb)]
      ret.enableBsm = 0x30F in fingerprint[0]  # SWA_01
      ret.networkLocation = NetworkLocation.gateway
      ret.dashcamOnly = False

    elif ret.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      safety_configs = meb_interface.configure_meb_fingerprint(ret, fingerprint)

    else:
      # Set global MQB parameters
      safety_configs = [get_safety_config(structs.CarParams.SafetyModel.volkswagen)]
      ret.enableBsm = 0x30F in fingerprint[0]  # SWA_01

      if 0xAD in fingerprint[0] or docs:  # Getriebe_11
        ret.transmissionType = TransmissionType.automatic
      elif 0x187 in fingerprint[0]:  # Motor_EV_01
        ret.transmissionType = TransmissionType.direct
      else:
        ret.transmissionType = TransmissionType.manual

      if any(msg in fingerprint[1] for msg in (0x40, 0x86, 0xB2, 0xFD)):  # Airbag_01, LWI_01, ESP_19, ESP_21
        ret.networkLocation = NetworkLocation.gateway
      else:
        ret.networkLocation = NetworkLocation.fwdCamera

      if 0x126 in fingerprint[2]:  # HCA_01
        ret.flags |= VolkswagenFlags.STOCK_HCA_PRESENT.value
      if 0x6B8 in fingerprint[0]:  # Kombi_03
        ret.flags |= VolkswagenFlags.KOMBI_PRESENT.value

      # Auto-detect CC only mode by checking for ACC_06/ACC_07 presence
      # ACC_06 = 0x122, ACC_07 = 0x12E, ACC_10 = 0x117
      has_acc = 0x122 in fingerprint[0] or 0x12E in fingerprint[0]
      if not has_acc:
        has_radar = 0x117 in fingerprint[0]  # ACC_10 for FCW/AEB
        if has_radar:
          ret.flags |= VolkswagenFlagsIQ.IQ_CC_ONLY.value
        else:
          ret.flags |= VolkswagenFlagsIQ.IQ_CC_ONLY_NO_RADAR.value

    # Global lateral tuning defaults, can be overridden per-vehicle

    ret.steerLimitTimer = 0.4
    if ret.flags & VolkswagenFlags.PQ:
      ret.steerActuatorDelay = 0.2
      ret.longitudinalTuning.kfDEPRECATED = 1.2
      ret.longitudinalTuning.kpBP = [0.]
      ret.longitudinalTuning.kpV = [.45]
      ret.longitudinalTuning.kiBP = [0.]
      ret.longitudinalTuning.kiV = [.69]
      ret.longitudinalActuatorDelay = 0.6
      if angle_lat_enabled:
        ret.steerControlType = structs.CarParams.SteerControlType.angle
        ret.steerAtStandstill = bool(joystick_mode)
      else:
        CarInterfaceBase.configure_torque_tune(candidate, ret.lateralTuning)
    elif ret.flags & VolkswagenFlags.MLB:
      ret.steerActuatorDelay = 0.2
      CarInterfaceBase.configure_torque_tune(candidate, ret.lateralTuning)
    elif ret.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      meb_interface.configure_meb_tuning(ret)
    else:
      ret.steerActuatorDelay = 0.1
      ret.lateralTuning.pid.kpBP = [0.]
      ret.lateralTuning.pid.kiBP = [0.]
      ret.lateralTuning.pid.kf = 0.00006
      ret.lateralTuning.pid.kpV = [0.6]
      ret.lateralTuning.pid.kiV = [0.2]
      if angle_lat_enabled:
        ret.steerControlType = structs.CarParams.SteerControlType.angle
        ret.steerAtStandstill = bool(joystick_mode)

    # Global longitudinal tuning defaults, can be overridden per-vehicle
    # MEB longitudinal tuning is set in meb_interface.configure_meb_tuning above.

    ret.alphaLongitudinalAvailable = ret.networkLocation == NetworkLocation.gateway or docs or bool(ret.flags & VolkswagenFlags.DISABLE_RADAR)
    if alpha_long:
      # Proof-of-concept, prep for E2E only. No radar points available. Panda ALLOW_DEBUG firmware required.
      ret.openpilotLongitudinalControl = True
      safety_configs[0].safetyParam |= VolkswagenSafetyFlags.LONG_CONTROL.value
      safety_configs[0].safetyParam |= VolkswagenSafetyFlags.ALLOW_LONG_ACCEL_WITH_GAS_PRESSED.value
      if ret.transmissionType == TransmissionType.manual:
        ret.minEnableSpeed = 4.5

    # Per-vehicle overrides

    if candidate == CAR.PORSCHE_MACAN_MK1:
      ret.steerActuatorDelay = 0.07

    ret.pcmCruise = not ret.openpilotLongitudinalControl
    if ret.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      meb_interface.configure_meb_starting(ret)
    else:
      ret.stopAccel = -0.55
      ret.vEgoStarting = 0.1
      ret.vEgoStopping = 1.5 * CV.KPH_TO_MS if ret.flags & VolkswagenFlags.PQ else 0.1
    ret.autoResumeSng = ret.minEnableSpeed == -1
    CAN = CanBus(fingerprint=fingerprint)
    if CAN.pt >= 4:
      safety_configs.insert(0, get_safety_config(structs.CarParams.SafetyModel.noOutput))
    ret.safetyConfigs = safety_configs

    return ret

  @staticmethod
  def pre_init(CP: structs.CarParams, CP_IQ: structs.IQCarParams, can_recv, can_send):
    # Engine-on check moved to init(): if radar can't be disabled, radarDisableFailed=True
    # gates only long control (carcontroller line ~308) while lateral still works.
    # Full dashcam mode here was too aggressive — lateral doesn't need radar disabled.
    pass

  @staticmethod
  def init(CP: structs.CarParams, CP_IQ: structs.IQCarParams, can_recv, can_send):
    # Disable radar via UDS programming session so openpilot can take over longitudinal.
    # The radar stops transmitting AWV_03/Strukturen_01 and carcontroller replaces those messages.
    meb_interface.init_radar(CP, can_recv, can_send)

  @staticmethod
  def deinit(CP: structs.CarParams, can_recv, can_send):
    # Re-enable radar TX on exit (currently never called by openpilot, car recovers after ignition cycle)
    meb_interface.deinit_radar(CP, can_recv, can_send)

  @staticmethod
  def _get_params_iq(stock_cp: structs.CarParams, ret: structs.IQCarParams, candidate, fingerprint: dict[int, dict[int, int]], car_fw: list[structs.CarParams.CarFw], alpha_long: bool, is_release_iq: bool, docs: bool) -> structs.IQCarParams:
    return ret
