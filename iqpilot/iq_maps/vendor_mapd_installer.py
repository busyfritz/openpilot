#!/usr/bin/env python3
import hashlib
"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
import logging
import os
import stat
import time
import traceback
import requests
from pathlib import Path
from urllib.request import urlopen

from cereal import messaging
from openpilot.common.params import Params
from openpilot.system.hardware.hw import Paths
from openpilot.common.spinner import Spinner
from openpilot.system.version import is_prebuilt
from openpilot.iqpilot.iq_maps import VENDOR_MAPD_BIN_DIR, VENDOR_MAPD_PATH
import openpilot.system.sentry as sentry

VENDOR_RELEASE_TAG = "v2.0.6"
VENDOR_RELEASE_URL = f"https://github.com/pfeiferj/mapd/releases/download/{VENDOR_RELEASE_TAG}/mapd"


def stamp_vendor_version(version: str, params: Params | None = None) -> None:
  if params is None:
    params = Params()

  params.put("MapdVersion", version)


def _expected_vendor_hash_path() -> str:
  from openpilot.common.basedir import BASEDIR
  return os.path.join(BASEDIR, "iqpilot", "iq_maps", "tests", "mapd_hash")


class VendorMapdInstaller:
  def __init__(self, spinner_ref: Spinner):
    self._spinner = spinner_ref
    self._params = Params()

  def fetch(self) -> None:
    self.ensure_directories_exist()
    self._download_file()
    stamp_vendor_version(VENDOR_RELEASE_TAG, self._params)

  def check_and_download(self) -> None:
    if self.download_needed():
      self.fetch()

  def download_needed(self) -> bool:
    if not os.path.exists(VENDOR_MAPD_PATH):
      return True
    if self.get_installed_version() != VENDOR_RELEASE_TAG:
      return True
    return not self._binary_hash_matches()

  @staticmethod
  def _binary_hash_matches() -> bool:
    try:
      hash_path = _expected_vendor_hash_path()
      with open(hash_path) as f:
        expected = f.read().strip()
    except Exception:
      return True
    if not expected:
      return True
    try:
      return get_file_hash(VENDOR_MAPD_PATH) == expected
    except Exception:
      return True

  @staticmethod
  def ensure_directories_exist() -> None:
    if not os.path.exists(Paths.mapd_root()):
      os.makedirs(Paths.mapd_root())
    if not os.path.exists(VENDOR_MAPD_BIN_DIR):
      os.makedirs(VENDOR_MAPD_BIN_DIR)

  @staticmethod
  def _safe_write_and_set_executable(file_path: Path, content: bytes) -> None:
    with open(file_path, 'wb') as output:
      output.write(content)
      output.flush()
      os.fsync(output.fileno())
    current_permissions = stat.S_IMODE(os.lstat(file_path).st_mode)
    os.chmod(file_path, current_permissions | stat.S_IEXEC)

  def _download_file(self, num_retries=5) -> None:
    temp_file = Path(VENDOR_MAPD_PATH + ".tmp")
    download_timeout = 60
    for cnt in range(num_retries):
      try:
        response = requests.get(VENDOR_RELEASE_URL, stream=True, timeout=download_timeout)
        response.raise_for_status()
        self._safe_write_and_set_executable(temp_file, response.content)
        temp_file.replace(VENDOR_MAPD_PATH)
        return
      except requests.exceptions.ReadTimeout:
        self._spinner.update(f"ReadTimeout caught. Timeout is [{download_timeout}]. Retrying download... [{cnt}]")
        time.sleep(0.5)
      except requests.exceptions.RequestException as e:
        self._spinner.update(f"RequestException caught: {e}. Retrying download... [{cnt}]")
        time.sleep(0.5)

    # Delete temp file if the process was not successful.
    if temp_file.exists():
      temp_file.unlink()
    logging.error("Failed to download file after all retries")

  def get_installed_version(self) -> str:
    return str(self._params.get("MapdVersion") or "")

  def wait_for_internet_connection(self, return_on_failure: bool = False) -> bool:
    max_retries = 10
    for retries in range(max_retries + 1):
      self._spinner.update(f"Waiting for internet connection... [{retries}/{max_retries}]")
      time.sleep(2)
      try:
        _ = urlopen('https://sentry.io', timeout=10)
        return True
      except Exception as e:
        print(f'Wait for internet failed: {e}')
        if return_on_failure and retries == max_retries:
          return False

    return False

  def non_prebuilt_install(self) -> None:
    sm = messaging.SubMaster(['deviceState'])
    metered = sm['deviceState'].networkMetered

    if metered:
      self._spinner.update("Can't proceed with mapd install since network is metered!")
      time.sleep(5)
      return

    try:
      self.ensure_directories_exist()
      if not self.download_needed():
        self._spinner.update("Offline maps binary is ready.")
        time.sleep(0.1)
        return

      if self.wait_for_internet_connection(return_on_failure=True):
        self._spinner.update(f"Downloading vendor mapd [{self.get_installed_version()}] => [{VENDOR_RELEASE_TAG}].")
        time.sleep(0.1)
        self.check_and_download()
      self._spinner.close()

    except Exception:
      for i in range(6):
        self._spinner.update("Failed to download OSM maps won't work until properly downloaded!" +
                             "Try again manually rebooting. " +
                             f"Boot will continue in {5 - i}s...")
        time.sleep(1)

      sentry.init(sentry.SentryProject.SELFDRIVE)
      traceback.print_exc()
      sentry.capture_exception()


if __name__ == "__main__":
  spinner = Spinner()
  install_manager = VendorMapdInstaller(spinner)
  install_manager.ensure_directories_exist()
  if is_prebuilt():
    debug_msg = f"[DEBUG] This is prebuilt, no vendor mapd install required. VERSION: [{VENDOR_RELEASE_TAG}], Param [{install_manager.get_installed_version()}]"
    spinner.update(debug_msg)
    stamp_vendor_version(VENDOR_RELEASE_TAG)
  else:
    spinner.update(f"Checking if vendor mapd is installed and valid. Prebuilt [{is_prebuilt()}]")
    install_manager.non_prebuilt_install()


def get_file_hash(path: str) -> str:
  """Hex SHA-256 of a file's contents."""
  with open(path, "rb") as handle:
    return hashlib.file_digest(handle, "sha256").hexdigest()
