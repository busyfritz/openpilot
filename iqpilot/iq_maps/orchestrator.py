#!/usr/bin/env python3
"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""
import platform
import os
import glob
import shutil
import subprocess
import threading
import time
from datetime import datetime

import cereal.messaging as messaging
from cereal import custom
from openpilot.common.params import Params
from openpilot.common.realtime import Ratekeeper, config_realtime_process
from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.selfdrived.alertmanager import set_offroad_alert
from openpilot.system.hardware.hw import Paths
from openpilot.iqpilot.iq_maps import VENDOR_MAPD_BIN_DIR, VENDOR_MAPD_PATH
from openpilot.iqpilot.iq_maps.vendor_mapd_installer import VendorMapdInstaller

OfflineMapAction = custom.MapdInputType
_region_sync_worker: threading.Thread | None = None


class _QuietSpinner:
  def update(self, *args, **kwargs) -> None:
    pass

  def close(self, *args, **kwargs) -> None:
    pass


def ensure_vendor_runtime() -> None:
  try:
    VendorMapdInstaller(_QuietSpinner()).check_and_download()
  except Exception:
    cloudlog.exception("iq_maps: vendor runtime install/download failed")

params = Params()
mem_params = Params("/dev/shm/params") if platform.system() != "Darwin" else params


def stale_region_artifacts() -> list[str]:
  patterns = [
    f"{Paths.mapd_root()}/db",
    f"{Paths.mapd_root()}/v*"
  ]
  stale_paths: list[str] = []
  for pattern in patterns:
    for match in glob.glob(pattern):
      stale_paths.append(match)
      if os.path.isdir(match):
        stale_paths.extend(glob.glob(match + '/**', recursive=True))
  if not os.path.isfile(VENDOR_MAPD_PATH):
    stale_paths.append(VENDOR_MAPD_PATH)
  return stale_paths


def purge_stale_region_artifacts(stale_paths: list[str]) -> None:
  for candidate in stale_paths:
    if candidate.endswith('/') and os.path.isfile(candidate[:-1]):
      candidate = candidate[:-1]
    if os.path.islink(candidate) or os.path.isfile(candidate):
      os.remove(candidate)
    elif os.path.isdir(candidate):
      shutil.rmtree(candidate, ignore_errors=False)


def _compose_region_selector(nations: list[str], states: list[str] | None = None) -> str:
  requested_paths: list[str] = []
  for state_code in (states or []):
    code = str(state_code).strip().upper()
    if code and code != "ALL":
      requested_paths.append(f"us_state.{code}")
  for nation_code in (nations or []):
    code = str(nation_code).strip().upper()
    if code:
      requested_paths.append(f"nation.{code}")
  return ",".join(requested_paths)


def _drive_vendor_fetch(region_selector: str, requested_regions: dict) -> None:
  proc = None
  try:
    mem_params.put("OSMDownloadLocations", requested_regions)
    proc = subprocess.Popen([VENDOR_MAPD_PATH], cwd=VENDOR_MAPD_BIN_DIR,
                            stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    pm = messaging.PubMaster(["mapdIn"])
    sm = messaging.SubMaster(["mapdExtendedOut"])
    time.sleep(4.0)

    for _ in range(10):
      msg = messaging.new_message("mapdIn")
      msg.mapdIn.type = OfflineMapAction.download
      msg.mapdIn.str = region_selector
      pm.send("mapdIn", msg)
      time.sleep(0.2)

    started = False
    deadline = time.monotonic() + 3600.0
    while time.monotonic() < deadline:
      sm.update(500)
      dp = sm["mapdExtendedOut"].downloadProgress
      mem_params.put("OSMDownloadProgress", {
        "active": bool(dp.active),
        "total_files": int(dp.totalFiles),
        "downloaded_files": int(dp.downloadedFiles),
      })
      if dp.active:
        started = True
      elif started:
        break
      if not mem_params.get("OSMDownloadLocations"):
        cancel = messaging.new_message("mapdIn")
        cancel.mapdIn.type = OfflineMapAction.cancelDownload
        pm.send("mapdIn", cancel)
        break
    cloudlog.info(f"iq_maps: vendor map download finished for {region_selector}")
  except Exception:
    cloudlog.exception("iq_maps: vendor map download failed")
  finally:
    try:
      mem_params.remove("OSMDownloadLocations")
    except Exception:
      pass
    if proc is not None:
      proc.terminate()
      try:
        proc.wait(timeout=5)
      except Exception:
        proc.kill()


def queue_region_refresh(nations: list[str], states: list[str] | None = None) -> None:
  global _region_sync_worker
  params.put("OsmDownloadedDate", str(datetime.now().timestamp()))
  params.put_bool("OsmDbUpdatesCheck", False)

  region_selector = _compose_region_selector(nations, states)
  if not region_selector:
    cloudlog.warning("iq_maps: no region selected for offline map download")
    return
  if _region_sync_worker is not None and _region_sync_worker.is_alive():
    cloudlog.warning("iq_maps: vendor map download already in progress")
    return

  requested_regions = {"nations": nations, "states": states or [], "paths": region_selector}
  cloudlog.info(f"iq_maps: starting vendor map download for {region_selector}")
  _region_sync_worker = threading.Thread(
    target=_drive_vendor_fetch,
    args=(region_selector, requested_regions),
    daemon=True,
  )
  _region_sync_worker.start()


def normalize_region_selection(nations: list[str], states: list[str] | None = None) -> tuple[list[str], list[str]]:
  normalized_nations = list(nations)
  normalized_states = list(states or [])
  lowered_states = {entry.lower() for entry in normalized_states}

  if "US" in normalized_nations and normalized_states and "all" not in lowered_states:
    normalized_nations = [entry for entry in normalized_nations if entry != "US"]
  elif normalized_states:
    normalized_states = [entry for entry in normalized_states if entry.lower() != "all"]

  return normalized_nations, normalized_states


def sync_osm_request_flags() -> None:
  if params.get_bool("OsmDbUpdatesCheck"):
    purge_stale_region_artifacts(stale_region_artifacts())
    country = params.get("OsmLocationName", return_default=True)
    state = params.get("OsmStateName", return_default=True)
    filtered_nations, filtered_states = normalize_region_selection([country], [state])
    queue_region_refresh(filtered_nations, filtered_states)

  if not mem_params.get("OSMDownloadBounds"):
    mem_params.put("OSMDownloadBounds", "")

  if not mem_params.get("LastGPSPosition"):
    mem_params.put("LastGPSPosition", "{}")


def run_loop():
  ensure_vendor_runtime()
  config_realtime_process([0, 1, 2, 3], 5)

  rk = Ratekeeper(1, print_delay_threshold=None)

  try:
    os.mkdir(Paths.mapd_root())
  except FileExistsError:
    pass
  except PermissionError:
    cloudlog.exception(f"iq_maps: failed to make {Paths.mapd_root()}")

  while True:
    show_alert = stale_region_artifacts() and params.get_bool("OsmLocal")
    set_offroad_alert("Offroad_OSMUpdateRequired", show_alert, "This alert will be cleared when new maps are downloaded.")

    sync_osm_request_flags()
    rk.keep_time()


def main():
  run_loop()


if __name__ == "__main__":
  main()
