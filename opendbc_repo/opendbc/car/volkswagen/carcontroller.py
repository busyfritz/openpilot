"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""
import sys
import os
import math
import numpy as np
import random
from opendbc.can import CANPacker
from opendbc.car import Bus, DT_CTRL, structs
from opendbc.car.lateral import apply_driver_steer_torque_limits, apply_steer_angle_limits_simple
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.common.numpy_fast import clip, interp
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.volkswagen import mlbcan, mqbcan, pqcan
from opendbc.car.volkswagen.meb import can as mebcan
from opendbc.car.volkswagen.meb.carcontroller import MEBCarController
from opendbc.car.volkswagen.values import CanBus, CarControllerParams, VolkswagenFlags, VolkswagenFlagsIQ
from opendbc.car.vehicle_model import VehicleModel
from openpilot.iqpilot.konn3kt.iqlvbs import alc as iq_lvbs_alc

iqpilot_path = os.path.join(os.path.dirname(__file__), '..', '..', '..')
sys.path.insert(0, iqpilot_path)
try:
  from openpilot.common.params import Params
except ImportError:
  pass

VisualAlert = structs.CarControl.HUDControl.VisualAlert
LongCtrlState = structs.CarControl.Actuators.LongControlState


class MQBStandstillManager:
  BRAKE_TORQUE_RAMP_RATE = 2000.0     # Nm/s
  ASSUMED_WHEEL_RADIUS = 0.328        # m, typical tire rolling radius
  GRAVITY = 9.81                      # m/s^2
  WEGIMPULSE_STILLNESS_FRAMES = 5     # frames of no wheel tick change before assuming standstill
  ESP_OVERRIDE_SPEED = 9.5 * CV.KPH_TO_MS
  MAX_SAFE_STOPPING_SPEED = 10.0 * CV.KPH_TO_MS

  def __init__(self, vehicle_mass: float = 1540.0, accel_min: float = -3.5):
    self.vehicle_mass = vehicle_mass
    self.accel_min = accel_min
    self.can_stop_forever = False
    self.rollback_detected = False
    self.start_commit_active = False
    self.frames_since_last_wheel_pulse = 0
    self.prev_sum_wegimpulse: int | None = None
    self.prev_accel = 0
    self.hold_recovery_active = False

  def get_hill_hold_decel_deficit(self, pitch: float, brake_torque: float) -> float:
    if self.vehicle_mass <= 0:
      return 0.0
    uphill_pitch = max(pitch, 0.0)
    hill_hold_decel = self.GRAVITY * math.sin(uphill_pitch)
    brake_decel = max(brake_torque, 0.0) / (self.vehicle_mass * self.ASSUMED_WHEEL_RADIUS)
    return max(hill_hold_decel - brake_decel, 0.0)

  def get_safe_speed_for_brake_torque(self, pitch: float, brake_torque: float) -> float:
    missing_brake_decel = self.get_hill_hold_decel_deficit(pitch, brake_torque)
    if missing_brake_decel <= 0 or self.vehicle_mass <= 0:
      return 0.0
    brake_decel_build_rate = self.BRAKE_TORQUE_RAMP_RATE / (self.vehicle_mass * self.ASSUMED_WHEEL_RADIUS)
    forward_speed_needed_while_brake_builds = 1.5 * missing_brake_decel ** 2 / brake_decel_build_rate
    return min(forward_speed_needed_while_brake_builds, self.MAX_SAFE_STOPPING_SPEED)

  def get_blended_brake_accel(self, raw_accel: float, v_ego: float, pitch: float, brake_torque: float) -> float:
    zero_brake_decel_deficit = self.get_hill_hold_decel_deficit(pitch, 0.0)
    current_brake_decel_deficit = self.get_hill_hold_decel_deficit(pitch, brake_torque)
    zero_brake_safe_speed = self.get_safe_speed_for_brake_torque(pitch, 0.0)
    if zero_brake_decel_deficit <= 0 or zero_brake_safe_speed <= 0:
      return raw_accel
    brake_deficit_risk = current_brake_decel_deficit / zero_brake_decel_deficit
    speed_risk = max(zero_brake_safe_speed - v_ego, 0.0) / zero_brake_safe_speed
    rollback_risk = float(np.clip(speed_risk * brake_deficit_risk, 0.0, 1.0))
    blended_accel = raw_accel + rollback_risk * (self.accel_min - raw_accel)
    return min(raw_accel, blended_accel)

  def update(self, CS, long_active: bool, accel: float, stopping: bool, starting: bool,
             max_planned_speed: float, pitch: float = 0.0,
             tsk_brake_torque: float = 0.0) -> tuple[bool, float, bool, bool, bool | None, bool | None]:

    safe_stopping_speed = self.get_safe_speed_for_brake_torque(pitch, 0.0)
    below_safe_stop_speed = CS.out.vEgo < safe_stopping_speed
    can_accelerate = max_planned_speed > safe_stopping_speed
    uphill_grade_pct = max(math.tan(pitch) * 100.0, 0.0)
    takeoff_acceleration = max(0.2, 0.1 * uphill_grade_pct)

    if CS.out.vEgo < self.ESP_OVERRIDE_SPEED:
      esp_starting_override: bool | None = True
      esp_stopping_override: bool | None = False
    else:
      esp_starting_override = None
      esp_stopping_override = None

    if CS.rolling_backward:
      self.rollback_detected = True
    elif CS.rolling_forward:
      self.rollback_detected = False

    wheel_did_pulse = CS.sum_wegimpulse != self.prev_sum_wegimpulse
    self.prev_sum_wegimpulse = CS.sum_wegimpulse
    if wheel_did_pulse:
      self.frames_since_last_wheel_pulse = 0
    else:
      self.frames_since_last_wheel_pulse += 1
    near_standstill = self.frames_since_last_wheel_pulse >= self.WEGIMPULSE_STILLNESS_FRAMES

    # acc type 1 is sensitive to control signals when brake is pressed (when preEnabled)
    if CS.out.brakePressed:
      long_active = False

    if long_active and not CS.out.gasPressed:
      if CS.esp_hold_confirmation:
        self.start_commit_active = True
      if can_accelerate and below_safe_stop_speed and accel > 0:
        self.start_commit_active = True
      elif self.start_commit_active:
        if CS.out.vEgo > safe_stopping_speed:
          self.start_commit_active = False
    else:
      self.start_commit_active = False

    if long_active:
      raw_accel = accel
      if self.start_commit_active:
        accel = max(accel, takeoff_acceleration)
        stopping = False
        starting = True
      elif self.rollback_detected:
        accel = self.accel_min
        stopping = True
        starting = False
      elif below_safe_stop_speed:
        accel = self.get_blended_brake_accel(accel, CS.out.vEgo, pitch, tsk_brake_torque)
        if accel < raw_accel:
          stopping = True
          starting = False
      if near_standstill and accel < 0 and tsk_brake_torque == 0:
        accel = self.accel_min
        stopping = True
        starting = False
      if CS.out.standstill and accel < 0:
        accel = min(accel, self.prev_accel)

    if long_active:
      if CS.out.vEgo > self.ESP_OVERRIDE_SPEED:
        self.can_stop_forever = False
      if CS.esp_hold_confirmation:
        self.can_stop_forever = False
        self.hold_recovery_active = True

      if self.start_commit_active:
        esp_starting_override = True
        esp_stopping_override = False
      elif CS.esp_stopping:
        self.can_stop_forever = True
        self.hold_recovery_active = False
        esp_starting_override = True
        esp_stopping_override = False
      elif self.can_stop_forever:
        esp_starting_override = True
        esp_stopping_override = False
      elif near_standstill:
        esp_starting_override = False
        esp_stopping_override = True
      # recover from hold confirmations while moving to prevent reconfirming them
      elif self.hold_recovery_active and not CS.out.standstill:
        esp_starting_override = False
        esp_stopping_override = True
    else:
      self.can_stop_forever = False
      self.hold_recovery_active = False

    self.prev_accel = accel
    return long_active, accel, stopping, starting, esp_starting_override, esp_stopping_override


class CarController(MEBCarController, CarControllerBase):
  def __init__(self, dbc_names, CP, CP_IQ):
    super().__init__(dbc_names, CP, CP_IQ)
    self._params = Params()
    self.CCP = CarControllerParams(CP)
    self.CAN = CanBus(CP)
    self.packer_pt = CANPacker(dbc_names[Bus.pt])

    if CP.flags & VolkswagenFlags.PQ:
      self.CCS = pqcan
    elif CP.flags & VolkswagenFlags.MLB:
      self.CCS = mlbcan
    elif CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      self.CCS = mebcan
    else:
      self.CCS = mqbcan

    self.accel = 0
    self.apply_torque_last = 0
    self.apply_curvature_last = 0.
    self.apply_angle_last = 0
    self.ALC_entryCounter = 0
    self.ALC_driverExit = False
    self.ALC_reentry_blocked = False
    self.ALC_override_last = False
    self.ALC_override_counter = 0
    self.entering = False
    self.active = False
    self.CSLH3_SignLast = 0
    self.CSsteeringAngleDegLast = 0
    self.steering_power_last = 0
    self.gra_acc_counter_last = None
    self.acc_counter_seeded = False
    self.klr_counter_last = None
    self.eps_timer_soft_disable_alert = False
    self.hca_frame_timer_running = 0
    self.hca_frame_same_torque = 0
    self.accel_last = 0
    self.accel_diff = 0
    self.long_deviation = 0
    self.long_jerklimit = 0
    self.HCA_Status = 3
    self.leadDistanceBars = 0
    self.lead_distance_bars_last = None
    self.distance_bar_frame = 0
    self.speed_limit_last = 0
    self.speed_limit_changed_timer = 0
    self.blinkerActive = None
    self.hide_ea_error = False
    self.radar_disabled_warning_timer = 0
    # Check once at init whether the DBC includes MEB_AWV_01 (AEB HUD for radar-disabled camera harness cars)
    self._has_aeb_hud_msg = "MEB_AWV_01" in self.packer_pt.dbc.name_to_msg
    self.eps_timer_workaround = bool(CP.flags & VolkswagenFlags.MLB)
    self.hca_frame_timer_resetting = 0
    self.hca_frame_low_torque = 0
    self.long_override_counter = 0
    self.long_disabled_counter = 0
    self.standstill_manager = MQBStandstillManager(CP.mass, self.CCP.ACCEL_MIN) if self.CCS == mqbcan else None
    self.VM = VehicleModel(CP)
    self._init_meb()

  def update(self, CC, CC_IQ, CS, now_nanos):
    actuators = CC.actuators
    hud_control = CC.hudControl
    can_sends = []
    output_torque = 0
    apply_torque = 0
    pqhca5or7Toggle = self._params.get_bool("pqhca5or7Toggle")
    eBrakeActive = self._params.get_bool("eBrakeActive")
    AngleLateralControl = iq_lvbs_alc.angle_lateral_control_enabled(self, CS)
    self.entering = CS.vw_iq_lvbs_alc_entering
    self.active = CS.vw_iq_lvbs_alc_active

    CS.force_rhd_for_bsm = getattr(CC, "forceRHDForBSM", False)
    CS.enable_predicative_speed_limit = getattr(CC.cruiseControl, "speedLimitPredicative", False)
    CS.enable_pred_react_to_speed_limits = getattr(CC.cruiseControl, "speedLimitPredReactToSL", False)
    CS.enable_pred_react_to_curves = getattr(CC.cruiseControl, "speedLimitPredReactToCurves", False)

    if self.frame % self.CCP.STEER_STEP == 0:
      if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
        self._meb_steering_control(CC, CS, actuators, can_sends)
      else:
        if CC.latActive and not AngleLateralControl:
          new_torque = int(round(actuators.torque * self.CCP.STEER_MAX))
          apply_torque = apply_driver_steer_torque_limits(new_torque, self.apply_torque_last, CS.out.steeringTorque, self.CCP)
          self.hca_frame_timer_running += self.CCP.STEER_STEP
          if self.apply_torque_last == apply_torque:
            self.hca_frame_same_torque += self.CCP.STEER_STEP
            if self.hca_frame_same_torque > self.CCP.STEER_TIME_STUCK_TORQUE / DT_CTRL:
              apply_torque -= (1, -1)[apply_torque < 0]
              self.hca_frame_same_torque = 0
          else:
            self.hca_frame_same_torque = 0
          hca_enabled = abs(apply_torque) > 0
          if self.eps_timer_workaround and self.hca_frame_timer_running >= self.CCP.STEER_TIME_BM / DT_CTRL:
            if abs(apply_torque) <= self.CCP.STEER_LOW_TORQUE:
              self.hca_frame_low_torque += self.CCP.STEER_STEP
              if self.hca_frame_low_torque >= self.CCP.STEER_TIME_LOW_TORQUE / DT_CTRL:
                hca_enabled = False
            else:
              self.hca_frame_low_torque = 0
              if self.hca_frame_timer_resetting > 0:
                apply_torque = 0
        else:
          self.hca_frame_low_torque = 0
          hca_enabled = False
          apply_torque = 0

        if hca_enabled:
          output_torque = apply_torque
          self.hca_frame_timer_resetting = 0
        else:
          output_torque = 0
          self.hca_frame_timer_resetting += self.CCP.STEER_STEP
          if self.hca_frame_timer_resetting >= self.CCP.STEER_TIME_RESET / DT_CTRL or not self.eps_timer_workaround:
            self.hca_frame_timer_running = 0
            apply_torque = 0

        if hca_enabled and abs(apply_torque) > 0:
          if pqhca5or7Toggle and (self.CP.flags & (VolkswagenFlags.PQ | VolkswagenFlags.MLB)):
            self.HCA_Status = 7
          else:
            self.HCA_Status = 5
        else:
          self.HCA_Status = 3

        self.eps_timer_soft_disable_alert = self.hca_frame_timer_running > self.CCP.STEER_TIME_ALERT / DT_CTRL
        self.apply_torque_last = apply_torque
        if not (AngleLateralControl and self.CCS in (mqbcan, pqcan)):
          can_sends.append(self.CCS.create_hca_steering_control(self.packer_pt, self.CAN.pt, output_torque, self.HCA_Status))

      if self.CP.flags & VolkswagenFlags.STOCK_HCA_PRESENT:
        ea_simulated_torque = float(np.clip(apply_torque * 2, -self.CCP.STEER_MAX, self.CCP.STEER_MAX))
        if abs(CS.out.steeringTorque) > abs(ea_simulated_torque):
          ea_simulated_torque = CS.out.steeringTorque
        can_sends.append(self.CCS.create_eps_update(self.packer_pt, self.CAN.cam, CS.eps_stock_values, ea_simulated_torque))

    iq_lvbs_alc.update_vw_alc(self, CC, CS, actuators, can_sends, apply_torque)
    if self.frame % self.CCP.STEER_STEP == 0:
      iq_lvbs_alc.append_private_apd(self, CC_IQ, can_sends)

    if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      self._meb_capacitive_wheel(CC, CS, can_sends)
      self._meb_blinker_control(CC, CS, can_sends)

    if self.CP.openpilotLongitudinalControl and self.CCS == mqbcan and not self.acc_counter_seeded and CS.acc_stock_counters:
      for name in ("ACC_02", "ACC_06", "ACC_07", "ACC_10"):
        addr = self.packer_pt.dbc.name_to_msg[name].address
        self.packer_pt.counters[addr] = (CS.acc_stock_counters[name] + 1) % 16
      self.acc_counter_seeded = True

    if self.frame % self.CCP.ACC_CONTROL_STEP == 0 and self.CP.openpilotLongitudinalControl and not CS.out.radarDisableFailed:
      stopping = actuators.longControlState == LongCtrlState.stopping
      if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
        self._meb_acceleration_control(CC, CS, actuators, hud_control, stopping, can_sends)
      else:
        stopping = actuators.longControlState == LongCtrlState.stopping
        starting = actuators.longControlState == LongCtrlState.pid and (CS.esp_hold_confirmation or CS.out.vEgo < self.CP.vEgoStopping)
        long_active = CC.longActive
        accel = actuators.accel
        esp_starting_override = None
        esp_stopping_override = None

        if self.CCS == mqbcan and CS.acc_type == 1 and self.standstill_manager is not None:
          pitch = CC.orientationNED[1] if len(CC.orientationNED) == 3 else 0.0
          long_active, accel, stopping, starting, esp_starting_override, esp_stopping_override = self.standstill_manager.update(
            CS, long_active, accel, stopping, starting, float(getattr(actuators, "speed", 0.0)),
            pitch, CS.tsk_brake_torque,
          )

        acc_control = self.CCS.acc_control_value(CS.out.cruiseState.available, long_active, CC.cruiseControl.override, CS.out.accFaulted)
        accel = float(np.clip(accel, self.CCP.ACCEL_MIN, self.CCP.ACCEL_MAX) if (long_active or CC.cruiseControl.override) else 0)
        self.accel_diff = (0.0019 * (accel - self.accel_last)) + (1 - 0.0019) * self.accel_diff
        self.long_jerklimit = 3.0 # AendGrad (0.01 * (np.clip(abs(accel), 0.7, 2))) + (1 - 0.01) * self.long_jerklimit
        self.long_deviation = 0.2 # RegelAbw np.interp(abs(accel - self.accel_diff), [0, 0.3, 1.0], [0.02, 0.04, 0.08])
        self.accel_last = accel
        if self.CCS == mqbcan:
          can_sends.extend(self.CCS.create_acc_accel_control(
            self.packer_pt, self.CAN.pt, CS.acc_type, accel, acc_control, stopping, starting, CS.esp_hold_confirmation,
            self.long_deviation, self.long_jerklimit, eBrakeActive,
            esp_starting_override=esp_starting_override, esp_stopping_override=esp_stopping_override,
          ))
        else:
          can_sends.extend(self.CCS.create_acc_accel_control(
            self.packer_pt, self.CAN.pt, CS.acc_type, accel, acc_control, stopping, starting, CS.esp_hold_confirmation,
            self.long_deviation, self.long_jerklimit, eBrakeActive,
          ))

    if (self.CP.flags & VolkswagenFlags.DISABLE_RADAR) and self.CP.openpilotLongitudinalControl and not CS.out.radarDisableFailed:
      if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
        self._meb_radar_disable(can_sends)

    if self.frame % self.CCP.LDW_STEP == 0:
      hud_alert = 0
      if hud_control.visualAlert in (VisualAlert.steerRequired, VisualAlert.ldw) or CS.out.steerFaultTemporary:
        hud_alert = self.CCP.LDW_MESSAGES["laneAssistTakeOver"]
      steering_pressed_hud = (self.frame // 2) % 2 == 0 if self.entering else CS.out.steeringPressed
      if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
        self._meb_lka_hud(CC, CS, hud_control, hud_alert, can_sends)
      else:
        can_sends.append(self.CCS.create_lka_hud_control(self.packer_pt, self.CAN.pt, CS.ldw_stock_values, CC.latActive, steering_pressed_hud,
                                                         hud_alert, hud_control, self.entering, self.CCS is pqcan and AngleLateralControl, self.active))

    if hud_control.leadDistanceBars != self.lead_distance_bars_last:
      self.distance_bar_frame = self.frame

    if self.frame % self.CCP.ACC_HUD_STEP == 0 and self.CP.openpilotLongitudinalControl:
      if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
        self._meb_acc_hud(CC, CS, hud_control, can_sends)
      else:
        leadDistance = min(8, hud_control.leadDistance) if hud_control.leadDistance != 0 else 0
        fcw_alert = hud_control.visualAlert == VisualAlert.fcw
        self.leadDistanceBars = min(3, hud_control.leadDistanceBars)
        acc_hud_status = self.CCS.acc_hud_status_value(CS.out.cruiseState.available, CS.out.accFaulted, CC.longActive, CC.cruiseControl.override)
        set_speed = hud_control.setSpeed * CV.MS_TO_KPH
        can_sends.append(self.CCS.create_acc_hud_control(self.packer_pt, self.CAN.pt, acc_hud_status, set_speed,
                                                         leadDistance, self.leadDistanceBars, fcw_alert, hud_control.leadVisible))

    if self.CP.flags & VolkswagenFlags.PQ:
      if self.frame % 2 == 0:
        self.blinkerActive = CS.leftBlinkerUpdate or CS.rightBlinkerUpdate
        leftBlinker = CC.leftBlinker if not self.blinkerActive else False
        rightBlinker = CC.rightBlinker if not self.blinkerActive else False
        can_sends.append(self.CCS.create_blinker_control(self.packer_pt, self.CAN.pt, leftBlinker, rightBlinker))

    if self.CP.openpilotLongitudinalControl and (self.CP.flags & VolkswagenFlags.PQ):
      if self.frame % 2 == 0:
        can_sends.append(self.CCS.filter_motor2(self.packer_pt, self.CAN.ext, CS.motor2_stock))

    gra_send_ready = self.CP.pcmCruise and CS.gra_stock_values["COUNTER"] != self.gra_acc_counter_last
    if self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO):
      main_cruise_latching = not bool(CS.gra_stock_values["GRA_Typ_Hauptschalter"])
      stock_cancel_pressed = bool(CS.gra_stock_values["GRA_Abbrechen"] if main_cruise_latching else CS.gra_stock_values["GRA_Hauptschalter"])
    elif self.CP.flags & VolkswagenFlags.MLB:
      stock_cancel_pressed = bool(CS.gra_stock_values["LS_Abbrechen"])
    else:
      stock_cancel_pressed = bool(CS.gra_stock_values["GRA_Abbrechen"])

    cancel_cmd = stock_cancel_pressed or CC.cruiseControl.cancel
    if gra_send_ready and (cancel_cmd or CC.cruiseControl.resume):
      bus_send = self.CAN.aux if self.CP.flags & VolkswagenFlags.PQ else self.CAN.ext
      can_sends.append(self.CCS.create_acc_buttons_control(self.packer_pt, bus_send, CS.gra_stock_values,
                                                           cancel=cancel_cmd, resume=CC.cruiseControl.resume))

    if self.CP.openpilotLongitudinalControl and self.CCS == pqcan:
      if self.frame % 3:
        can_sends.append(self.CCS.create_gra_neu(self.packer_pt, self.CAN.ext, CS.gra_stock_values, CC.longActive))


    new_actuators = actuators.as_builder()
    new_actuators.torque = self.apply_torque_last / self.CCP.STEER_MAX
    new_actuators.torqueOutputCan = self.apply_torque_last
    if self.CP.steerControlType == structs.CarParams.SteerControlType.angle:
      new_actuators.steeringAngleDeg = float(self.apply_angle_last)
    new_actuators.curvature = float(self.apply_curvature_last)
    new_actuators.accel = self.accel_last
    new_actuators.speed = float(getattr(actuators, "speed", 0.0))

    self.lead_distance_bars_last = hud_control.leadDistanceBars
    self.gra_acc_counter_last = CS.gra_stock_values["COUNTER"]
    self.frame += 1
    return new_actuators, can_sends
