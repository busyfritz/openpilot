"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
import os

from openpilot.selfdrive.ui.layouts.settings.software import SoftwareLayout
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.hardware import HARDWARE
from openpilot.system.ui.lib.application import gui_app
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.widgets import DialogResult
from openpilot.system.ui.widgets.confirm_dialog import ConfirmDialog
from openpilot.system.ui.widgets.option_dialog import MultiOptionDialog

from openpilot.system.ui.iqpilot.widgets.list_view import button_item, toggle_item
from openpilot.system.ui.iqpilot.widgets.tree_dialog import TreeOptionDialog, TreeNode, TreeFolder


DESCRIPTIONS = {
  'disable_updates_offroad': tr_noop(
    "When enabled, automatic software updates will be off.<br><b>This requires a reboot to take effect.</b>"
  ),
  'disable_updates_onroad': tr_noop(
    "Please enable \"Always Offroad\" mode or turn off the vehicle to adjust these toggles."
  ),
  'install_mode_offroad': tr_noop(
    "Choose whether updates only download and wait for confirmation, or download and install automatically after they are ready."
  ),
  'install_mode_onroad': tr_noop(
    "Please enable \"Always Offroad\" mode or turn off the vehicle to adjust update install behavior."
  )
}

INSTALL_MODE_DOWNLOAD_ONLY = "download_only"
INSTALL_MODE_DOWNLOAD_AND_INSTALL = "download_and_install"
INSTALL_MODE_OPTIONS = [
  INSTALL_MODE_DOWNLOAD_ONLY,
  INSTALL_MODE_DOWNLOAD_AND_INSTALL,
]
INSTALL_MODE_LABELS = {
  INSTALL_MODE_DOWNLOAD_ONLY: tr_noop("Predownload Only"),
  INSTALL_MODE_DOWNLOAD_AND_INSTALL: tr_noop("Predownload + Preinstall"),
}


class IQSoftwareLayout(SoftwareLayout):
  def __init__(self):
    super().__init__()
    self.disable_updates_toggle = toggle_item(
      lambda: tr("Disable Updates"),
      description="",
      initial_state=ui_state.params.get_bool("DisableUpdates"),
      callback=self._on_disable_updates_toggled,
    )
    install_mode = ui_state.params.get("UpdaterInstallMode") or INSTALL_MODE_DOWNLOAD_AND_INSTALL
    if install_mode not in INSTALL_MODE_OPTIONS:
      install_mode = INSTALL_MODE_DOWNLOAD_AND_INSTALL
    self._install_mode_dialog = None
    self.install_mode_btn = button_item(
      lambda: tr("Update Install Mode"),
      lambda: tr("CHANGE"),
      description="",
      callback=self._on_change_install_mode,
    )
    self.install_mode_btn.action_item.set_value(tr(INSTALL_MODE_LABELS[install_mode]))
    self._scroller.add_widget(self.disable_updates_toggle)
    self._scroller.add_widget(self.install_mode_btn)

  def _handle_reboot(self, result):
    if result == DialogResult.CONFIRM:
      ui_state.params.put_bool("DisableUpdates", self.disable_updates_toggle.action_item.get_state())
      ui_state.params.put_bool("DoReboot", True)
    else:
      self.disable_updates_toggle.action_item.set_state(ui_state.params.get_bool("DisableUpdates"))

  def _on_disable_updates_toggled(self, enabled):
    dialog = ConfirmDialog(tr("System reboot required for changes to take effect. Reboot now?"), tr("Reboot"))
    gui_app.set_modal_overlay(dialog, callback=self._handle_reboot)

  def _on_change_install_mode(self):
    current_mode = ui_state.params.get("UpdaterInstallMode") or INSTALL_MODE_DOWNLOAD_AND_INSTALL
    if current_mode not in INSTALL_MODE_OPTIONS:
      current_mode = INSTALL_MODE_DOWNLOAD_AND_INSTALL

    labels = [tr(INSTALL_MODE_LABELS[mode]) for mode in INSTALL_MODE_OPTIONS]
    current_label = tr(INSTALL_MODE_LABELS[current_mode])
    self._install_mode_dialog = MultiOptionDialog(tr("Update Install Mode"), labels, current_label)

    def handle_selection(result):
      if result == DialogResult.CONFIRM and self._install_mode_dialog is not None and self._install_mode_dialog.selection:
        selected_label = self._install_mode_dialog.selection
        for mode in INSTALL_MODE_OPTIONS:
          if selected_label == tr(INSTALL_MODE_LABELS[mode]):
            ui_state.params.put("UpdaterInstallMode", mode)
            self.install_mode_btn.action_item.set_value(selected_label)
            break
      self._install_mode_dialog = None

    gui_app.set_modal_overlay(self._install_mode_dialog, callback=handle_selection)

  def _on_uninstall(self):
    def handle_uninstall_confirmation(result):
      if result == DialogResult.CONFIRM:
        ui_state.params.put_bool("DoUninstall", True)
        gui_app.request_close()

    dialog = ConfirmDialog(tr("Are you sure you want to uninstall?"), tr("Uninstall"))
    gui_app.set_modal_overlay(dialog, callback=handle_uninstall_confirmation)

  def _on_select_branch(self):
    current_git_branch = ui_state.params.get("GitBranch") or ""
    branches_str = ui_state.params.get("UpdaterAvailableBranches") or ""
    branches = [b for b in branches_str.split(",") if b]
    current_target = ui_state.params.get("UpdaterTargetBranch") or ""
    top_level_branches = [current_git_branch, "release-mici", "release-tizi", "staging", "dev", "master"]

    if HARDWARE.get_device_type() == "tici":
      top_level_branches = ["release-new", "release-tici", "staging-tici"]
      branches = [b for b in branches if b in ("release-new", "beta") or b.endswith("-tici")]

    top_level_nodes = [TreeNode(b, {'display_name': b}) for b in top_level_branches if b in branches]
    remaining_branches = [b for b in branches if b not in top_level_branches]
    prebuilt_nodes = [TreeNode(b, {'display_name': b}) for b in remaining_branches if b.endswith("-prebuilt")]
    non_prebuilt_nodes = [TreeNode(b, {'display_name': b}) for b in remaining_branches if not b.endswith("-prebuilt")]

    folders = [
      TreeFolder("", top_level_nodes),
      TreeFolder("Prebuilt Branches", prebuilt_nodes),
      TreeFolder("Non-Prebuilt Branches", non_prebuilt_nodes),
    ]

    def _on_branch_selected(result):
      if result == DialogResult.CONFIRM and self._branch_dialog is not None:
        selection = self._branch_dialog.selection_ref
        if selection:
          ui_state.params.put("UpdaterTargetBranch", selection)
          self._branch_btn.action_item.set_value(selection)
          os.system("pkill -SIGUSR1 -f system.updated.updated")
      self._branch_dialog = None

    self._branch_dialog = TreeOptionDialog(tr("Select a branch"), folders, current_target, "",
                                           on_exit=_on_branch_selected)

    gui_app.set_modal_overlay(self._branch_dialog, callback=_on_branch_selected)

  def _update_state(self):
    super()._update_state()
    self.disable_updates_toggle.action_item.set_enabled(ui_state.is_offroad())
    self.disable_updates_toggle.set_visible(True)
    self.install_mode_btn.action_item.set_enabled(ui_state.is_offroad())
    self.install_mode_btn.set_visible(True)

    disable_updates_desc = tr(DESCRIPTIONS["disable_updates_offroad"] if ui_state.is_offroad() else DESCRIPTIONS["disable_updates_onroad"])
    self.disable_updates_toggle.set_description(disable_updates_desc)
    install_mode_desc = tr(DESCRIPTIONS["install_mode_offroad"] if ui_state.is_offroad() else DESCRIPTIONS["install_mode_onroad"])
    self.install_mode_btn.set_description(install_mode_desc)

    install_mode = ui_state.params.get("UpdaterInstallMode") or INSTALL_MODE_DOWNLOAD_AND_INSTALL
    if install_mode not in INSTALL_MODE_OPTIONS:
      install_mode = INSTALL_MODE_DOWNLOAD_AND_INSTALL
    self.install_mode_btn.action_item.set_value(tr(INSTALL_MODE_LABELS[install_mode]))
