from types import SimpleNamespace

import pytest

from cereal import log
from openpilot.selfdrive.monitoring.dmonitoringd import create_driver_monitoring, get_dm_inputs, use_legacy_dm
from openpilot.selfdrive.monitoring.legacy_policy import DriverMonitoring as LegacyDriverMonitoring
from openpilot.selfdrive.monitoring.policy import DriverMonitoring as UpstreamDriverMonitoring


@pytest.mark.parametrize("device_type, expected_type", [
  ("mici", LegacyDriverMonitoring),
  ("tici", UpstreamDriverMonitoring),
  ("tizi", UpstreamDriverMonitoring),
])
def test_policy_selection(device_type, expected_type):
  assert use_legacy_dm(device_type) == (device_type == "mici")
  assert isinstance(create_driver_monitoring(device_type, False, False), expected_type)


def test_mici_uses_legacy_thresholds():
  dm = create_driver_monitoring("mici", False, False)

  assert dm.settings._DISTRACTED_TIME == 11.
  assert dm.settings._AWARENESS_TIME == 30.
  assert dm.settings._PHONE_THRESH == 0.75


@pytest.mark.parametrize("enabled, lat_active, expected", [
  (False, False, False),
  (True, False, True),
  (False, True, True),
  (True, True, True),
])
def test_iq_enabled_adapter(enabled, lat_active, expected):
  sm = {
    'driverStateV2': object(),
    'liveCalibration': object(),
    'carState': object(),
    'selfdriveState': SimpleNamespace(enabled=enabled),
    'modelV2': object(),
    'carControl': SimpleNamespace(latActive=lat_active),
  }

  assert get_dm_inputs(sm)['selfdriveState'].enabled == expected


def test_legacy_policy_packet_uses_shared_schema():
  dm = create_driver_monitoring("mici", False, False)
  dm.awareness = 0.
  dm.terminal_alert_cnt = 2
  dm.terminal_time = dm.settings._MAX_TERMINAL_DURATION
  dm.too_distracted = True

  state = dm.get_state_packet().driverMonitoringState

  assert state.lockout
  assert state.alert3Count == 2
  assert state.noResponseCount == 1
  assert state.noResponseForceDecel
  assert state.alertLevel == log.DriverMonitoringState.AlertLevel.three
