"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
import requests
import threading
import time
import pyray as rl

from openpilot.common.api import api_get
from openpilot.common.constants import CV
from openpilot.common.params import Params
from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.ui.lib.api_helpers import get_token
from openpilot.common.time_helpers import system_time_valid
from openpilot.selfdrive.ui.ui_state import ui_state, device
from openpilot.iqpilot.konn3kt.registration import UNREGISTERED_DONGLE_ID
from openpilot.system.ui.lib.application import gui_app, FontWeight, FONT_SCALE
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.lib.text_measure import measure_text_cached
from openpilot.system.ui.widgets import Widget


class TripsLayout(Widget):
  PARAM_KEY = "ApiCache_DriveStats"
  UPDATE_INTERVAL = 30  # seconds

  CARD_BG = rl.Color(38, 40, 46, 255)
  CARD_BORDER = rl.Color(255, 255, 255, 18)
  TEAL = rl.Color(30, 200, 168, 255)
  LABEL_TEAL = rl.Color(93, 202, 165, 255)
  UNIT = rl.Color(138, 139, 144, 255)
  DIVIDER = rl.Color(255, 255, 255, 16)

  def __init__(self):
    super().__init__()
    self._params = Params()
    self._session = requests.Session()
    self._stats = self._get_stats()

    # Unified height so the three columns line up; tinted teal at draw time.
    self._icon_drives = gui_app.texture("icons_mici/wheel.png", 64, 64, keep_aspect_ratio=True)
    self._icon_distance = gui_app.texture("icons/road.png", 88, 64, keep_aspect_ratio=True)
    self._icon_hours = gui_app.texture("../../iqpilot/selfdrive/assets/icons/clock.png", 64, 64, keep_aspect_ratio=True)

    self._running = True
    self._update_thread = threading.Thread(target=self._update_loop, daemon=True)
    self._update_thread.start()

  def __del__(self):
    self._running = False
    try:
      if self._update_thread and self._update_thread.is_alive():
        self._update_thread.join(timeout=1.0)
    except Exception:
      pass

  def _get_stats(self):
    stats = self._params.get(self.PARAM_KEY)
    if not stats:
      return {}
    try:
      return stats
    except Exception:
      cloudlog.exception(f"Failed to decode drive stats: {stats}")
      return {}

  def _fetch_drive_stats(self):
    try:
      dongle_id = self._params.get("DongleId")
      if not dongle_id or dongle_id == UNREGISTERED_DONGLE_ID:
        return
      # clock not NTP-synced yet at boot -> token can't be minted; skip quietly
      if not system_time_valid():
        return
      identity_token = get_token(dongle_id)
      response = api_get(f"v1.1/devices/{dongle_id}/stats", access_token=identity_token, session=self._session)
      if response.status_code == 200:
        data = response.json()
        self._stats = data
        self._params.put(self.PARAM_KEY, data)
    except Exception as e:
      cloudlog.error(f"Failed to fetch drive stats: {e}")

  def _update_loop(self):
    while self._running:
      if not ui_state.started and device._awake:
        self._fetch_drive_stats()
      time.sleep(self.UPDATE_INTERVAL)

  def _render_stat_group(self, x, y, width, height, title, data, is_metric):
    # Card
    card = rl.Rectangle(x, y, width, height)
    rl.draw_rectangle_rounded(card, 0.10, 20, self.CARD_BG)
    rl.draw_rectangle_rounded_lines_ex(card, 0.10, 20, 2, self.CARD_BORDER)

    # Section label: teal tick + muted-teal title
    pad = 44
    label_y = y + 36
    tick_h = 30
    title_size = 34 * FONT_SCALE
    rl.draw_rectangle_rounded(rl.Rectangle(x + pad, label_y, 6, tick_h), 0.5, 6, self.TEAL)
    title_font = gui_app.font(FontWeight.BOLD)
    rl.draw_text_ex(title_font, title, rl.Vector2(x + pad + 22, label_y + (tick_h - title_size) / 2),
                    title_size, 4, self.LABEL_TEAL)

    # Three columns: icon + value + unit, vertically centered in the area below the label
    col_width = width / 3
    content_top = label_y + tick_h + 20
    content_bottom = y + height - 30

    number_font = gui_app.font(FontWeight.BOLD)
    unit_font = gui_app.font(FontWeight.MEDIUM)
    number_size = 84 * FONT_SCALE
    unit_size = 30 * FONT_SCALE
    unit_spacing = 2.0
    icon_gap = 16
    num_gap = 14

    routes = int(data.get("routes", 0))
    distance = data.get("distance", 0)
    distance_str = str(int(distance * CV.MPH_TO_KPH)) if is_metric else str(int(distance))
    hours = int(data.get("minutes", 0) / 60)
    dist_unit = tr("KM") if is_metric else tr("Miles")

    # Column dividers
    div_top = content_top + 4
    div_bottom = content_bottom - 4
    for i in (1, 2):
      dx = x + col_width * i
      rl.draw_line_ex(rl.Vector2(dx, div_top), rl.Vector2(dx, div_bottom), 1, self.DIVIDER)

    def draw_col(col_idx, icon, value, unit):
      center_x = x + (col_width * col_idx) + (col_width / 2)
      unit = unit.upper()

      val_w = measure_text_cached(number_font, value, int(number_size)).x
      unit_w = measure_text_cached(unit_font, unit, int(unit_size)).x + unit_spacing * max(0, len(unit) - 1)

      block_h = icon.height + icon_gap + number_size + num_gap + unit_size
      start_y = content_top + max(0.0, (content_bottom - content_top - block_h) / 2)

      rl.draw_texture(icon, int(center_x - icon.width / 2), int(start_y), self.TEAL)
      num_y = start_y + icon.height + icon_gap
      rl.draw_text_ex(number_font, value, rl.Vector2(center_x - val_w / 2, num_y), number_size, 0, rl.WHITE)
      unit_y = num_y + number_size + num_gap
      rl.draw_text_ex(unit_font, unit, rl.Vector2(center_x - unit_w / 2, unit_y), unit_size, unit_spacing, self.UNIT)

    draw_col(0, self._icon_drives, str(routes), tr("Drives"))
    draw_col(1, self._icon_distance, distance_str, dist_unit)
    draw_col(2, self._icon_hours, str(hours), tr("Hours"))

    return y + height

  def _render(self, rect: rl.Rectangle):
    x, y, w = rect.x, rect.y, rect.width

    spacing = 28
    card_height = (rect.height - spacing) / 2

    is_metric = self._params.get_bool("IsMetric")
    all_time = self._stats.get("all", {})
    week = self._stats.get("week", {})

    y = self._render_stat_group(x, y, w, card_height, tr("ALL TIME"), all_time, is_metric)
    y += spacing
    self._render_stat_group(x, y, w, card_height, tr("PAST WEEK"), week, is_metric)

    return -1
