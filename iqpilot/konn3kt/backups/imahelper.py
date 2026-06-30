"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""

from __future__ import annotations

import base64
import json
import os
from dataclasses import dataclass
from typing import Any

from cereal import custom
from openpilot.common.git import get_branch
from openpilot.common.params import Params, ParamKeyFlag
from openpilot.common.swaglog import cloudlog
from openpilot.system.version import get_version

from iqpilot.konn3kt.common.params import param_from_base64, param_to_bytes

CALIBRATION_KEYS = ["CalibrationParams", "LiveParametersV2", "LiveTorqueParameters"]
MODEL_SELECTION_KEY = "ModelManager_ActiveBundle"
PENDING_MODEL_RESTORE_FILE = "/data/k3_pending_model_restore"
RAW_FILE_PATHS = ["/cache/params/SecOCKey"]
RAW_FILE_PREFIX = "__file__:"


@dataclass(frozen=True)
class ImaSnapshot:
  entries: dict[str, str]


@dataclass(frozen=True)
class ImaRemoteRecord:
  metadata: dict[str, str]
  encrypted_config: str


@dataclass(frozen=True)
class ImaRestoreReport:
  restored_count: int
  skipped_count: int
  restored_calibration: bool
  restored_model: bool


class ImaHelper:
  def __init__(self, params: Params):
    self.params = params

  def capture_entries(self) -> ImaSnapshot:
    entries = self._encoded_params()
    entries.update(self._encoded_files())
    return ImaSnapshot(entries=entries)

  def decode_remote_metadata(self, metadata_list: list[dict[str, Any]]) -> dict[str, str]:
    return {
      str(entry.get("key", "")): str(entry.get("value", ""))
      for entry in metadata_list
      if entry.get("key")
    }

  def read_remote_record(self, payload: dict[str, Any]) -> ImaRemoteRecord:
    encrypted_config = payload.get("config", "")
    if not encrypted_config:
      raise Exception("Empty backup configuration")
    return ImaRemoteRecord(
      metadata=self.decode_remote_metadata(payload.get("backup_metadata", [])),
      encrypted_config=encrypted_config,
    )

  def build_backup_info(self, device_id: str, encrypted_config: str) -> custom.IQBackupManager.BackupInfo:
    backup_info = custom.IQBackupManager.BackupInfo()
    backup_info.deviceId = device_id
    backup_info.config = encrypted_config
    backup_info.isEncrypted = True
    backup_info.createdAt = backup_info.updatedAt = self._utc_timestamp()
    backup_info.iqpilotVersion = self.current_build_version()
    backup_info.backupMetadata = [
      custom.IQBackupManager.MetadataEntry(key="creator", value="BackupManagerK3"),
      custom.IQBackupManager.MetadataEntry(key="all_values_encoded", value="True"),
      custom.IQBackupManager.MetadataEntry(key="AES", value="256"),
      custom.IQBackupManager.MetadataEntry(key="schema", value="2"),
      custom.IQBackupManager.MetadataEntry(key="includes_calibration", value="True"),
      custom.IQBackupManager.MetadataEntry(key="includes_model", value="True"),
    ]
    return backup_info

  def absorb_entries(self, config_data: dict[str, str]) -> ImaRestoreReport:
    known_keys = {k.decode("utf-8") for k in self.params.all_keys(ParamKeyFlag.BACKUP)}
    known_keys |= set(CALIBRATION_KEYS) | {MODEL_SELECTION_KEY}
    lowercase_lookup = {name.lower(): name for name in known_keys}

    restored_count = 0
    skipped_count = 0
    restored_calibration = False
    restored_model = False
    deferred_model_bundle = None

    for key_name, encoded_value in config_data.items():
      if key_name.startswith(RAW_FILE_PREFIX):
        if self._restore_file_blob(key_name, encoded_value):
          restored_count += 1
        else:
          skipped_count += 1
        continue

      canonical_name = lowercase_lookup.get(key_name.lower())
      if canonical_name is None:
        skipped_count += 1
        cloudlog.info(f"[K3] Skipped restoring param {key_name}: not restorable in current version")
        continue

      if canonical_name == MODEL_SELECTION_KEY:
        deferred_model_bundle = encoded_value
        continue

      try:
        param_from_base64(canonical_name, encoded_value)
        restored_count += 1
        restored_calibration = restored_calibration or canonical_name in CALIBRATION_KEYS
      except Exception as e:
        cloudlog.error(f"[K3] Failed to restore param {key_name}: {str(e)}")

    if deferred_model_bundle:
      restored_model = self.queue_model_restore(deferred_model_bundle)

    return ImaRestoreReport(
      restored_count=restored_count,
      skipped_count=skipped_count,
      restored_calibration=restored_calibration,
      restored_model=restored_model,
    )

  def queue_model_restore(self, encoded_bundle_b64: str) -> bool:
    try:
      raw = base64.b64decode(encoded_bundle_b64)
      bundle = json.loads(raw.decode("utf-8"))
    except Exception as e:
      cloudlog.error(f"[K3] Could not parse saved model bundle: {e}")
      return False

    ref = (bundle.get("ref") or bundle.get("internalName") or bundle.get("displayName") or "").strip()
    if not ref:
      cloudlog.info("[K3] No model selection in backup (stock model)")
      return False

    try:
      with open(PENDING_MODEL_RESTORE_FILE, "w", encoding="utf-8") as f:
        f.write(ref)
      cloudlog.info(f"[K3] Queued model '{ref}' for re-download/activation after reboot")
      return True
    except Exception as e:
      cloudlog.error(f"[K3] Failed to queue model selection '{ref}': {e}")
      return False

  def revive_pending_model_download(self) -> None:
    try:
      if not os.path.exists(PENDING_MODEL_RESTORE_FILE):
        return

      with open(PENDING_MODEL_RESTORE_FILE, encoding="utf-8") as f:
        ref = f.read().strip()
      os.remove(PENDING_MODEL_RESTORE_FILE)
      if not ref:
        return

      from openpilot.iqpilot.selfdrive.iqmodeld.models.fetcher import ModelFetcher  # pylint: disable=import-error
      available = ModelFetcher(self.params).get_available_bundles()
      match = next((bundle for bundle in available if (getattr(bundle, "ref", None) or "") == ref), None)
      if match is None:
        cloudlog.warning(f"[K3] Restored model '{ref}' is no longer offered; keeping stock model")
        return

      self.params.put("ModelManager_DownloadIndex", int(getattr(match, "index")))
      cloudlog.info(f"[K3] Re-triggered restored model '{ref}' download after reboot")
    except Exception as e:
      cloudlog.error(f"[K3] Pending model restore failed: {e}")

  @staticmethod
  def current_build_version() -> custom.IQBackupManager.Version:
    version_obj = custom.IQBackupManager.Version()
    version_str = get_version()

    version_parts = version_str.split("-")
    version_nums = version_parts[0].split(".")

    build = 0
    if len(version_parts) > 1 and version_parts[1].isdigit():
      build = int(version_parts[1])
    elif len(version_nums) > 3 and version_nums[3].isdigit():
      build = int(version_nums[3])

    version_obj.major = int(version_nums[0]) if len(version_nums) > 0 and version_nums[0].isdigit() else 0
    version_obj.minor = int(version_nums[1]) if len(version_nums) > 1 and version_nums[1].isdigit() else 0
    version_obj.patch = int(version_nums[2]) if len(version_nums) > 2 and version_nums[2].isdigit() else 0
    version_obj.build = build
    version_obj.branch = get_branch()
    return version_obj

  def _encoded_params(self) -> dict[str, str]:
    encoded: dict[str, str] = {}
    for param_name in self._backup_param_names():
      raw_value = param_to_bytes(param_name)
      if raw_value is not None:
        encoded[param_name] = base64.b64encode(raw_value).decode("utf-8")
    return encoded

  def _encoded_files(self) -> dict[str, str]:
    encoded: dict[str, str] = {}
    for path in RAW_FILE_PATHS:
      try:
        with open(path, "rb") as f:
          encoded[RAW_FILE_PREFIX + path] = base64.b64encode(f.read()).decode("utf-8")
      except Exception:
        pass
    return encoded

  def _backup_param_names(self) -> list[str]:
    names = [k.decode("utf-8") for k in self.params.all_keys(ParamKeyFlag.BACKUP)]
    for extra in CALIBRATION_KEYS + [MODEL_SELECTION_KEY]:
      if extra not in names:
        names.append(extra)
    return names

  def _restore_file_blob(self, key_name: str, encoded_value: str) -> bool:
    path = key_name[len(RAW_FILE_PREFIX):]
    if path not in RAW_FILE_PATHS:
      return False
    try:
      os.makedirs(os.path.dirname(path), exist_ok=True)
      with open(path, "wb") as f:
        f.write(base64.b64decode(encoded_value))
      return True
    except Exception as e:
      cloudlog.error(f"[K3] Failed to restore file {path}: {str(e)}")
      return False

  @staticmethod
  def _utc_timestamp() -> str:
    from datetime import datetime
    return datetime.utcnow().strftime("%Y-%m-%d %H:%M:%S")
