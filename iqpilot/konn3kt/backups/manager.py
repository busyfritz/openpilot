"""
Copyright ©️ IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos
"""

import json
import time
from enum import Enum
from typing import Any
from datetime import datetime

from openpilot.common.params import Params
from openpilot.common.realtime import Ratekeeper
from openpilot.common.swaglog import cloudlog
from openpilot.system.hardware import HARDWARE

from cereal import messaging, custom
from iqpilot.konn3kt.api import Konn3ktApi
from iqpilot.konn3kt.backups.imahelper import ImaHelper, ImaRemoteRecord
from iqpilot.konn3kt.backups.utils import decrypt_compressed_data, encrypt_compress_data, SnakeCaseEncoder

K3_LOG_FILE = "/data/openpilot/k3_log.txt"

def k3_log(msg: str):
  try:
    with open(K3_LOG_FILE, "a") as f:
      timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
      f.write(f"[{timestamp}] {msg}\n")
      f.flush()
  except Exception as e:
    cloudlog.error(f"[K3] Failed to write to debug log: {e}")

k3_log("=== K3 Backup Manager Module Loaded ===")


class OperationType(Enum):
  BACKUP = "backup"
  RESTORE = "restore"


class BackupManagerK3:

  def __init__(self):
    k3_log("BackupManagerK3 initializing...")
    self.params = Params()
    self.device_id = self.params.get("DongleId")
    k3_log(f"Device ID: {self.device_id}")
    self.api = Konn3ktApi(self.device_id)
    self.imahelper = ImaHelper(self.params)
    self.pm = messaging.PubMaster(["backupManagerK3"])

    self.backup_status = custom.IQBackupManager.Status.idle
    self.restore_status = custom.IQBackupManager.Status.idle

    self.progress = 0.0
    self.operation: OperationType | None = None

    self.last_error = ""
    k3_log("BackupManagerK3 initialized successfully")

  def _report_status(self) -> None:
    msg = messaging.new_message('backupManagerK3', valid=True)
    backup_state = msg.backupManagerK3

    backup_state.backupStatus = self.backup_status
    backup_state.restoreStatus = self.restore_status
    backup_state.backupProgress = self.progress
    backup_state.restoreProgress = self.progress
    backup_state.lastError = self.last_error

    self.pm.send('backupManagerK3', msg)

  def _update_progress(self, progress: float, op_type: OperationType) -> None:
    self.progress = progress
    self.operation = op_type
    self._report_status()

  def _build_backup_info(self, encrypted_config: str) -> custom.IQBackupManager.BackupInfo:
    return self.imahelper.build_backup_info(self.device_id, encrypted_config)

  @staticmethod
  def _serialize_backup_info(backup_info: custom.IQBackupManager.BackupInfo) -> dict[str, Any]:
    return json.loads(json.dumps(backup_info.to_dict(), cls=SnakeCaseEncoder))

  def _fetch_remote_record(self, version: int | None) -> ImaRemoteRecord:
    endpoint = f"api/v2/backup/{self.device_id}" + (f"/{version}" if version else "")
    k3_log(f"Fetching backup from endpoint: {endpoint}")
    backup_response = self.api.api_get(endpoint, access_token=self.api.get_token())
    k3_log(f"API response: {backup_response}")
    if not backup_response:
      raise Exception(f"No backup found for device {self.device_id}")

    data = backup_response.json()
    k3_log(f"Parsed JSON data, keys: {data.keys() if data else None}")
    return self.imahelper.read_remote_record(data)

  async def create_backup(self) -> bool:
    k3_log("create_backup() called")
    try:
      k3_log("Setting status to inProgress")
      self.backup_status = custom.IQBackupManager.Status.inProgress
      self._update_progress(0.0, OperationType.BACKUP)

      snapshot = self.imahelper.capture_entries()
      k3_log(f"Collected {len(snapshot.entries)} backup entries")
      self._update_progress(25.0, OperationType.BACKUP)

      encrypted_config = encrypt_compress_data(json.dumps(snapshot.entries), use_aes_256=True)
      k3_log(f"Encrypted config length: {len(encrypted_config)}")
      self._update_progress(50.0, OperationType.BACKUP)

      payload = self._serialize_backup_info(self._build_backup_info(encrypted_config))
      self._update_progress(75.0, OperationType.BACKUP)

      k3_log(f"Uploading backup to api/v2/backup/{self.device_id}")
      cloudlog.debug(f"[K3] Uploading backup with payload: {json.dumps(payload)}")
      result = self.api.api_get(
        f"api/v2/backup/{self.device_id}",
        method='PUT',
        access_token=self.api.get_token(),
        json=payload
      )
      k3_log(f"API result: {result}")

      if result:
        k3_log("Backup upload successful!")
        self.backup_status = custom.IQBackupManager.Status.completed
        self._update_progress(100.0, OperationType.BACKUP)
        cloudlog.info("[K3] Backup successfully created and uploaded")
      else:
        k3_log(f"Backup upload failed: {result}")
        self.backup_status = custom.IQBackupManager.Status.failed
        self.last_error = "Failed to upload backup"
        cloudlog.error(f"[K3] {result}")
        self._report_status()

      return bool(self.backup_status == custom.IQBackupManager.Status.completed)

    except Exception as e:
      k3_log(f"Exception in create_backup: {type(e).__name__}: {str(e)}")
      import traceback
      k3_log(f"Traceback: {traceback.format_exc()}")
      cloudlog.exception(f"[K3] Error creating backup: {str(e)}")
      self.backup_status = custom.IQBackupManager.Status.failed
      self.last_error = str(e)
      self._report_status()
      return False

  async def restore_backup(self, version: int | None = None) -> bool:
    k3_log(f"restore_backup() called with version={version}")
    try:
      k3_log("Setting restore status to inProgress")
      self.restore_status = custom.IQBackupManager.Status.inProgress
      self._update_progress(0.0, OperationType.RESTORE)

      remote_record = self._fetch_remote_record(version)
      self._update_progress(25.0, OperationType.RESTORE)

      k3_log(f"Encrypted config length: {len(remote_record.encrypted_config)}")
      self._update_progress(50.0, OperationType.RESTORE)

      use_aes_256 = remote_record.metadata.get("AES", "128") == "256"
      k3_log(f"Using AES-256: {use_aes_256}")
      config_json = decrypt_compressed_data(remote_record.encrypted_config, use_aes_256)
      k3_log(f"Decrypted config length: {len(config_json) if config_json else 0}")
      if not config_json:
        raise Exception("Failed to decrypt backup configuration")

      config_data = json.loads(config_json)
      k3_log(f"Parsed config data, {len(config_data)} params")
      self._update_progress(75.0, OperationType.RESTORE)

      k3_log("Applying configuration...")
      restore_report = self.imahelper.absorb_entries(config_data)
      cloudlog.info(
        f"[K3] Restore complete: {restore_report.restored_count} restored, "
        f"{restore_report.skipped_count} skipped, "
        f"calibration={restore_report.restored_calibration}, model={restore_report.restored_model}"
      )

      k3_log("Restore completed successfully!")
      self.restore_status = custom.IQBackupManager.Status.completed
      self._update_progress(100.0, OperationType.RESTORE)
      cloudlog.info("[K3] Backup successfully restored")
      # Settings + calibration + SecOC + model only fully take effect from a clean boot.
      self._reboot_after_restore()
      return True

    except Exception as e:
      k3_log(f"Exception in restore_backup: {type(e).__name__}: {str(e)}")
      import traceback
      k3_log(f"Traceback: {traceback.format_exc()}")
      cloudlog.exception(f"[K3] Error restoring backup: {str(e)}")
      self.restore_status = custom.IQBackupManager.Status.failed
      self.last_error = str(e)
      self._report_status()
      return False

  def _process_pending_model_restore(self) -> None:
    self.imahelper.revive_pending_model_download()

  def _reboot_after_restore(self) -> None:
    # Reboot so calibrationd/paramsd/torqued seed from the restored values at init instead of
    # overwriting them mid-session, and so SecOC / model / every param applies from a clean start.
    try:
      time.sleep(3)  # let the app read the 'completed' status before the device goes down
      k3_log("Rebooting device to finish restore")
      cloudlog.info("[K3] Rebooting to finish restore")
      HARDWARE.reboot()
    except Exception as e:
      cloudlog.error(f"[K3] Reboot after restore failed: {e}")

  async def main_thread(self) -> None:
    k3_log("main_thread() starting")
    # If a prior restore queued a model selection, re-trigger its download now (post-reboot).
    self._process_pending_model_restore()
    rk = Ratekeeper(1, print_delay_threshold=None)
    reset_progress = False

    k3_log("Entering main loop")
    while True:
      try:
        if reset_progress:
          self.progress = 100.0
          self.operation = None
          self.restore_status = custom.IQBackupManager.Status.idle
          self.backup_status = custom.IQBackupManager.Status.idle

        create_backup_param = self.params.get_bool("BackupManagerK3_CreateBackup")
        if create_backup_param:
          k3_log(f"Detected BackupManagerK3_CreateBackup = {create_backup_param}")
          try:
            if await self.create_backup():
              k3_log("create_backup() returned True")
              reset_progress = True
            else:
              k3_log("create_backup() returned False")
          finally:
            k3_log("Removing BackupManagerK3_CreateBackup param")
            self.params.remove("BackupManagerK3_CreateBackup")

        restore_version = self.params.get("BackupManagerK3_RestoreVersion")
        if restore_version:
          k3_log(f"Detected BackupManagerK3_RestoreVersion = {restore_version}")
          k3_log(f"Type: {type(restore_version)}, Value: {repr(restore_version)}")
          try:

            version_str = restore_version.decode('utf-8') if isinstance(restore_version, bytes) else restore_version
            k3_log(f"Converted version string: {version_str}")
            version = int(version_str) if version_str.isdigit() else None
            k3_log(f"Parsed version number: {version}")
            k3_log("Calling restore_backup()...")
            result = await self.restore_backup(version)
            k3_log(f"restore_backup() returned: {result}")
            reset_progress = True
          except Exception as e:
            k3_log(f"Exception during restore: {type(e).__name__}: {str(e)}")
            import traceback
            k3_log(f"Traceback: {traceback.format_exc()}")
          finally:
            k3_log("Removing BackupManagerK3_RestoreVersion param")
            self.params.remove("BackupManagerK3_RestoreVersion")

        self._report_status()
        rk.keep_time()

      except Exception as e:
        k3_log(f"Exception in main_thread: {type(e).__name__}: {str(e)}")
        import traceback
        k3_log(f"Traceback: {traceback.format_exc()}")
        cloudlog.exception(f"[K3] Error in backup manager main thread: {str(e)}")
        self.last_error = str(e)
        self._report_status()
        rk.keep_time()


def main():
  k3_log("main() function called, starting BackupManagerK3")
  import asyncio
  asyncio.run(BackupManagerK3().main_thread())


if __name__ == "__main__":
  main()
