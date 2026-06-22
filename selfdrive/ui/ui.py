#!/usr/bin/env python3
import os
import pyray as rl

from openpilot.system.hardware import TICI
from openpilot.common.realtime import config_realtime_process, set_core_affinity
from openpilot.system.ui.lib.application import gui_app
from openpilot.selfdrive.ui.layouts.main import MainLayout
from openpilot.selfdrive.ui.mici.layouts.main import MiciMainLayout
from openpilot.selfdrive.ui.ui_state import ui_state


def main():
  cores = {5, }
  config_realtime_process(0, 51)

  gui_app.init_window("UI")
  big = gui_app.big_ui()
  if big:
    # BIG UI (comma 3/3x): rendered manually each frame (unchanged).
    main_layout = MainLayout()
    main_layout.set_rect(rl.Rectangle(0, 0, gui_app.width, gui_app.height))
  else:
    # mici (comma 4): drives the nav stack; MiciMainLayout pushes itself in __init__
    # and is rendered by gui_app.render() (settings push on top).
    main_layout = MiciMainLayout()
  for should_render in gui_app.render():
    ui_state.update()
    if should_render:
      if big:
        main_layout.render()

      # reaffine after power save offlines our core
      if TICI and os.sched_getaffinity(0) != cores:
        try:
          set_core_affinity(list(cores))
        except OSError:
          pass


if __name__ == "__main__":
  main()
