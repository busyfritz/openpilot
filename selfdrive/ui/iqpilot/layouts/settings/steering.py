"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
from cereal import car
from enum import IntEnum

from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.iqpilot.widgets.list_view import toggle_item, simple_button_item, IQLineSeparator
from openpilot.system.ui.widgets.scroller_tici import Scroller
from openpilot.system.ui.widgets import Widget
from .steering_sub_layouts.lane_change_settings import LaneChangeSettingsLayout
from .steering_sub_layouts.sab_settings import SabSettingsLayout
from .steering_sub_layouts.lane_lines_settings import LaneLinesSettingsLayout


class PanelType(IntEnum):
  STEERING = 0
  SAB = 1
  LANE_CHANGE = 2
  LANE_LINES = 3


class SteeringLayout(Widget):
  def __init__(self):
    super().__init__()

    self._current_panel = PanelType.STEERING
    self._lane_change_settings_layout = LaneChangeSettingsLayout(lambda: self._set_current_panel(PanelType.STEERING))
    self._sab_settings_layout = SabSettingsLayout(lambda: self._set_current_panel(PanelType.STEERING))
    self._lane_lines_layout = LaneLinesSettingsLayout(lambda: self._set_current_panel(PanelType.STEERING))

    items = self._initialize_items()
    self._scroller = Scroller(items, line_separator=False, spacing=0)

  def _initialize_items(self):
    self._aol_base_desc = tr("Enable Always on Lateral (AOL). Disable this toggle to return to stock IQ.Pilot steering engagement behavior.")
    self._sab_limited_desc = tr("This platform supports a limited set of steering assistance behavior options.")
    self._sab_full_desc = tr("This platform supports the full steering assistance behavior configuration.")
    self._sab_check_compat_desc = tr("Start the vehicle to check steering assistance behavior compatibility.")

    self._aol_toggle = toggle_item(
      param="AolEnabled",
      title=lambda: tr("Always on Lateral (AOL)"),
      description=self._aol_base_desc,
    )
    self._aol_settings_button = simple_button_item(
      button_text=lambda: tr("Steering Assistance Behavior"),
      button_width=800,
      callback=lambda: self._set_current_panel(PanelType.SAB)
    )
    self._lane_change_settings_button = simple_button_item(
      button_text=lambda: tr("Customize Lane Change"),
      button_width=800,
      callback=lambda: self._set_current_panel(PanelType.LANE_CHANGE)
    )
    self._nnff_toggle = toggle_item(
      param="NeuralNetworkFeedForward",
      title=lambda: tr("Neural Network Feed Forward (NNFF)"),
      description=""
    )
    self._lane_lines_button = simple_button_item(
      button_text=lambda: tr("Lane Planner"),
      button_width=800,
      callback=lambda: self._set_current_panel(PanelType.LANE_LINES)
    )

    items = [
      self._aol_toggle,
      self._aol_settings_button,
      IQLineSeparator(40),
      self._lane_change_settings_button,
      IQLineSeparator(40),
      self._nnff_toggle,
      IQLineSeparator(40),
      self._lane_lines_button,
    ]
    return items

  def _set_current_panel(self, panel: PanelType):
    self._current_panel = panel

  def _update_state(self):
    super()._update_state()

    steering_supported = True
    if ui_state.CP is not None:
      sab_desc = self._sab_limited_desc if self._sab_settings_layout._has_limited_sab_options() else self._sab_full_desc
      self._aol_toggle.set_description(f"<b>{sab_desc}</b><br><br>{self._aol_base_desc}")

      if ui_state.CP.steerControlType == car.CarParams.SteerControlType.angle:
        ui_state.params.remove("NeuralNetworkFeedForward")
        steering_supported = False
    else:
      self._aol_toggle.set_description(f"<b>{self._sab_check_compat_desc}</b><br><br>{self._aol_base_desc}")
      ui_state.params.remove("NeuralNetworkFeedForward")
      steering_supported = False

    self._aol_toggle.action_item.set_enabled(ui_state.is_offroad())
    self._aol_settings_button.action_item.set_enabled(ui_state.is_offroad() and self._aol_toggle.action_item.get_state())
    self._nnff_toggle.action_item.set_enabled(ui_state.is_offroad() and steering_supported)

  def _render(self, rect):
    if self._current_panel == PanelType.LANE_CHANGE:
      self._lane_change_settings_layout.render(rect)
    elif self._current_panel == PanelType.SAB:
      self._sab_settings_layout.render(rect)
    elif self._current_panel == PanelType.LANE_LINES:
      self._lane_lines_layout.render(rect)
    else:
      self._scroller.render(rect)

  def show_event(self):
    self._set_current_panel(PanelType.STEERING)
    self._scroller.show_event()
