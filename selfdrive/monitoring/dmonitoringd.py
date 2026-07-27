#!/usr/bin/env python3
from types import SimpleNamespace

import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import config_realtime_process
from openpilot.selfdrive.monitoring.legacy_policy import DRIVER_MONITOR_SETTINGS as LegacySettings
from openpilot.selfdrive.monitoring.legacy_policy import DriverMonitoring as LegacyDriverMonitoring
from openpilot.selfdrive.monitoring.policy import DriverMonitoring as UpstreamDriverMonitoring
from openpilot.system.hardware import HARDWARE


def use_legacy_dm(device_type: str) -> bool:
  return device_type == 'mici'


def create_driver_monitoring(device_type: str, rhd_saved: bool, always_on: bool):
  if use_legacy_dm(device_type):
    return LegacyDriverMonitoring(rhd_saved=rhd_saved, settings=LegacySettings(device_type), always_on=always_on)
  return UpstreamDriverMonitoring(rhd_saved=rhd_saved, always_on=always_on)


def get_dm_inputs(sm):
  return {
    'driverStateV2': sm['driverStateV2'],
    'liveCalibration': sm['liveCalibration'],
    'carState': sm['carState'],
    'selfdriveState': SimpleNamespace(enabled=sm['selfdriveState'].enabled or sm['carControl'].latActive),
    'modelV2': sm['modelV2'],
  }


def dmonitoringd_thread():
  config_realtime_process([0, 1, 2, 3], 5)

  params = Params()
  pm = messaging.PubMaster(['driverMonitoringState'])
  sm = messaging.SubMaster(['driverStateV2', 'liveCalibration', 'carState', 'selfdriveState', 'modelV2',
                            'carControl'], poll='driverStateV2')

  device_type = HARDWARE.get_device_type()
  legacy_dm = use_legacy_dm(device_type)
  DM = create_driver_monitoring(device_type, params.get_bool("IsRhdDetected"), params.get_bool("AlwaysOnDM"))
  demo_mode=False

  # 20Hz <- dmonitoringmodeld
  while True:
    sm.update()
    if not sm.updated['driverStateV2']:
      # iterate when model has new output
      continue

    valid = sm.all_checks()
    if demo_mode and sm.valid['driverStateV2']:
      DM.run_step(sm if legacy_dm else get_dm_inputs(sm), demo=True)
    elif valid:
      DM.run_step(sm if legacy_dm else get_dm_inputs(sm), demo=demo_mode)

    # publish
    dat = DM.get_state_packet(valid=valid)
    pm.send('driverMonitoringState', dat)

    # load live always-on toggle
    if sm['driverStateV2'].frameId % 40 == 1:
      DM.always_on = params.get_bool("AlwaysOnDM")
      demo_mode = params.get_bool("IsDriverViewEnabled")

    # save rhd virtual toggle every 5 mins
    wheelpos_offsetter = DM.wheelpos.prob_offseter if legacy_dm else DM.wheelpos_offsetter
    if (sm['driverStateV2'].frameId % 6000 == 0 and not demo_mode and
     wheelpos_offsetter.filtered_stat.n > DM.settings._WHEELPOS_FILTER_MIN_COUNT and
     DM.wheel_on_right == (wheelpos_offsetter.filtered_stat.M > DM.settings._WHEELPOS_THRESHOLD)):
      params.put_bool("IsRhdDetected", DM.wheel_on_right)

def main():
  dmonitoringd_thread()


if __name__ == '__main__':
  main()
