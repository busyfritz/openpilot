"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
import random
import time

import pytest
from pytest_mock import MockerFixture

from cereal import custom
from openpilot.iqpilot.selfdrive.controls.lib.speed_limit import LIMIT_MAX_MAP_DATA_AGE

from openpilot.iqpilot.selfdrive.controls.lib.speed_limit.speed_limit_resolver import (
  SpeedLimitResolver, ALL_SOURCES, LIMIT_HOLD_RADIUS_M, LIMIT_HOLD_TIME_S)
from openpilot.iqpilot.selfdrive.controls.lib.speed_limit.common import Policy

SpeedLimitSource = custom.IQPlan.SpeedLimit.Source


def create_mock(properties, mocker: MockerFixture):
  mock = mocker.MagicMock()
  for _property, value in properties.items():
    setattr(mock, _property, value)
  return mock


def setup_sm_mock(mocker: MockerFixture):
  cruise_speed_limit = random.uniform(0, 120)
  live_map_data_limit = random.uniform(0, 120)

  car_state = create_mock({
    'gasPressed': False,
    'brakePressed': False,
    'standstill': False,
  }, mocker)
  car_state_iq = create_mock({
    'speedLimit': cruise_speed_limit,
  }, mocker)
  live_map_data = create_mock({
    'speedLimit': live_map_data_limit,
    'speedLimitValid': True,
    'speedLimitAhead': 0.,
    'speedLimitAheadValid': 0.,
    'speedLimitAheadDistance': 0.,
  }, mocker)
  gps_data = create_mock({
    'unixTimestampMillis': time.monotonic() * 1e3,
  }, mocker)
  sm_mock = mocker.MagicMock()
  sm_mock.__getitem__.side_effect = lambda key: {
    'carState': car_state,
    'iqLiveData': live_map_data,
    'iqCarState': car_state_iq,
    'gpsLocation': gps_data,
  }[key]
  return sm_mock


parametrized_policies = pytest.mark.parametrize(
  "policy, sm_key, function_key", [
    (Policy.car_state_only, 'iqCarState', SpeedLimitSource.car),
    (Policy.car_state_priority, 'iqCarState', SpeedLimitSource.car),
    (Policy.map_data_only, 'iqLiveData', SpeedLimitSource.map),
    (Policy.map_data_priority, 'iqLiveData', SpeedLimitSource.map),
  ],
  ids=lambda val: val.name if hasattr(val, 'name') else str(val)
)


@pytest.mark.parametrize("resolver_class", [SpeedLimitResolver])
class TestSpeedLimitResolverValidation:

  @pytest.mark.parametrize("policy", list(Policy), ids=lambda policy: policy.name)
  def test_initial_state(self, resolver_class, policy):
    resolver = resolver_class()
    resolver.policy = policy
    for source in ALL_SOURCES:
      if source in resolver.limit_solutions:
        assert resolver.limit_solutions[source] == 0.
        assert resolver.distance_solutions[source] == 0.

  @parametrized_policies
  def test_resolver(self, resolver_class, policy, sm_key, function_key, mocker: MockerFixture):
    resolver = resolver_class()
    resolver.policy = policy
    sm_mock = setup_sm_mock(mocker)
    source_speed_limit = sm_mock[sm_key].speedLimit

    # Assert the resolver
    resolver.update(source_speed_limit, sm_mock)
    assert resolver.speed_limit == source_speed_limit
    assert resolver.source == ALL_SOURCES[function_key]

  def test_resolver_combined(self, resolver_class, mocker: MockerFixture):
    resolver = resolver_class()
    resolver.policy = Policy.combined
    sm_mock = setup_sm_mock(mocker)
    socket_to_source = {'iqCarState': SpeedLimitSource.car, 'iqLiveData': SpeedLimitSource.map}
    minimum_key, minimum_speed_limit = min(
      ((key, sm_mock[key].speedLimit) for key in
       socket_to_source.keys()), key=lambda x: x[1])

    # Assert the resolver
    resolver.update(minimum_speed_limit, sm_mock)
    assert resolver.speed_limit == minimum_speed_limit
    assert resolver.source == socket_to_source[minimum_key]

  @parametrized_policies
  def test_parser(self, resolver_class, policy, sm_key, function_key, mocker: MockerFixture):
    resolver = resolver_class()
    resolver.policy = policy
    sm_mock = setup_sm_mock(mocker)
    source_speed_limit = sm_mock[sm_key].speedLimit

    # Assert the parsing
    resolver.update(source_speed_limit, sm_mock)
    assert resolver.limit_solutions[ALL_SOURCES[function_key]] == source_speed_limit
    assert resolver.distance_solutions[ALL_SOURCES[function_key]] == 0.

  @pytest.mark.parametrize("policy", list(Policy), ids=lambda policy: policy.name)
  def test_resolve_interaction_in_update(self, resolver_class, policy, mocker: MockerFixture):
    v_ego = 50
    resolver = resolver_class()
    resolver.policy = policy

    sm_mock = setup_sm_mock(mocker)
    resolver.update(v_ego, sm_mock)

    # After resolution
    assert resolver.speed_limit is not None
    assert resolver.distance is not None
    assert resolver.source is not None

  def test_held_limit_expires_after_radius_and_time(self, resolver_class):
    resolver = resolver_class()
    resolver.v_ego = 30.0
    resolver.speed_limit = 25.0
    resolver.update_speed_limit_states()
    assert resolver.speed_limit_last == 25.0

    resolver.speed_limit = 0.0
    for _ in range(20):
      resolver.update_speed_limit_states()
    assert resolver.speed_limit_last == 25.0

    resolver._dist_since_limit = LIMIT_HOLD_RADIUS_M + 100.0
    resolver._time_since_limit = 0.0
    resolver.update_speed_limit_states()
    assert resolver.speed_limit_last == 25.0

    resolver._dist_since_limit = LIMIT_HOLD_RADIUS_M + 100.0
    resolver._time_since_limit = LIMIT_HOLD_TIME_S + 10.0
    resolver.update_speed_limit_states()
    assert resolver.speed_limit_last == 0.0
    assert resolver.speed_limit_final_last == 0.0

  def test_reacquiring_limit_resets_expiry(self, resolver_class):
    resolver = resolver_class()
    resolver.v_ego = 30.0
    resolver.speed_limit = 25.0
    resolver.update_speed_limit_states()
    resolver.speed_limit = 0.0
    resolver._dist_since_limit = LIMIT_HOLD_RADIUS_M - 1.0
    resolver._time_since_limit = LIMIT_HOLD_TIME_S - 1.0
    resolver.speed_limit = 20.0
    resolver.update_speed_limit_states()
    assert resolver._dist_since_limit == 0.0
    assert resolver._time_since_limit == 0.0
    assert resolver.speed_limit_last == 20.0

  @pytest.mark.parametrize("policy", list(Policy), ids=lambda policy: policy.name)
  def test_old_map_data_ignored(self, resolver_class, policy, mocker: MockerFixture):
    resolver = resolver_class()
    resolver.policy = policy
    sm_mock = mocker.MagicMock()
    sm_mock['gpsLocation'].unixTimestampMillis = (time.monotonic() - 2 * LIMIT_MAX_MAP_DATA_AGE) * 1e3
    resolver._read_map_solution(sm_mock)
    assert resolver.limit_solutions[SpeedLimitSource.map] == 0.
    assert resolver.distance_solutions[SpeedLimitSource.map] == 0.
