"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
# DISCLAIMER: This code is intended principally for development and debugging purposes.
# Although it provides a standalone entry point to the program, users should refer
# to the actual implementations for consumption. Usage outside of development scenarios
# is not advised and could lead to unpredictable results.

import threading
import traceback

from openpilot.common.realtime import Ratekeeper, config_realtime_process
from openpilot.iqpilot.iq_maps.road_data import debug_road_data
from openpilot.iqpilot.iq_maps.road_data.iq_road_layer import IQRoadLayer


def excepthook(args):
  debug_road_data(f'IQ maps threading exception:\n{args}')
  traceback.print_exception(args.exc_type, args.exc_value, args.exc_traceback)


def live_map_data_iq_thread():
  config_realtime_process([0, 1, 2, 3], 5)

  live_map_iq = IQRoadLayer()
  rk = Ratekeeper(1, print_delay_threshold=None)

  while True:
    live_map_iq.step()
    rk.keep_time()


def main():
  threading.excepthook = excepthook
  live_map_data_iq_thread()


if __name__ == "__main__":
  main()
