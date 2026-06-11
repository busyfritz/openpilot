import math
import time

from opendbc.can import CANParser
from opendbc.car import Bus, structs
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.volkswagen.values import DBC, CanBus, NetworkLocation, RADAR_DISABLE_STATE, GearShifter, VolkswagenFlags

ButtonType = structs.CarState.ButtonEvent.Type


class MEBCarState:
  def update_meb(self, pt_cp, main_cp, cam_cp, ext_cp) -> tuple[structs.CarState, structs.IQCarState]:
    ret = structs.CarState()
    ret_iq = structs.IQCarState()

    if time.monotonic() - self._param_update_time > 2.0:
      self.enable_speed_limit_predicative = self._params.get_bool("EnableSpeedLimitPredicative")
      self.enable_pred_react_to_speed_limits = self._params.get_bool("EnableSLPredReactToSL")
      self.enable_pred_react_to_curves = self._params.get_bool("EnableSLPredReactToCurves")
      self._param_update_time = time.monotonic()

    self.parse_wheel_speeds(ret,
      pt_cp.vl["ESC_51"]["VL_Radgeschw"],
      pt_cp.vl["ESC_51"]["VR_Radgeschw"],
      pt_cp.vl["ESC_51"]["HL_Radgeschw"],
      pt_cp.vl["ESC_51"]["HR_Radgeschw"],
    )

    if self.CP.flags & VolkswagenFlags.KOMBI_PRESENT:
      ret.vEgoCluster = pt_cp.vl["Kombi_01"]["KBI_angez_Geschw"] * CV.KPH_TO_MS
    ret.standstill = ret.vEgoRaw == 0

    ret.steeringAngleDeg = pt_cp.vl["LWI_01"]["LWI_Lenkradwinkel"] * (1, -1)[int(pt_cp.vl["LWI_01"]["LWI_VZ_Lenkradwinkel"])]
    ret.steeringRateDeg = pt_cp.vl["LWI_01"]["LWI_Lenkradw_Geschw"] * (1, -1)[int(pt_cp.vl["LWI_01"]["LWI_VZ_Lenkradw_Geschw"])]
    ret.steeringTorque = pt_cp.vl["LH_EPS_03"]["EPS_Lenkmoment"] * (1, -1)[int(pt_cp.vl["LH_EPS_03"]["EPS_VZ_Lenkmoment"])]
    ret.steeringPressed = abs(ret.steeringTorque) > self.CCP.STEER_DRIVER_ALLOWANCE

    self.curvature = -pt_cp.vl["QFK_01"]["Curvature"] * (1, -1)[int(pt_cp.vl["QFK_01"]["Curvature_VZ"])]
    ret.steeringCurvature = self.curvature
    ret.yawRate = -pt_cp.vl["ESC_50"]["Yaw_Rate"] * (1, -1)[int(pt_cp.vl["ESC_50"]["Yaw_Rate_Sign"])] * CV.DEG_TO_RAD

    if self.CP.flags & VolkswagenFlags.ALT_GEAR:
      gear_raw = pt_cp.vl["Gateway_73"]["GE_Fahrstufe"]
    else:
      gear_raw = pt_cp.vl["Getriebe_11"]["GE_Fahrstufe"]
    ret.gearShifter = self.parse_gear_shifter(self.CCP.shifter_values.get(gear_raw, None))
    drive_mode = ret.gearShifter == GearShifter.drive

    hca_status = self.CCP.hca_status_values.get(pt_cp.vl["QFK_01"]["LatCon_HCA_Status"])
    ret.steerFaultTemporary, ret.steerFaultPermanent = self.update_hca_state(hca_status, drive_mode=drive_mode)

    self.eps_stock_values = pt_cp.vl["LH_EPS_03"]
    self.klr_stock_values = pt_cp.vl["KLR_01"] if self.CP.flags & VolkswagenFlags.STOCK_KLR_PRESENT else {}
    self.ea_hud_stock_values = cam_cp.vl["EA_02"]
    self.ea_control_stock_values = cam_cp.vl["EA_01"]
    ret.carFaultedNonCritical = cam_cp.vl["EA_01"]["EA_Funktionsstatus"] in (3, 4, 5, 6)

    ret.gasPressed = pt_cp.vl["Motor_51"]["Accel_Pedal_Pressure"] > 0  # Motor_54 has unreliable offset on MQBevo
    ret.brakePressed = bool(pt_cp.vl["Motor_14"]["MO_Fahrer_bremst"])
    ret.brake = pt_cp.vl["ESC_51"]["Brake_Pressure"]

    ret.parkingBrake = pt_cp.vl["ESC_50"]["EPB_Status"] in (1, 4)

    doors = pt_cp.vl["ZV_02"] if bool(pt_cp.vl["Gateway_72"]["ZV_02_alt"]) else pt_cp.vl["Gateway_72"]
    ret.doorOpen = any([doors["ZV_FT_offen"],
                        doors["ZV_BT_offen"],
                        doors["ZV_HFS_offen"],
                        doors["ZV_HBFS_offen"],
                        doors["ZV_HD_offen"]])

    ret.seatbeltUnlatched = pt_cp.vl["Airbag_02"]["AB_Gurtschloss_FA"] != 3

    if self.CP.enableBsm:
      bsm_bus = pt_cp if self.CP.flags & (VolkswagenFlags.MEB_GEN2 | VolkswagenFlags.MQB_EVO) else ext_cp
      blindspot_driver = bool(bsm_bus.vl["MEB_Side_Assist_01"]["Blind_Spot_Info_Driver"]) or bool(bsm_bus.vl["MEB_Side_Assist_01"]["Blind_Spot_Warn_Driver"])
      blindspot_passenger = bool(bsm_bus.vl["MEB_Side_Assist_01"]["Blind_Spot_Info_Passenger"]) or bool(bsm_bus.vl["MEB_Side_Assist_01"]["Blind_Spot_Warn_Passenger"])
      force_rhd = self.force_rhd_for_bsm or self._params.get_bool("ForceRHDForBSM")
      car_is_lhd = not force_rhd
      ret.leftBlindspot = blindspot_driver if car_is_lhd else blindspot_passenger
      ret.rightBlindspot = blindspot_passenger if car_is_lhd else blindspot_driver

    self.ldw_stock_values = cam_cp.vl["LDW_02"]

    awv_values = ext_cp.vl.get("AWV_03", ext_cp.vl.get("ACC_10", {}))
    ret.stockFcw = bool(awv_values.get("FCW_Active", 0)) or bool(awv_values.get("AWV2_Freigabe", 0))
    ret.stockAeb = False

    self.acc_type = ext_cp.vl["ACC_18"]["ACC_Typ"]
    self.travel_assist_available = bool(pt_cp.vl.get("TA_01", {}).get("Travel_Assist_Available", 0))

    ret.cruiseState.available = pt_cp.vl["Motor_51"]["TSK_Status"] in (2, 3, 4, 5)
    ret.cruiseState.enabled = pt_cp.vl["Motor_51"]["TSK_Status"] in (3, 4, 5)
    acc_values = ext_cp.vl.get("MEB_ACC_01", ext_cp.vl.get("ACC_19", {}))
    ret.cruiseState.nonAdaptive = bool(acc_values.get("ACC_Limiter_Mode", 0)) if self.CP.pcmCruise else bool(pt_cp.vl["Motor_51"]["TSK_Limiter_ausgewaehlt"])

    acc_faulted = pt_cp.vl["Motor_51"]["TSK_Status"] in (6, 7)
    ret.accFaulted = self.update_acc_fault(acc_faulted, parking_brake=ret.parkingBrake, drive_mode=drive_mode)

    if self.CP.flags & VolkswagenFlags.MQB_EVO:
      self.esp_hold_confirmation = bool(pt_cp.vl["ESP_21"]["ESP_Haltebestaetigung"])
    else:
      self.esp_hold_confirmation = pt_cp.vl["ESC_50"]["Motion_State"] == 3
    ret.cruiseState.standstill = self.CP.pcmCruise and self.esp_hold_confirmation

    if self.CP.pcmCruise:
      ret.cruiseState.speed = float(int(round(acc_values.get("ACC_Wunschgeschw_02", 0)))) * CV.KPH_TO_MS
      if ret.cruiseState.speed > 90:
        ret.cruiseState.speed = 0

    if ret.cruiseState.speed > 0 and ret.vEgo > 1.0 and ret.vEgoCluster > 0:
      cluster_ratio = ret.vEgoCluster / ret.vEgo
      ret.cruiseState.speedCluster = ret.cruiseState.speed * cluster_ratio

    raining = pt_cp.vl["RLS_01"]["RS_Regenmenge"] > 0
    vze_01_values = cam_cp.vl.get("MEB_VZE_01", cam_cp.vl.get("VZE_04", {}))
    psd_04_values = main_cp.vl["PSD_04"] if self.CP.flags & VolkswagenFlags.STOCK_PSD_PRESENT else {}
    psd_05_values = main_cp.vl["PSD_05"] if self.CP.flags & VolkswagenFlags.STOCK_PSD_PRESENT else {}
    psd_06_values = main_cp.vl["PSD_06"] if self.CP.flags & VolkswagenFlags.STOCK_PSD_PRESENT else {}
    psd_06_values = pt_cp.vl["PSD_06"] if not psd_06_values and self.CP.flags & VolkswagenFlags.STOCK_PSD_06_PRESENT else psd_06_values
    diagnose_01_values = pt_cp.vl["Diagnose_01"] if self.CP.flags & VolkswagenFlags.STOCK_DIAGNOSE_01_PRESENT else {}

    if self.enable_speed_limit_predicative and not self.enable_predicative_speed_limit:
      self.enable_predicative_speed_limit = True
    self.speed_limit_mgr.enable_predicative_speed_limit(self.enable_predicative_speed_limit, self.enable_pred_react_to_speed_limits, self.enable_pred_react_to_curves)
    self.speed_limit_mgr.update(ret.vEgo, psd_04_values, psd_05_values, psd_06_values, vze_01_values, raining, diagnose_01_values)
    ret.cruiseState.speedLimit = self.speed_limit_mgr.get_speed_limit()
    ret.cruiseState.speedLimitPredicative = self.speed_limit_mgr.get_speed_limit_predicative()
    self.speed_limit_predicative_type = self.speed_limit_mgr.get_speed_limit_predicative_type()
    ret_iq.speedLimit = ret.cruiseState.speedLimit

    self.left_blinker_active = bool(pt_cp.vl["Blinkmodi_02"]["BM_links"])
    self.right_blinker_active = bool(pt_cp.vl["Blinkmodi_02"]["BM_rechts"])
    stalk_values = pt_cp.vl.get("SMLS_01", {})
    ret.leftBlinker, ret.rightBlinker = self.update_blinker_from_stalk(240, stalk_values.get("BH_Blinker_li", 0), stalk_values.get("BH_Blinker_re", 0))

    main_cruise_latching = not bool(pt_cp.vl["GRA_ACC_01"]["GRA_Typ_Hauptschalter"])
    buttons = getattr(self.CCP, "BUTTONS_ALT", self.CCP.BUTTONS) if main_cruise_latching else self.CCP.BUTTONS
    ret.buttonEvents = self.create_button_events(pt_cp, buttons)

    self.gra_stock_values = pt_cp.vl["GRA_ACC_01"]

    ret.espDisabled = bool(pt_cp.vl["ESP_21"]["ESP_Tastung_passiv"])
    ret.espActive = bool(pt_cp.vl["ESP_21"]["ESP_Eingriff"])

    allow_lat_only = self._params.get_bool("AllowLateralWhenLongUnavailable")
    cruise_main_switch = bool(pt_cp.vl["GRA_ACC_01"]["GRA_Hauptschalter"])
    cruise_fault_candidate = allow_lat_only and ret.accFaulted and cruise_main_switch

    if cruise_fault_candidate:
      self.cruise_faulted_frames += 1
      self.cruise_fault_clear_frames = 0
      if self.cruise_faulted_frames >= self.CRUISE_FAULT_LATERAL_ENABLE_FRAMES:
        self.cruise_fault_lateral_active = True
    else:
      self.cruise_faulted_frames = 0
      if self.cruise_fault_lateral_active:
        self.cruise_fault_clear_frames += 1
        if self.cruise_fault_clear_frames >= self.CRUISE_FAULT_LATERAL_DISABLE_FRAMES:
          self.cruise_fault_lateral_active = False
      else:
        self.cruise_fault_clear_frames = 0

    if not allow_lat_only:
      self.cruise_fault_lateral_active = False
      self.cruise_faulted_frames = 0
      self.cruise_fault_clear_frames = 0

    ret.cruiseFaultLateralMode = self.cruise_fault_lateral_active
    ret.lateralAvailable = ret.cruiseState.available or ret.cruiseFaultLateralMode
    ret.blockPcmEnable = ret.cruiseFaultLateralMode

    if self.CP.flags & VolkswagenFlags.MEB:
      ret.batteryDetails.charge = pt_cp.vl["Motor_16"]["MO_Energieinhalt_BMS"]
      if self.CP.networkLocation == NetworkLocation.gateway:
        ret.batteryDetails.heaterActive = bool(main_cp.vl["MEB_HVEM_03"]["PTC_ON"])
        ret.batteryDetails.voltage = main_cp.vl["MEB_HVEM_01"]["Battery_Voltage"]
        ret.batteryDetails.capacity = main_cp.vl["BMS_04"]["BMS_Kapazitaet_02"] * ret.batteryDetails.voltage
        ret.batteryDetails.soc = ret.batteryDetails.charge / ret.batteryDetails.capacity * 100.0 if ret.batteryDetails.capacity > 0 else 0.0
        ret.batteryDetails.power = main_cp.vl["MEB_HVEM_01"]["Engine_Power"]
        ret.batteryDetails.temperature = main_cp.vl["DCDC_03"]["DC_Temperatur"]
        ret.batteryDetails.chargingMode = int(main_cp.vl["BMS_04"]["BMS_IstModus"])
        ret.fuelGauge = ret.batteryDetails.soc / 100.0

    self.update_meb_virtual_lkas(ret, pt_cp, hca_status)

    ret.lowSpeedAlert = self.update_low_speed_alert(ret.vEgo)

    # Propagate radar disable failure state so carcontroller can skip ACC sends
    if self.CP.flags & VolkswagenFlags.DISABLE_RADAR:
      ret.radarDisableFailed = RADAR_DISABLE_STATE["error"]

    self.cruise_faulted = ret.accFaulted
    self.frame += 1
    self._apply_iq_private_flags(ret_iq)
    return ret, ret_iq

  def update_meb_virtual_lkas(self, ret, pt_cp, hca_status):
    temp_cruise_fault = pt_cp.vl["Motor_51"]["TSK_Status"] == self.MEB_TEMP_CRUISE_FAULT
    drive_mode = ret.gearShifter == GearShifter.drive
    if temp_cruise_fault and ret.parkingBrake and not drive_mode:
      ret.cruiseState.available = True
      self.tolerance_counter = 0
    elif self.tolerance_counter < self.MEB_TOLERANCE_MAX:
      ret.cruiseState.available = True
      self.tolerance_counter = min(self.tolerance_counter + 1, self.MEB_TOLERANCE_MAX)

    self.prev_lkas_button = self.lkas_button
    user_disable = any(b.type == ButtonType.cancel and b.pressed for b in ret.buttonEvents)
    steering_enabled = hca_status == "ACTIVE"
    cruise_standby = not ret.cruiseState.enabled
    self.lkas_button = steering_enabled and user_disable and cruise_standby

    if self.prev_lkas_button != self.lkas_button:
      event = structs.CarState.ButtonEvent()
      event.type = ButtonType.lkas
      event.pressed = self.lkas_button
      ret.buttonEvents = list(ret.buttonEvents) + [event]

  @staticmethod
  def get_can_parsers_meb(CP):
    pt_messages = [
      ("Blinkmodi_02", 1),
      ("SMLS_01", 1),
      # TA_01 lives on bus 0 (car ECU / OP-generated when long is active).
      # math.nan → ignore_alive=True so it never contributes to can_valid.
      ("TA_01", math.nan),
    ]
    if CP.networkLocation == NetworkLocation.fwdCamera:
      pt_messages.append(("AWV_03", 1))

    cam_messages = []
    if CP.networkLocation == NetworkLocation.gateway:
      cam_messages.append(("AWV_03", 1))

    return {
      Bus.pt: CANParser(DBC[CP.carFingerprint][Bus.pt], pt_messages, CanBus(CP).pt),
      Bus.main: CANParser(DBC[CP.carFingerprint][Bus.pt], [], CanBus(CP).main),
      Bus.cam: CANParser(DBC[CP.carFingerprint][Bus.pt], cam_messages, CanBus(CP).cam),
    }
