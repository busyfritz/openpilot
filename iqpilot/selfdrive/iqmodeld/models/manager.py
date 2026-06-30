#!/usr/bin/env python3
"""
Copyright (c) IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
from openpilot.iqpilot._proprietary_loader import load_private_module
from openpilot.common.swaglog import cloudlog

load_private_module(__name__, "iqpilot_private.models.manager")
from openpilot.iqpilot.selfdrive.iqmodeld.models.helpers import bundle_files_ready, get_active_bundle, get_runtime_bundle_upgrade, persist_active_bundle


_BaseIQModelManager = IQModelManager


class IQModelManager(_BaseIQModelManager):
  def _queue_tinygrad_upgrade(self) -> None:
    if self.active_bundle is None:
      return

    replacement = get_runtime_bundle_upgrade(self.active_bundle, self.params, self.available_models)
    if replacement is None or replacement is self.active_bundle:
      return

    if bundle_files_ready(replacement):
      persist_active_bundle(self.params, replacement)
      self.active_bundle = replacement
      return

    if self._download_index() is None and getattr(replacement, "index", None) is not None:
      self.params.put("ModelManager_DownloadIndex", int(replacement.index))
      cloudlog.warning(f"Queued tinygrad upgrade for retired bundle {getattr(self.active_bundle, 'internalName', '<unknown>')}")

  def main_thread(self) -> None:
    rk = Ratekeeper(1, print_delay_threshold=None)

    while True:
      try:
        self.available_models = self.model_fetcher.get_available_bundles()
        self.active_bundle = get_active_bundle(self.params)
        self._queue_tinygrad_upgrade()

        if not self.active_bundle and self._download_index() is None and self.available_models:
          default = get_default_model_bundle(self.available_models)
          if default:
            cloudlog.warning(f"No active model bundle, auto-selecting default model {default.internalName}")
            self.params.put("ModelManager_DownloadIndex", default.index)

        if (index_to_download := self._download_index()) is not None:
          if model_to_download := next((model for model in self.available_models if model.index == index_to_download), None):
            try:
              self.download(model_to_download, Paths.model_root())
            except Exception as e:
              cloudlog.exception(e)
            finally:
              self.params.remove("ModelManager_DownloadIndex")
              self.selected_bundle = None

        if self.params.get("ModelManager_ClearCache"):
          self.clear_model_cache()
          self.params.remove("ModelManager_ClearCache")

        self._report_status()
        rk.keep_time()

      except Exception as e:
        cloudlog.exception(f"Error in main thread: {str(e)}")
        rk.keep_time()


def main():
  IQModelManager().main_thread()

if __name__ == "__main__":
  main()
