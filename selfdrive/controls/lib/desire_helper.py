from cereal import custom, log
from openpilot.common.constants import CV
from openpilot.common.realtime import DT_MDL
from openpilot.iqpilot.selfdrive.controls.lib.helpers.lane_change import AutoLaneChangeController, AutoLaneChangeMode
from openpilot.iqpilot.selfdrive.controls.lib.helpers.lane_turn import LaneTurnController

LaneChangeState = log.LaneChangeState
LaneChangeDirection = log.LaneChangeDirection
TurnDirection = custom.IQModelDataV2.TurnDirection
NavManeuverPhase = custom.IQNavState.ManeuverPhase

LANE_CHANGE_SPEED_MIN = 20 * CV.MPH_TO_MS
LANE_CHANGE_TIME_MAX = 10.

DESIRES = {
  LaneChangeDirection.none: {
    LaneChangeState.off: log.Desire.none,
    LaneChangeState.preLaneChange: log.Desire.none,
    LaneChangeState.laneChangeStarting: log.Desire.none,
    LaneChangeState.laneChangeFinishing: log.Desire.none,
  },
  LaneChangeDirection.left: {
    LaneChangeState.off: log.Desire.none,
    LaneChangeState.preLaneChange: log.Desire.none,
    LaneChangeState.laneChangeStarting: log.Desire.laneChangeLeft,
    LaneChangeState.laneChangeFinishing: log.Desire.laneChangeLeft,
  },
  LaneChangeDirection.right: {
    LaneChangeState.off: log.Desire.none,
    LaneChangeState.preLaneChange: log.Desire.none,
    LaneChangeState.laneChangeStarting: log.Desire.laneChangeRight,
    LaneChangeState.laneChangeFinishing: log.Desire.laneChangeRight,
  },
}

# lane-turn assist + nav turn desires map a TurnDirection onto the model's turn desire
TURN_DESIRES = {
  TurnDirection.none: log.Desire.none,
  TurnDirection.turnLeft: log.Desire.turnLeft,
  TurnDirection.turnRight: log.Desire.turnRight,
}


class DesireHelper:
  """Lane-change desire generation.

  The lane-change state machine is kept identical to upstream openpilot (deep_rl3) so the model
  sees stock-equivalent lane-change desires. IQ.Pilot's lane-change *controller* features are layered
  on without altering that state machine:
    - AutoLaneChangeController (nudgeless / timed auto-start) only adds an alternative start trigger
      alongside the stock torque nudge, and is fully gated by the same blindspot check.
    - LaneTurnController (low-speed turn assist) and nav turn desires are pure desire *overrides*
      applied after the state machine; nav is limited to blinker-equivalent turn desires (no geometry).
  """

  def __init__(self):
    self.lane_change_state = LaneChangeState.off
    self.lane_change_direction = LaneChangeDirection.none
    self.lane_change_timer = 0.0
    self.lane_change_ll_prob = 1.0
    self.prev_one_blinker = False
    self.desire = log.Desire.none

    # IQ.Pilot lane-change controller features (kept working)
    self.alc = AutoLaneChangeController(self)
    self.lane_turn_controller = LaneTurnController(self)
    self.lane_turn_direction = TurnDirection.none
    self.nav_turn_direction = TurnDirection.none

  @staticmethod
  def get_lane_change_direction(carstate):
    if carstate.leftBlinker:
      return LaneChangeDirection.left
    if carstate.rightBlinker:
      return LaneChangeDirection.right
    return LaneChangeDirection.none

  @staticmethod
  def _nav_turn_desire(nav_state):
    # nav contributes a blinker-equivalent turn desire only (no lane geometry, no lane-change init)
    if nav_state is None or not getattr(nav_state, "active", False):
      return TurnDirection.none
    if getattr(nav_state, "maneuverPhase", NavManeuverPhase.none) == NavManeuverPhase.turnActive and \
       getattr(nav_state, "shouldSendTurnDesire", False):
      return getattr(nav_state, "turnDesireDirection", TurnDirection.none)
    return TurnDirection.none

  def update(self, carstate, lateral_active, lane_change_prob, nav_state=None, modeldata=None, radar_state=None):
    v_ego = carstate.vEgo
    one_blinker = carstate.leftBlinker != carstate.rightBlinker
    below_lane_change_speed = v_ego < LANE_CHANGE_SPEED_MIN

    # opt-in desire overrides (computed up front, applied after the lane-change state machine)
    self.lane_turn_controller.update_params()
    self.lane_turn_controller.update_lane_turn(
      blindspot_left=carstate.leftBlindspot,
      blindspot_right=carstate.rightBlindspot,
      left_blinker=carstate.leftBlinker,
      right_blinker=carstate.rightBlinker,
      v_ego=v_ego,
    )
    self.lane_turn_direction = self.lane_turn_controller.get_turn_direction()
    self.nav_turn_direction = self._nav_turn_desire(nav_state)

    # advance the nudgeless/auto timer params (no-op in NUDGE mode)
    self.alc.update_params()

    # ---- lane-change state machine (mirrors stock openpilot) ----
    if not lateral_active or self.lane_change_timer > LANE_CHANGE_TIME_MAX or \
       self.alc.lane_change_set_timer == AutoLaneChangeMode.OFF:
      self.lane_change_state = LaneChangeState.off
      self.lane_change_direction = LaneChangeDirection.none
    else:
      # LaneChangeState.off
      if self.lane_change_state == LaneChangeState.off and one_blinker and not self.prev_one_blinker and not below_lane_change_speed:
        self.lane_change_state = LaneChangeState.preLaneChange
        self.lane_change_ll_prob = 1.0
        self.lane_change_direction = self.get_lane_change_direction(carstate)

      # LaneChangeState.preLaneChange
      elif self.lane_change_state == LaneChangeState.preLaneChange:
        self.lane_change_direction = self.get_lane_change_direction(carstate)

        torque_applied = carstate.steeringPressed and \
                         ((carstate.steeringTorque > 0 and self.lane_change_direction == LaneChangeDirection.left) or
                          (carstate.steeringTorque < 0 and self.lane_change_direction == LaneChangeDirection.right))

        blindspot_detected = ((carstate.leftBlindspot and self.lane_change_direction == LaneChangeDirection.left) or
                              (carstate.rightBlindspot and self.lane_change_direction == LaneChangeDirection.right))

        # nudgeless/auto-start timer; ready only after the configured delay, never with brake/blindspot
        self.alc.update_lane_change(blindspot_detected=blindspot_detected, brake_pressed=carstate.brakePressed)

        if not one_blinker or below_lane_change_speed:
          self.lane_change_state = LaneChangeState.off
          self.lane_change_direction = LaneChangeDirection.none
        elif (torque_applied or self.alc.auto_lane_change_allowed) and not blindspot_detected:
          self.lane_change_state = LaneChangeState.laneChangeStarting

      # LaneChangeState.laneChangeStarting
      elif self.lane_change_state == LaneChangeState.laneChangeStarting:
        # fade out over .5s
        self.lane_change_ll_prob = max(self.lane_change_ll_prob - 2 * DT_MDL, 0.0)

        # 98% certainty
        if lane_change_prob < 0.02 and self.lane_change_ll_prob < 0.01:
          self.lane_change_state = LaneChangeState.laneChangeFinishing

      # LaneChangeState.laneChangeFinishing
      elif self.lane_change_state == LaneChangeState.laneChangeFinishing:
        # fade in laneline over 1s
        self.lane_change_ll_prob = min(self.lane_change_ll_prob + DT_MDL, 1.0)

        if self.lane_change_ll_prob > 0.99:
          self.lane_change_direction = LaneChangeDirection.none
          if one_blinker:
            self.lane_change_state = LaneChangeState.preLaneChange
          else:
            self.lane_change_state = LaneChangeState.off

    if self.lane_change_state in (LaneChangeState.off, LaneChangeState.preLaneChange):
      self.lane_change_timer = 0.0
    else:
      self.lane_change_timer += DT_MDL

    self.prev_one_blinker = one_blinker
    self.alc.update_state()

    # ---- desire output: nav turn > lane-turn assist > lane change ----
    if self.nav_turn_direction != TurnDirection.none:
      self.desire = TURN_DESIRES[self.nav_turn_direction]
    elif self.lane_turn_direction != TurnDirection.none:
      self.desire = TURN_DESIRES[self.lane_turn_direction]
    else:
      self.desire = DESIRES[self.lane_change_direction][self.lane_change_state]
