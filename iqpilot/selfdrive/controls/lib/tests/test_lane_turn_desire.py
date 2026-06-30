"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""

from __future__ import annotations

from dataclasses import dataclass

import pytest
from cereal import custom, log

from openpilot.common.params import Params
from openpilot.iqpilot.selfdrive.controls.lib.helpers.lane_change import AutoLaneChangeMode
from openpilot.iqpilot.selfdrive.controls.lib.helpers.lane_turn import LANE_CHANGE_SPEED_MIN, LaneTurnController
from openpilot.selfdrive.controls.lib.desire_helper import DesireHelper

TurnDirection = custom.IQTurnSignalDirection


def _fresh_controller() -> LaneTurnController:
  hub = DesireHelper()
  controller = LaneTurnController(hub)
  controller.enabled = True
  controller.lane_turn_value = LANE_CHANGE_SPEED_MIN
  controller.turn_direction = TurnDirection.none
  return controller


@dataclass
class _ControllerCase:
  left_blinker: bool
  right_blinker: bool
  speed_mps: float
  blindspot_left: bool
  blindspot_right: bool
  expected: int


@pytest.mark.parametrize("case", [
  _ControllerCase(True, False, 5, False, False, TurnDirection.turnLeft),
  _ControllerCase(False, True, 6, False, False, TurnDirection.turnRight),
  _ControllerCase(True, False, 9, False, False, TurnDirection.none),
  _ControllerCase(True, False, 7, True, False, TurnDirection.none),
  _ControllerCase(False, True, 6, False, True, TurnDirection.none),
  _ControllerCase(False, False, 5, False, False, TurnDirection.none),
  _ControllerCase(True, True, 5, False, False, TurnDirection.none),
])
def test_lane_turn_controller_gate(case: _ControllerCase):
  controller = _fresh_controller()
  controller.update_lane_turn(case.blindspot_left,
                              case.blindspot_right,
                              case.left_blinker,
                              case.right_blinker,
                              case.speed_mps)
  assert controller.get_turn_direction() == case.expected


def test_lane_turn_controller_respects_disable():
  controller = _fresh_controller()
  controller.enabled = False
  controller.update_lane_turn(False, False, True, False, 7)
  assert controller.get_turn_direction() == TurnDirection.none


def test_lane_turn_controller_reacts_to_signal_swaps():
  controller = _fresh_controller()
  controller.update_lane_turn(False, False, True, False, 5)
  assert controller.get_turn_direction() == TurnDirection.turnLeft
  controller.update_lane_turn(False, False, False, True, 6)
  assert controller.get_turn_direction() == TurnDirection.turnRight
  controller.update_lane_turn(False, False, False, False, 7)
  assert controller.get_turn_direction() == TurnDirection.none


@pytest.mark.parametrize(("speed_mps", "expected"), [
  (8.93, TurnDirection.turnLeft),
  (8.96, TurnDirection.none),
  (8.95, TurnDirection.none),
])
def test_lane_turn_threshold(speed_mps: float, expected: int):
  controller = _fresh_controller()
  controller.update_lane_turn(False, True, True, False, speed_mps)
  assert controller.get_turn_direction() == expected


@dataclass
class _FakeCarState:
  vEgo: float = 0.0
  leftBlinker: bool = False
  rightBlinker: bool = False
  leftBlindspot: bool = False
  rightBlindspot: bool = False
  steeringPressed: bool = False
  steeringTorque: float = 0.0
  brakePressed: bool = False


@pytest.fixture
def lane_turn_params():
  params = Params()
  params.put("LaneTurnDesire", True)
  params.put("LaneTurnValue", 20.0)


@pytest.mark.parametrize(("carstate", "lateral_active", "lane_change_prob", "expected_desire"), [
  (_FakeCarState(vEgo=5, leftBlinker=True), True, 1.0, log.Desire.turnLeft),
  (_FakeCarState(vEgo=7, rightBlinker=True), True, 1.0, log.Desire.turnRight),
  (_FakeCarState(vEgo=9, leftBlinker=True, steeringPressed=True, steeringTorque=1), True, 1.0, log.Desire.laneChangeLeft),
  (_FakeCarState(vEgo=9, rightBlinker=True, steeringPressed=True, steeringTorque=-1), True, 1.0, log.Desire.laneChangeRight),
  (_FakeCarState(vEgo=9), False, 1.0, log.Desire.none),
  (_FakeCarState(vEgo=4), True, 1.0, log.Desire.none),
])
def test_desire_helper_lane_turn_priority(carstate: _FakeCarState,
                                          lateral_active: bool,
                                          lane_change_prob: float,
                                          expected_desire,
                                          lane_turn_params):
  desire_logic = DesireHelper()
  desire_logic.alc.lane_change_set_timer = AutoLaneChangeMode.NUDGE
  for _ in range(10):
    desire_logic.update(carstate, lateral_active, lane_change_prob)
  assert desire_logic.desire == expected_desire
