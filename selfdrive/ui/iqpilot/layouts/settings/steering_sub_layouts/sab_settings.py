"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
from collections.abc import Callable
import pyray as rl

from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.widgets import Widget
from openpilot.system.ui.widgets.network import NavButton
from openpilot.system.ui.widgets.scroller_tici import Scroller
from openpilot.system.ui.iqpilot.widgets.list_view import multiple_button_item, toggle_item

SAB_BRAKE_RESPONSE_OPTIONS = [
  (tr("Remain Active"), tr_noop("Remain Active: steering assistance stays active when the brake pedal is pressed.")),
  (tr("Standby"), tr_noop("Standby: steering assistance remains armed and resumes when brake intervention ends.")),
  (tr("Disengage"), tr_noop("Disengage: steering assistance turns off when the brake pedal is pressed.")),
]

SAB_AVAILABILITY_DESC = tr("Note: For vehicles without an LFA/LKAS button, disabling this will prevent steering assistance from arming when cruise availability changes.")
SAB_DRIVER_INTERVENTION_DESC = tr("Choose how steering assistance responds when the driver presses the brake pedal.")

STATUS_CHECK_COMPATIBILITY = tr("Start the vehicle to check vehicle compatibility.")
DEFAULT_TO_OFF = tr("This feature defaults to OFF, and does not allow selection due to vehicle limitations.")
STATUS_DISENGAGE_ONLY = tr("This platform only supports Disengage mode due to vehicle limitations.")


class SabSettingsLayout(Widget):
  def __init__(self, back_btn_callback: Callable):
    super().__init__()
    self._back_button = NavButton(tr("Back"))
    self._back_button.set_click_callback(back_btn_callback)
    self._initialize_items()
    self._scroller = Scroller(self.items, line_separator=True, spacing=0)

  def _initialize_items(self):
    self._main_cruise_toggle = toggle_item(
      title=lambda: tr("Availability While Cruise Changes"),
      description=SAB_AVAILABILITY_DESC,
      param="AolMainCruiseAllowed",
    )
    self._disengage_on_brake_toggle = toggle_item(
      title=lambda: tr("Driver Intervention Handling"),
      description=SAB_DRIVER_INTERVENTION_DESC,
      initial_state=False,
      callback=self._set_disengage_on_brake,
    )
    self._steering_mode = multiple_button_item(
      param="AolSteeringMode",
      title=lambda: tr("Brake Response Mode"),
      description="",
      buttons=[opt[0] for opt in SAB_BRAKE_RESPONSE_OPTIONS],
      inline=False,
      button_width=350,
      callback=self._update_steering_mode_description,
    )

    self.items = [
      self._main_cruise_toggle,
      self._disengage_on_brake_toggle,
      self._steering_mode,
    ]

  def _update_state(self):
    super()._update_state()
    self._update_toggles()

  def _render(self, rect):
    self._back_button.set_position(self._rect.x, self._rect.y + 20)
    self._back_button.render()
    content_rect = rl.Rectangle(rect.x, rect.y + self._back_button.rect.height + 40, rect.width, rect.height - self._back_button.rect.height - 40)
    self._scroller.render(content_rect)

  def show_event(self):
    self._scroller.show_event()

  @staticmethod
  def _has_limited_sab_options() -> bool:
    brand = ""
    if ui_state.is_offroad():
      bundle = ui_state.params.get("CarPlatformBundle")
      if bundle:
        brand = bundle.get("brand", "")
    if not brand:
      brand = ui_state.CP.brand if ui_state.CP else ""

    return brand == "rivian"

  def _set_disengage_on_brake(self, enabled: bool):
    current_mode = int(ui_state.params.get("AolSteeringMode", return_default=True))
    if enabled:
      ui_state.params.put("AolSteeringMode", 2)
      self._steering_mode.action_item.set_selected_button(2)
    elif current_mode == 2:
      ui_state.params.put("AolSteeringMode", 1)
      self._steering_mode.action_item.set_selected_button(1)

  def _update_steering_mode_description(self, button_index: int):
    base_desc = tr("Choose how steering assistance behaves after the driver presses the brake pedal in IQ.Pilot.")
    result = base_desc + "<br><br>"
    for opt in SAB_BRAKE_RESPONSE_OPTIONS:
      desc = "<b>" + opt[1] + "</b>" if button_index == SAB_BRAKE_RESPONSE_OPTIONS.index(opt) else opt[1]
      result += desc + "<br>"
    self._steering_mode.set_description(result)
    self._steering_mode.show_description(True)

  def _update_toggles(self):
    if ui_state.params.get_bool("AolEnabled"):
      ui_state.params.put_bool("AolUnifiedEngagementMode", True)

    current_mode = int(ui_state.params.get("AolSteeringMode", return_default=True))
    self._disengage_on_brake_toggle.action_item.set_state(current_mode == 2)
    self._update_steering_mode_description(self._steering_mode.action_item.get_selected_button())
    if self._has_limited_sab_options():
      ui_state.params.remove("AolMainCruiseAllowed")
      ui_state.params.put_bool("AolUnifiedEngagementMode", True)
      ui_state.params.put("AolSteeringMode", 2)

      self._main_cruise_toggle.action_item.set_enabled(False)
      self._main_cruise_toggle.action_item.set_state(False)
      self._main_cruise_toggle.set_description("<b>" + DEFAULT_TO_OFF + "</b><br>" + SAB_AVAILABILITY_DESC)

      self._disengage_on_brake_toggle.action_item.set_enabled(False)
      self._disengage_on_brake_toggle.action_item.set_state(True)
      self._disengage_on_brake_toggle.set_description(STATUS_DISENGAGE_ONLY)

      self._steering_mode.action_item.set_enabled(False)
      self._steering_mode.set_description(STATUS_DISENGAGE_ONLY)
      self._steering_mode.action_item.set_selected_button(2)
    else:
      self._main_cruise_toggle.action_item.set_enabled(True)
      self._main_cruise_toggle.set_description(SAB_AVAILABILITY_DESC)

      self._disengage_on_brake_toggle.action_item.set_enabled(True)
      self._disengage_on_brake_toggle.set_description(SAB_DRIVER_INTERVENTION_DESC)

      self._steering_mode.action_item.set_enabled(True)
