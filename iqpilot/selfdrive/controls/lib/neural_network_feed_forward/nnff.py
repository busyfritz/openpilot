"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
from collections import deque
import math
import numpy as np
import cereal.messaging as messaging
from cereal import custom

from opendbc.car.lateral import FRICTION_THRESHOLD, get_friction
from opendbc.iqpilot.car.interfaces import LatControlInputs
from opendbc.iqpilot.car.lateral_ext import get_friction as get_friction_in_torque_space
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.params import Params
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.iqpilot.selfdrive.controls.lib.helpers.lateral_brain import PilotLateralBrain, sign
from openpilot.iqpilot.selfdrive.controls.lib.neural_network_feed_forward.locator import MOCK_MODEL_PATH
from openpilot.iqpilot.selfdrive.controls.lib.neural_network_feed_forward.network import NNTorqueModel

LOW_SPEED_X = [0, 10, 20, 30]
LOW_SPEED_Y = [12, 3, 1, 0]
TURN_NUDGE_TORQUE = 0.8
EXIT_NUDGE_TORQUE = 0.6
TURN_PULSE_FRAMES = 50
EXIT_PULSE_FRAMES = 75
IQP_NAV_TORQUE_INFLUENCE_ENABLED = False

def roll_pitch_adjust(roll, pitch):
  return roll * math.cos(pitch)


class NavTorquePulseBrain:
  def __init__(self, lac_torque):
    self._controller = lac_torque
    self._nav_sm = messaging.SubMaster(["iqNavState"], poll="iqNavState")
    self._nav_key = ""
    self._nav_pulse_sign = 0.0
    self._nav_pulse_frames = 0

  def _lookup_nav_pulse(self):
    if not IQP_NAV_TORQUE_INFLUENCE_ENABLED:
      return "", 0.0, 0

    self._nav_sm.update(0)
    nav_state = self._nav_sm["iqNavState"]
    phase = getattr(nav_state, "maneuverPhase", custom.IQNavState.ManeuverPhase.none)
    maneuver_direction = getattr(nav_state, "maneuverDirection", custom.NavDirection.none)

    if phase == custom.IQNavState.ManeuverPhase.turnActive:
      direction = getattr(nav_state, "turnDesireDirection", 0)
      return f"turn-phase:{direction}", -TURN_NUDGE_TORQUE if direction == 1 else TURN_NUDGE_TORQUE, TURN_PULSE_FRAMES

    if phase == custom.IQNavState.ManeuverPhase.highwayCommit and maneuver_direction in (custom.NavDirection.left, custom.NavDirection.right):
      direction = getattr(nav_state, "lanePositioningDirection", 0)
      return f"highway-phase:{direction}", -EXIT_NUDGE_TORQUE if direction == 1 else EXIT_NUDGE_TORQUE, EXIT_PULSE_FRAMES

    if getattr(nav_state, "shouldSendTurnDesire", False):
      direction = getattr(nav_state, "turnDesireDirection", 0)
      return f"turn:{direction}", -TURN_NUDGE_TORQUE if direction == 1 else TURN_NUDGE_TORQUE, TURN_PULSE_FRAMES

    if getattr(nav_state, "shouldSendLanePositioning", False):
      direction = getattr(nav_state, "lanePositioningDirection", 0)
      return f"keep:{direction}", -EXIT_NUDGE_TORQUE if direction == 1 else EXIT_NUDGE_TORQUE, EXIT_PULSE_FRAMES

    return "", 0.0, 0

  def nudge_output_torque(self, active: bool, car_state, output_torque: float) -> float:
    if not IQP_NAV_TORQUE_INFLUENCE_ENABLED:
      self._nav_pulse_frames = 0
      self._nav_key = ""
      return output_torque

    nav_key, pulse_sign, pulse_frames = self._lookup_nav_pulse()

    if not active or getattr(car_state, "steeringPressed", False):
      self._nav_pulse_frames = 0
      if not nav_key:
        self._nav_key = ""
      return output_torque

    if nav_key and nav_key != self._nav_key:
      self._nav_key = nav_key
      self._nav_pulse_sign = pulse_sign
      self._nav_pulse_frames = pulse_frames
    elif not nav_key and self._nav_pulse_frames == 0:
      self._nav_key = ""

    if self._nav_pulse_frames > 0:
      self._nav_pulse_frames -= 1
      steer_max = float(getattr(self._controller, "steer_max", 1.0))
      output_torque = float(np.clip(output_torque + self._nav_pulse_sign, -steer_max, steer_max))

    return output_torque


class NeuralNetworkFeedForward(PilotLateralBrain):
  def __init__(self, lac_torque, CP, CP_IQ, CI):
    super().__init__(lac_torque, CP, CP_IQ, CI)
    self.params = Params()
    self.enabled = self.params.get_bool("NeuralNetworkFeedForward")
    self.has_nn_model = CP_IQ.neuralNetworkFeedForward.model.path != MOCK_MODEL_PATH
    self.model = NNTorqueModel(CP_IQ.neuralNetworkFeedForward.model.path)
    self.pitch = FirstOrderFilter(0.0, 0.5, 0.01)
    self.pitch_last = 0.0
    self.future_times = [0.3, 0.6, 1.0, 1.5]
    self.nn_future_times = [i + self.desired_lat_jerk_time for i in self.future_times]
    self.past_times = [-0.3, -0.2, -0.1]
    history_check_frames = [int(abs(i)*100) for i in self.past_times]
    self.history_frame_offsets = [history_check_frames[0] - i for i in history_check_frames]
    self.lateral_accel_desired_deque = deque(maxlen=history_check_frames[0])
    self.roll_deque = deque(maxlen=history_check_frames[0])
    self.error_deque = deque(maxlen=history_check_frames[0])
    self.past_future_len = len(self.past_times) + len(self.nn_future_times)
    self.nav_torque_pulse = NavTorquePulseBrain(lac_torque)

  @property
  def _nnff_enabled(self):
    return self.enabled and self.model_valid and self.has_nn_model

  def update_limits(self):
    if not self._nnff_enabled:
      return
    self._pid.set_limits(self.lac_torque.steer_max, -self.lac_torque.steer_max)

  def update_lateral_lag(self, lag):
    super().update_lateral_lag(lag)
    self.nn_future_times = [t + self.desired_lat_jerk_time for t in self.future_times]

  def update_feedforward_torque_space(self, CS):
    torque_from_setpoint = self.torque_from_lateral_accel_in_torque_space(LatControlInputs(self._setpoint, self._roll_compensation, CS.vEgo, CS.aEgo),
                                                                          self.lac_torque.torque_params, gravity_adjusted=False)
    torque_from_measurement = self.torque_from_lateral_accel_in_torque_space(LatControlInputs(self._measurement, self._roll_compensation, CS.vEgo, CS.aEgo),
                                                                             self.lac_torque.torque_params, gravity_adjusted=False)
    self._pid_log.error = float(torque_from_setpoint - torque_from_measurement)
    self._ff = self.torque_from_lateral_accel_in_torque_space(LatControlInputs(self._gravity_adjusted_lateral_accel, self._roll_compensation,
                                                                               CS.vEgo, CS.aEgo), self.lac_torque.torque_params, gravity_adjusted=True)
    self._ff += get_friction_in_torque_space(self._desired_lateral_accel - self._actual_lateral_accel, self._lateral_accel_deadzone,
                                             FRICTION_THRESHOLD, self.lac_torque.torque_params)

  def update_output_torque(self, CS):
    freeze_integrator = self._steer_limited_by_safety or CS.steeringPressed or CS.vEgo < 5
    self._output_torque = self._pid.update(self._pid_log.error,
                                           feedforward=self._ff,
                                           speed=CS.vEgo,
                                           freeze_integrator=freeze_integrator)

  def update_neural_network_feedforward(self, CS, params, calibrated_pose) -> None:
    if not self._nnff_enabled:
      return
    self.update_feedforward_torque_space(CS)
    low_speed_factor = float(np.interp(CS.vEgo, LOW_SPEED_X, LOW_SPEED_Y)) ** 2
    self._setpoint = self._desired_lateral_accel + low_speed_factor * self._desired_curvature
    self._measurement = self._actual_lateral_accel + low_speed_factor * self._actual_curvature
    roll = params.roll
    if calibrated_pose is not None:
      pitch = self.pitch.update(calibrated_pose.orientation.pitch)
      roll = roll_pitch_adjust(roll, pitch)
      self.pitch_last = pitch
    self.roll_deque.append(roll)
    self.lateral_accel_desired_deque.append(self._desired_lateral_accel)
    adjusted_future_times = [t + 0.5 * CS.aEgo * (t / max(CS.vEgo, 1.0)) for t in self.nn_future_times]
    past_rolls = [self.roll_deque[min(len(self.roll_deque) - 1, i)] for i in self.history_frame_offsets]
    future_rolls = [roll_pitch_adjust(np.interp(t, ModelConstants.T_IDXS, self.model_v2.orientation.x) + roll,
                                      np.interp(t, ModelConstants.T_IDXS, self.model_v2.orientation.y) + self.pitch_last) for t in
                    adjusted_future_times]
    past_lateral_accels_desired = [self.lateral_accel_desired_deque[min(len(self.lateral_accel_desired_deque) - 1, i)]
                                   for i in self.history_frame_offsets]
    future_planned_lateral_accels = [np.interp(t, ModelConstants.T_IDXS, self.model_v2.acceleration.y) for t in
                                     adjusted_future_times]

    nnff_setpoint_input = [CS.vEgo, self._setpoint, self.lateral_jerk_setpoint, roll] \
                          + [self._setpoint] * self.past_future_len \
                          + past_rolls + future_rolls
    nnff_measurement_input = [CS.vEgo, self._measurement, self.lateral_jerk_measurement, roll] \
                             + [self._measurement] * self.past_future_len \
                             + past_rolls + future_rolls
    torque_from_setpoint = self.model.evaluate(nnff_setpoint_input)
    torque_from_measurement = self.model.evaluate(nnff_measurement_input)
    self._pid_log.error = torque_from_setpoint - torque_from_measurement
    error_blend_factor = float(np.interp(abs(self._desired_lateral_accel), [1.0, 2.0], [0.0, 1.0]))
    if error_blend_factor > 0.0:
      nnff_error_input = [CS.vEgo, self._setpoint - self._measurement, self.lateral_jerk_setpoint - self.lateral_jerk_measurement, 0.0]
      torque_from_error = self.model.evaluate(nnff_error_input)
      if sign(self._pid_log.error) == sign(torque_from_error) and abs(self._pid_log.error) < abs(torque_from_error):
        self._pid_log.error = self._pid_log.error * (1.0 - error_blend_factor) + torque_from_error * error_blend_factor
    friction_input = self.update_friction_input(self._setpoint, self._measurement)
    nn_input = [CS.vEgo, self._desired_lateral_accel, friction_input, roll] \
               + past_lateral_accels_desired + future_planned_lateral_accels \
               + past_rolls + future_rolls
    self._ff = self.model.evaluate(nn_input)
    if self.model.friction_override:
      self._pid_log.error += get_friction(friction_input, self._lateral_accel_deadzone, FRICTION_THRESHOLD, self.lac_torque.torque_params)

    self.update_output_torque(CS)

  def _snapshot_cycle(self,
                      feedforward_seed,
                      pid_core,
                      pid_trace,
                      torque_goal,
                      torque_actual,
                      roll_bias,
                      deadzone,
                      lat_accel_goal,
                      lat_accel_actual,
                      curvature_goal,
                      curvature_actual,
                      gravity_lat_accel,
                      safety_limited,
                      torque_output) -> None:
    self._ff = feedforward_seed
    self._pid = pid_core
    self._pid_log = pid_trace
    self._setpoint = torque_goal
    self._measurement = torque_actual
    self._roll_compensation = roll_bias
    self._lateral_accel_deadzone = deadzone
    self._desired_lateral_accel = lat_accel_goal
    self._actual_lateral_accel = lat_accel_actual
    self._desired_curvature = curvature_goal
    self._actual_curvature = curvature_actual
    self._gravity_adjusted_lateral_accel = gravity_lat_accel
    self._steer_limited_by_safety = safety_limited
    self._output_torque = torque_output

  def update(self,
             car_state,
             vehicle_model,
             pid_core,
             calibrator,
             feedforward_seed,
             pid_trace,
             torque_goal,
             torque_actual,
             calibrated_pose,
             roll_bias,
             lat_accel_goal,
             lat_accel_actual,
             deadzone,
             gravity_lat_accel,
             curvature_goal,
             curvature_actual,
             safety_limited,
             torque_output):
    self._snapshot_cycle(
      feedforward_seed,
      pid_core,
      pid_trace,
      torque_goal,
      torque_actual,
      roll_bias,
      deadzone,
      lat_accel_goal,
      lat_accel_actual,
      curvature_goal,
      curvature_actual,
      gravity_lat_accel,
      safety_limited,
      torque_output,
    )
    self.update_calculations(car_state, vehicle_model, lat_accel_goal)
    self.update_neural_network_feedforward(car_state, calibrator, calibrated_pose)
    self._output_torque = self.nav_torque_pulse.nudge_output_torque(True, car_state, self._output_torque)
    return self._pid_log, self._output_torque
