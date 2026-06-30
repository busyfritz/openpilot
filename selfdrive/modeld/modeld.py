"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""

from __future__ import annotations

import argparse

from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.modeld.stock_iq_modeld import LAT_SMOOTH_SECONDS, run_stock_modeld


def main(demo: bool = False):
  run_stock_modeld(demo=demo)


if __name__ == "__main__":
  try:
    parser = argparse.ArgumentParser()
    parser.add_argument("--demo", action="store_true", help="A boolean for demo mode.")
    args = parser.parse_args()
    main(demo=args.demo)
  except KeyboardInterrupt:
    cloudlog.warning("got SIGINT")
