"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""

import pyray as rl

from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import gui_app, FontWeight
from openpilot.system.ui.lib.text_measure import measure_text_cached
from openpilot.system.ui.widgets import Widget

FONT_SIZE = 28


class RoadNameRendererMici(Widget):
  def __init__(self):
    super().__init__()
    self.road_name = ""
    self.font = gui_app.font(FontWeight.SEMI_BOLD)

  def update(self):
    sm = ui_state.sm
    if sm.recv_frame["carState"] < ui_state.started_frame:
      return
    if sm.updated["iqLiveData"]:
      self.road_name = sm["iqLiveData"].roadName

  def _render(self, rect: rl.Rectangle):
    if not self.road_name or not ui_state.road_name_toggle:
      return

    text = self.road_name
    ts = measure_text_cached(self.font, text, FONT_SIZE)
    rect_width = max(120, min(ts.x + 28, rect.width - 200))
    road_rect = rl.Rectangle(rect.x + rect.width / 2 - rect_width / 2, rect.y + 8, rect_width, FONT_SIZE + 14)
    rl.draw_rectangle_rounded(road_rect, 0.35, 8, rl.Color(0, 0, 0, 140))

    max_w = road_rect.width - 16
    if ts.x > max_w:
      while ts.x > max_w and len(text) > 3:
        text = text[:-1]
        ts = measure_text_cached(self.font, text + "...", FONT_SIZE)
      text = text + "..."

    sz = measure_text_cached(self.font, text, FONT_SIZE)
    origin = rl.Vector2(road_rect.x + road_rect.width / 2 - sz.x / 2, road_rect.y + road_rect.height / 2 - sz.y / 2)
    rl.draw_text_ex(self.font, text, origin, FONT_SIZE, 0, rl.Color(255, 255, 255, 210))
