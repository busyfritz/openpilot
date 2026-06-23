"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""

import os
import re
import time

import pyray as rl

from cereal import custom

from openpilot.system.ui.iqpilot.widgets.helpers.star_icon import draw_star

from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import gui_app
from openpilot.system.ui.widgets.scroller import NavScroller
from openpilot.selfdrive.ui.mici.widgets.stock_button import BigButton, BigParamControl
from openpilot.selfdrive.ui.mici.widgets.stock_dialog import BigConfirmationDialog
from openpilot.selfdrive.ui.mici.layouts.settings.iq_widgets import MappedParamToggle

from openpilot.iqpilot.models.runners.constants import CUSTOM_MODEL_PATH
from openpilot.iqpilot.models.helpers import get_default_model_bundle

_DELAY_OPTIONS = ["0.05s", "0.10s", "0.15s", "0.20s", "0.25s", "0.30s", "0.35s", "0.40s", "0.45s", "0.50s"]
_DELAY_VALUES = [0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45, 0.50]

_LANE_TURN_OPTIONS = ["slow", "normal", "fast"]
_LANE_TURN_VALUES = [15.0, 19.0, 20.0]

_DL = custom.IQModelManager.DownloadStatus


class _ModelSelectPanel(NavScroller):
  """A throwaway scroller panel (folder list or bundle list) pushed onto the nav stack."""
  def __init__(self, items):
    super().__init__()
    self._scroller.add_widgets(items)


class _ModelButton(BigButton):
  """A bundle in the model list: single tap selects (download), double tap toggles favorite.

  A golden star is drawn in the corner when the model is favorited.
  """
  THRESHOLD = 0.4
  _STAR_GOLD = rl.Color(0xFF, 0xC1, 0x07, 255)

  def __init__(self, bundle, on_select, on_favorite, is_favorite):
    super().__init__(bundle.displayName.lower())
    self._bundle = bundle
    self._on_select = on_select
    self._on_favorite = on_favorite
    self._is_favorite = is_favorite
    self._pending_t = 0.0
    self._pending_pos = None

  def _handle_mouse_release(self, mouse_pos):
    now = time.monotonic()
    if self._pending_pos is not None and now - self._pending_t < self.THRESHOLD:
      self._pending_pos = None
      self._pending_t = 0.0
      self._is_favorite = self._on_favorite(self._bundle)
      return
    self._pending_t = now
    self._pending_pos = mouse_pos

  def _update_state(self):
    super()._update_state()
    if self._pending_pos is not None and time.monotonic() - self._pending_t >= self.THRESHOLD:
      self._pending_pos = None
      self._on_select(self._bundle)

  def _render(self, _):
    super()._render(_)
    if self._is_favorite:
      cx = self._rect.x + self._rect.width - 46
      cy = self._rect.y + 46
      draw_star(cx, cy, 24, True, self._STAR_GOLD)


class ModelsLayoutMici(NavScroller):
  def __init__(self):
    super().__init__()
    self._last_cache_t = 0.0
    self._clear_icon = gui_app.texture("icons_mici/settings/developer_icon.png", 56, 56)
    self._reset_icon = gui_app.texture("icons_mici/wheel.png", 56, 56)

    self._current = BigButton("current model")
    self._current.set_click_callback(self._show_folders)

    self._cancel = BigButton("cancel download")
    self._cancel.set_click_callback(lambda: ui_state.params.remove("ModelManager_DownloadIndex"))
    self._cancel.set_visible(self._is_downloading)

    self._refresh = BigButton("refresh model list")
    self._refresh.set_click_callback(lambda: ui_state.params.put("ModelManager_LastSyncTime", 0))

    self._clear = BigButton("clear model cache")
    self._clear.set_click_callback(self._confirm_clear_cache)
    self._clear.set_enabled(lambda: ui_state.is_offroad())

    self._lagd = BigParamControl("live learning steer delay", "LagdToggle")
    self._sw_delay = MappedParamToggle("software delay", "LagdToggleDelay", _DELAY_OPTIONS, _DELAY_VALUES)
    self._sw_delay.set_visible(lambda: not self._lagd._checked)

    self._lane_turn = BigParamControl("use lane turn desires", "LaneTurnDesire")
    self._lane_speed = MappedParamToggle("lane turn speed", "LaneTurnValue", _LANE_TURN_OPTIONS, _LANE_TURN_VALUES)
    self._lane_speed.set_visible(lambda: self._lane_turn._checked)

    self._main_items = [self._current, self._cancel, self._refresh, self._clear,
                        self._lagd, self._sw_delay, self._lane_turn, self._lane_speed]
    self._scroller.add_widgets(self._main_items)

  @property
  def model_manager(self):
    return ui_state.sm["iqModelManager"]

  @staticmethod
  def _has_download_request() -> bool:
    try:
      return int(ui_state.params.get("ModelManager_DownloadIndex")) >= 0
    except (TypeError, ValueError):
      return False

  def _is_downloading(self) -> bool:
    if not self._has_download_request():
      return False
    try:
      return self.model_manager.selectedBundle.status == _DL.downloading
    except Exception:
      return False

  @staticmethod
  def _calculate_cache_size() -> float:
    if os.path.exists(CUSTOM_MODEL_PATH):
      return sum(os.path.getsize(os.path.join(CUSTOM_MODEL_PATH, f)) for f in os.listdir(CUSTOM_MODEL_PATH)) / (1024 ** 2)
    return 0.0

  def _group_folders(self, bundles):
    folders: dict = {}
    for bundle in bundles:
      folder = next((ov.value for ov in bundle.overrides if ov.key == "folder"), "")
      folders.setdefault(folder, []).append(bundle)
    return folders

  @staticmethod
  def _read_favorites() -> set:
    favs = ui_state.params.get("ModelManager_Favs")
    return set(favs.split(';')) if favs else set()

  def _toggle_favorite(self, bundle) -> bool:
    favs = self._read_favorites()
    if bundle.ref in favs:
      favs.discard(bundle.ref)
    else:
      favs.add(bundle.ref)
    ui_state.params.put("ModelManager_Favs", ';'.join(sorted(favs)))
    return bundle.ref in favs

  def _confirm_clear_cache(self):
    gui_app.push_widget(BigConfirmationDialog("slide to\nclear cache", self._clear_icon,
                                              lambda: ui_state.params.put_bool("ModelManager_ClearCache", True),
                                              red=True))

  def _show_folders(self):
    bundles = list(self.model_manager.availableBundles)
    favorites = self._read_favorites()
    btns = []

    default_btn = BigButton("default model")
    default_btn.set_click_callback(self._select_default)
    btns.append(default_btn)

    if favorites and (fav_bundles := [b for b in bundles if b.ref in favorites]):
      fav_btn = BigButton("favorites", str(len(fav_bundles)))
      fav_btn.set_click_callback(lambda fb=fav_bundles: self._show_bundles(fb))
      btns.append(fav_btn)

    folders = self._group_folders(bundles)
    for folder in sorted(folders, key=lambda f: max((b.index for b in folders[f]), default=-1), reverse=True):
      name = folder.lower() if folder else "other"
      folder_bundles = sorted(folders[folder], key=lambda b: b.index, reverse=True)
      if folder_bundles and (m := re.search(r'\(([^)]*)\)[^(]*$', folder_bundles[0].displayName)):
        name += f" ({m.group(1)})"
      btn = BigButton(name)
      btn.set_click_callback(lambda fb=folder_bundles: self._show_bundles(fb))
      btns.append(btn)

    gui_app.push_widget(_ModelSelectPanel(btns))

  def _show_bundles(self, bundles):
    favorites = self._read_favorites()
    btns = [_ModelButton(b, self._select_model, self._toggle_favorite, b.ref in favorites) for b in bundles]
    gui_app.push_widget(_ModelSelectPanel(btns))

  def _generation_changed(self, bundle) -> bool:
    try:
      active = self.model_manager.activeBundle
      return bool(active and active.ref and bundle.generation != active.generation)
    except Exception:
      return False

  def _select_model(self, bundle):
    ui_state.params.put("ModelManager_DownloadIndex", bundle.index)
    cb = self._show_reset_calibration_prompt if self._generation_changed(bundle) else None
    gui_app.pop_widgets_to(self, callback=cb)

  def _select_default(self):
    default_bundle = get_default_model_bundle(self.model_manager.availableBundles)
    if default_bundle:
      ui_state.params.put("ModelManager_DownloadIndex", default_bundle.index)
    gui_app.pop_widgets_to(self, callback=self._show_reset_calibration_prompt)

  def _show_reset_calibration_prompt(self):
    def _reset():
      ui_state.params.remove("CalibrationParams")
      ui_state.params.remove("LiveTorqueParameters")
    gui_app.push_widget(BigConfirmationDialog("slide to\nreset calibration", self._reset_icon, _reset))

  def _update_state(self):
    super()._update_state()

    if self._is_downloading():
      self._current.set_value(self._download_progress_text())
    else:
      self._current.set_value(self._active_model_name())
    self._current.set_enabled(ui_state.is_offroad())

    now = time.monotonic()
    if now - self._last_cache_t > 1.0:
      self._last_cache_t = now
      self._clear.set_value(f"{self._calculate_cache_size():.1f} MB")

    self._lagd.refresh()
    self._update_lagd_subtext()
    self._sw_delay.refresh()
    self._lane_turn.refresh()
    self._lane_speed.refresh()

  def _update_lagd_subtext(self):
    if self._lagd._checked:
      try:
        self._lagd.set_value(f"live {ui_state.sm['liveDelay'].lateralDelay:.3f} s")
      except Exception:
        self._lagd.set_value("")
      return
    try:
      sw = float(ui_state.params.get("LagdToggleDelay", return_default=True))
    except (TypeError, ValueError):
      sw = 0.2
    if ui_state.CP is not None:
      self._lagd.set_value(f"total {ui_state.CP.steerActuatorDelay + sw:.2f} s")
    else:
      self._lagd.set_value(f"+{sw:.2f} s software")

  def _active_model_name(self) -> str:
    try:
      active = self.model_manager.activeBundle
      if active and active.ref:
        return active.internalName.lower()
    except Exception:
      pass
    return "default model"

  def _download_progress_text(self) -> str:
    try:
      parts = []
      for model in self.model_manager.selectedBundle.models:
        p = model.artifact.downloadProgress
        if p.status == _DL.downloading:
          parts.append(f"{int(p.progress)}%")
        elif p.status in (_DL.downloaded, _DL.cached):
          parts.append("ready")
        elif p.status == _DL.failed:
          parts.append("failed")
      return " ".join(parts) or "downloading..."
    except Exception:
      return "downloading..."

  def show_event(self):
    super().show_event()
    for w in (self._lagd, self._sw_delay, self._lane_turn, self._lane_speed):
      w.refresh()
