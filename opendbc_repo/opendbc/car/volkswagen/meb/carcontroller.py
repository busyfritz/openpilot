import numpy as np

from opendbc.car import DT_CTRL, make_tester_present_msg, structs
from opendbc.car.lateral import apply_std_curvature_limits
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.volkswagen.values import VolkswagenFlags
from opendbc.car.volkswagen.meb import can as mebcan
from opendbc.car.volkswagen.meb.utils import LongControlJerk, LongControlLimit, LatControlCurvature

VisualAlert = structs.CarControl.HUDControl.VisualAlert
LongCtrlState = structs.CarControl.Actuators.LongControlState


class MEBCarController:
  def _init_meb(self):
    is_meb = bool(self.CP.flags & (VolkswagenFlags.MEB | VolkswagenFlags.MQB_EVO))
    self.long_jerk_control = LongControlJerk(dt=(DT_CTRL * self.CCP.ACC_CONTROL_STEP)) if is_meb else None
    self.long_limit_control = LongControlLimit(dt=(DT_CTRL * self.CCP.ACC_CONTROL_STEP)) if is_meb else None
    self.LateralController = (
      LatControlCurvature(self.CCP.CURVATURE_PID, self.CCP.CURVATURE_LIMITS.CURVATURE_MAX, 1 / (DT_CTRL * self.CCP.STEER_STEP))
      if is_meb
      else None
    )

  def _meb_steering_control(self, CC, CS, actuators, can_sends):
    if CC.latActive:
      hca_enabled = True
      if CC.curvatureControllerActive:
        apply_curvature = self.LateralController.update(CS.out, CC, actuators.curvature)
        apply_curvature = apply_curvature + (CS.out.steeringCurvature - (CC.currentCurvature - CC.rollCompensation))
      else:
        apply_curvature = actuators.curvature + (CS.out.steeringCurvature - CC.currentCurvature)
      apply_curvature = apply_std_curvature_limits(apply_curvature, self.apply_curvature_last, CS.out.vEgoRaw, CS.out.steeringCurvature,
                                                   CS.out.steeringPressed, self.CCP.STEER_STEP, CC.latActive, self.CCP.CURVATURE_LIMITS)

      min_power = max(self.steering_power_last - self.CCP.STEERING_POWER_STEP, self.CCP.STEERING_POWER_MIN)
      max_power = min(self.steering_power_last + self.CCP.STEERING_POWER_STEP, self.CCP.STEERING_POWER_MAX)
      target_power_driver = int(np.interp(CS.out.steeringTorque, [self.CCP.STEER_DRIVER_ALLOWANCE, self.CCP.STEER_DRIVER_MAX],
                                                                 [self.CCP.STEERING_POWER_MAX, self.CCP.STEERING_POWER_MIN]))
      # Full steering authority down to standstill (steer-at-0): no low-speed power de-rate.
      # The MEB curvature rack supports standstill steering (steerAtStandstill=True).
      target_power = target_power_driver
      steering_power = min(max(target_power, min_power), max_power)
    else:
      if self.LateralController is not None:
        self.LateralController.reset()
      if self.steering_power_last > 0:
        hca_enabled = True
        apply_curvature = np.clip(CS.out.steeringCurvature, -self.CCP.CURVATURE_LIMITS.CURVATURE_MAX, self.CCP.CURVATURE_LIMITS.CURVATURE_MAX)
        steering_power = max(self.steering_power_last - self.CCP.STEERING_POWER_STEP, 0)
      else:
        hca_enabled = False
        apply_curvature = 0.
        steering_power = 0

    can_sends.append(self.CCS.create_steering_control(self.packer_pt, self.CAN.pt, apply_curvature, hca_enabled, steering_power))
    self.apply_curvature_last = apply_curvature
    self.steering_power_last = steering_power

  def _meb_capacitive_wheel(self, CC, CS, can_sends):
    if self.CP.flags & VolkswagenFlags.STOCK_KLR_PRESENT:
      if CS.klr_stock_values:
        klr_send_ready = CS.klr_stock_values["COUNTER"] != self.klr_counter_last
        if klr_send_ready:
          can_sends.append(mebcan.create_capacitive_wheel_touch(self.packer_pt, self.CAN.cam, CC.latActive, CS.klr_stock_values))
          can_sends.append(mebcan.create_capacitive_wheel_touch(self.packer_pt, self.CAN.pt, CC.latActive, CS.klr_stock_values))
        self.klr_counter_last = CS.klr_stock_values["COUNTER"]

  def _meb_blinker_control(self, CC, CS, can_sends):
    if self.frame % 2 == 0:
      blinker_active = CS.left_blinker_active or CS.right_blinker_active
      left_blinker = CC.leftBlinker if not blinker_active else False
      right_blinker = CC.rightBlinker if not blinker_active else False
      can_sends.append(self.CCS.create_blinker_control(self.packer_pt, self.CAN.pt, CS.ea_hud_stock_values, CS.ea_control_stock_values,
                                                       left_blinker, right_blinker, self.hide_ea_error))

  def _meb_acceleration_control(self, CC, CS, actuators, hud_control, stopping, can_sends):
    starting = actuators.longControlState == LongCtrlState.starting and CS.out.vEgo <= self.CP.vEgoStarting
    accel = float(np.clip(actuators.accel, self.CCP.ACCEL_MIN, self.CCP.ACCEL_MAX) if CC.enabled else 0)

    long_override = CC.cruiseControl.override or CS.out.gasPressed
    self.long_override_counter = min(self.long_override_counter + 1, 5) if long_override else 0
    long_override_begin = long_override and self.long_override_counter < 5

    self.long_disabled_counter = min(self.long_disabled_counter + 1, 5) if not CC.enabled else 0
    long_disabling = not CC.enabled and self.long_disabled_counter < 5

    critical_state = hud_control.visualAlert == VisualAlert.fcw
    if CC.longComfortMode and self.long_jerk_control is not None and self.long_limit_control is not None:
      self.long_jerk_control.update(CC.enabled, long_override, hud_control.leadDistance, hud_control.leadVisible, accel, critical_state)
      self.long_limit_control.update(CC.enabled, CS.out.vEgoRaw, hud_control.setSpeed, hud_control.leadDistance, hud_control.leadVisible, critical_state)

    acc_control = self.CCS.acc_control_value(CS.out.cruiseState.available, CS.out.accFaulted, CC.enabled, long_override)
    acc_hold_type = self.CCS.acc_hold_type(CS.out.cruiseState.available, CS.out.accFaulted, CC.enabled, starting, stopping,
                                           CS.esp_hold_confirmation, long_override, long_override_begin, long_disabling)
    can_sends.extend(self.CCS.create_acc_accel_control(
      self.packer_pt, self.CAN.pt, self.CP, CS.acc_type, CC.enabled,
      self.long_jerk_control.get_jerk_up() if CC.longComfortMode and self.long_jerk_control is not None else 4.0,
      self.long_jerk_control.get_jerk_down() if CC.longComfortMode and self.long_jerk_control is not None else 4.0,
      self.long_limit_control.get_upper_limit() if CC.longComfortMode and self.long_limit_control is not None else 0.,
      self.long_limit_control.get_lower_limit() if CC.longComfortMode and self.long_limit_control is not None else 0.,
      accel, acc_control, acc_hold_type, stopping, starting, CS.esp_hold_confirmation,
      CS.out.vEgoRaw * CV.MS_TO_KPH, long_override, CS.travel_assist_available,
    ))
    self.accel_last = accel

  def _meb_radar_disable(self, can_sends):
    if self.radar_disabled_warning_timer < 600:
      self.radar_disabled_warning_timer += 1
    else:
      self.hide_ea_error = True

    if self.frame % self.CCP.AEB_CONTROL_STEP == 0:
      can_sends.append(make_tester_present_msg(0x700, self.CAN.pt, suppress_response=True))
      can_sends.append(self.CCS.create_aeb_control(self.packer_pt, self.CAN.pt, self.CP))

    if self.frame % self.CCP.AEB_HUD_STEP == 0 and self._has_aeb_hud_msg:
      can_sends.append(self.CCS.create_aeb_hud(self.packer_pt, self.CAN.pt, self.radar_disabled_warning_timer < 600))

    if self.frame % 4 == 0:
      can_sends.append(self.CCS.create_radar_objects(self.packer_pt, self.CAN.pt))

  def _meb_lka_hud(self, CC, CS, hud_control, hud_alert, can_sends):
    disable_alerts = getattr(CC, "disableCarSteerAlerts", False)
    sound_alert = self.CCP.LDW_SOUNDS["Chime"] if hud_alert != 0 and not disable_alerts else self.CCP.LDW_SOUNDS["None"]
    can_sends.append(self.CCS.create_lka_hud_control(self.packer_pt, self.CAN.pt, CS.ldw_stock_values, CC.latActive, CS.out.steeringPressed,
                                                     hud_alert, hud_control, sound_alert))

  def _meb_acc_hud(self, CC, CS, hud_control, can_sends):
    fcw_alert = hud_control.visualAlert == VisualAlert.fcw
    show_distance_bars = self.frame - self.distance_bar_frame < 400
    gap = max(8, CS.out.vEgo * hud_control.leadFollowTime)
    distance = max(8, hud_control.leadDistance) if hud_control.leadDistance != 0 else 0
    acc_hud_status = self.CCS.acc_hud_status_value(CS.out.cruiseState.available, CS.out.accFaulted, CC.enabled,
                                                   CC.cruiseControl.override or CS.out.gasPressed)

    sl_predicative_active = CC.cruiseControl.speedLimitPredicative and CS.out.cruiseState.speedLimitPredicative != 0
    if CC.cruiseControl.speedLimit and CS.out.cruiseState.speedLimit != 0 and self.speed_limit_last != CS.out.cruiseState.speedLimit:
      self.speed_limit_changed_timer = self.frame
    self.speed_limit_last = CS.out.cruiseState.speedLimit
    sl_active = self.frame - self.speed_limit_changed_timer < 400
    speed_limit = CS.out.cruiseState.speedLimitPredicative if sl_predicative_active else (CS.out.cruiseState.speedLimit if sl_active else 0)

    acc_hud_event = self.CCS.acc_hud_event(acc_hud_status, CS.esp_hold_confirmation, sl_predicative_active,
                                           CS.speed_limit_predicative_type, sl_active)

    can_sends.append(self.CCS.create_acc_hud_control(self.packer_pt, self.CAN.pt, acc_hud_status, hud_control.setSpeed * CV.MS_TO_KPH,
                                                     hud_control.leadVisible, hud_control.leadDistanceBars + 1, show_distance_bars,
                                                     CS.esp_hold_confirmation, distance, gap, fcw_alert, acc_hud_event, speed_limit))
