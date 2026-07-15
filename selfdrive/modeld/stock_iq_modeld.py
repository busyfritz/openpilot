"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""

from __future__ import annotations

import os
import pickle
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import cereal.messaging as messaging
import numpy as np

from cereal import car, custom, log
from cereal.messaging import PubMaster, SubMaster
from msgq.visionipc import VisionBuf, VisionIpcClient, VisionStreamType
from opendbc.car.car_helpers import get_demo_car_params

from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.issue_debug import log_issue, log_issue_limited
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL, config_realtime_process
from openpilot.common.swaglog import cloudlog
from openpilot.common.transformations.camera import DEVICE_CAMERAS
from openpilot.common.transformations.model import get_warp_matrix
from openpilot.iqpilot.common.steer_delay import resolve_steer_delay
from openpilot.iqpilot.selfdrive.iqmodeld.models.inference_state import InferenceStateBase
from openpilot.selfdrive.controls.lib.desire_helper import DesireHelper
from openpilot.selfdrive.controls.lib.drive_helpers import (
  MODEL_SMOOTHING_MAX_TOTAL_SEC,
  dynamic_lat_smooth_extra_seconds,
  get_accel_from_plan,
  get_curvature_from_plan,
  smooth_value,
)
from openpilot.selfdrive.locationd.calibration_helpers import get_calibrated_rpy
from openpilot.selfdrive.modeld.constants import ModelConstants, Plan
from openpilot.selfdrive.modeld.fill_model_msg import PublishState, fill_model_msg, fill_pose_msg
from openpilot.selfdrive.modeld.parse_model_outputs import Parser


def _configure_runtime_backend() -> bool:
  from openpilot.system.hardware import TICI

  usbgpu = "USBGPU" in os.environ
  if usbgpu:
    os.environ["DEV"] = "AMD"
    os.environ["AMD_IFACE"] = "USB"
  else:
    os.environ["DEV"] = "QCOM" if TICI else "CPU"
  return usbgpu


USBGPU = _configure_runtime_backend()

from tinygrad.dtype import dtypes
from tinygrad.tensor import Tensor

from openpilot.selfdrive.modeld.models.commonmodel_pyx import CLContext, DrivingModelFrame
from openpilot.selfdrive.modeld.runners.tinygrad_helpers import qcom_tensor_from_opencl_address


TurnDirection = custom.IQTurnSignalDirection
PROCESS_NAME = "selfdrive.modeld.modeld"
SEND_RAW_PRED = os.getenv("SEND_RAW_PRED")

VISION_PKL_PATH = Path(__file__).parent / "models/driving_vision_tinygrad.pkl"
POLICY_PKL_PATH = Path(__file__).parent / "models/driving_policy_tinygrad.pkl"
VISION_METADATA_PATH = Path(__file__).parent / "models/driving_vision_metadata.pkl"
POLICY_METADATA_PATH = Path(__file__).parent / "models/driving_policy_metadata.pkl"

LAT_SMOOTH_SECONDS = 0.0
LONG_SMOOTH_SECONDS = 0.3
MIN_LAT_CONTROL_SPEED = 0.3


@dataclass
class CaptureStamp:
  frame_id: int = 0
  timestamp_sof: int = 0
  timestamp_eof: int = 0

  @classmethod
  def from_client(cls, client: VisionIpcClient) -> "CaptureStamp":
    return cls(client.frame_id, client.timestamp_sof, client.timestamp_eof)


@dataclass(frozen=True)
class CameraStreamLayout:
  dual_camera: bool
  primary_stream: VisionStreamType
  primary_is_wide: bool


@dataclass
class FramePair:
  main_buf: VisionBuf
  extra_buf: VisionBuf
  main_meta: CaptureStamp
  extra_meta: CaptureStamp


@dataclass
class WarpState:
  main_transform: np.ndarray
  extra_transform: np.ndarray
  live_calib_seen: bool = False


def _metadata_blob(path: Path) -> dict:
  with open(path, "rb") as handle:
    return pickle.load(handle)


def _zero_one_hot(index: int, width: int) -> np.ndarray:
  vector = np.zeros(width, dtype=np.float32)
  if 0 <= index < width:
    vector[index] = 1.0
  return vector


def _plan_y_std_1s(outputs: dict[str, np.ndarray]) -> float:
  # plan_stds is (batch, IDX_N, PLAN_WIDTH); index 10 ~= 1s ahead (see ModelConstants.T_IDXS),
  # POSITION is an (x, y, z) slice within PLAN_WIDTH so [1] picks the lateral (y) std.
  try:
    return float(outputs["plan_stds"][0, 10, Plan.POSITION][1])
  except (KeyError, IndexError):
    return 0.0


def _model_lat_smooth_max_sec(params: Params) -> float:
  if not params.get_bool("ModelSmoothingEnabled"):
    return 0.0
  try:
    raw = params.get("ModelLatSmoothSec", return_default=True)
    raw = 0 if raw is None else int(raw)
  except (ValueError, TypeError):
    raw = 0
  return min(max(raw, 0), 30) * 0.01


def _compute_action(outputs: dict[str, np.ndarray], previous: log.ModelDataV2.Action,
                    lat_horizon: float, long_horizon: float, speed_mps: float,
                    lat_smooth_seconds: float = LAT_SMOOTH_SECONDS) -> log.ModelDataV2.Action:
  plan = outputs["plan"][0]
  accel_cmd, stop_cmd = get_accel_from_plan(
    plan[:, Plan.VELOCITY][:, 0],
    plan[:, Plan.ACCELERATION][:, 0],
    ModelConstants.T_IDXS,
    action_t=long_horizon,
  )
  accel_cmd = smooth_value(accel_cmd, previous.desiredAcceleration, LONG_SMOOTH_SECONDS)

  curvature_cmd = get_curvature_from_plan(
    plan[:, Plan.T_FROM_CURRENT_EULER][:, 2],
    plan[:, Plan.ORIENTATION_RATE][:, 2],
    ModelConstants.T_IDXS,
    speed_mps,
    lat_horizon,
  )
  if speed_mps > MIN_LAT_CONTROL_SPEED:
    curvature_cmd = smooth_value(curvature_cmd, previous.desiredCurvature, lat_smooth_seconds)
  else:
    curvature_cmd = previous.desiredCurvature

  return log.ModelDataV2.Action(
    desiredCurvature=float(curvature_cmd),
    desiredAcceleration=float(accel_cmd),
    shouldStop=bool(stop_cmd),
  )


class SlidingTensorArchive:
  def __init__(self, model_fps: int, env_fps: int, frame_context: int):
    assert env_fps % model_fps == 0
    assert env_fps >= model_fps
    self._model_fps = model_fps
    self._env_fps = env_fps
    self._frame_context = frame_context
    self._dtypes: dict[str, np.dtype] = {}
    self._shapes: dict[str, tuple[int, ...]] = {}
    self._archive: dict[str, np.ndarray] = {}

  def _expanded_shape(self, tensor_name: str, tensor_shape: tuple[int, ...]) -> tuple[int, ...]:
    if self._env_fps == self._model_fps:
      return tensor_shape
    mutable = list(tensor_shape)
    if "img" in tensor_name:
      channel_span = mutable[1] // self._frame_context
      mutable[1] = (self._env_fps // self._model_fps + (self._frame_context - 1)) * channel_span
    else:
      mutable[1] = (self._env_fps // self._model_fps) * mutable[1]
    return tuple(mutable)

  def register(self, tensor_name: str, tensor_dtype: np.dtype, tensor_shape: tuple[int, ...]) -> None:
    self._dtypes[tensor_name] = tensor_dtype
    self._shapes[tensor_name] = self._expanded_shape(tensor_name, tensor_shape)

  def reset(self) -> None:
    self._archive = {
      name: np.zeros(self._shapes[name], dtype=self._dtypes[name])
      for name in self._dtypes
    }

  def _shift_append(self, tensor_name: str, value: np.ndarray) -> None:
    if value.dtype != self._dtypes[tensor_name]:
      raise ValueError(f"supplied input <{tensor_name}({value.dtype})> has wrong dtype, expected {self._dtypes[tensor_name]}")
    source_shape = list(self._shapes[tensor_name])
    source_shape[1] = -1
    flattened = value.reshape(tuple(source_shape))
    width = flattened.shape[1]
    self._archive[tensor_name][:, :-width] = self._archive[tensor_name][:, width:]
    self._archive[tensor_name][:, -width:] = flattened

  def push(self, inputs: dict[str, np.ndarray]) -> None:
    for tensor_name, value in inputs.items():
      self._shift_append(tensor_name, value)

  def _emit_image_history(self, tensor_name: str, tensor_shape: tuple[int, ...]) -> np.ndarray:
    channel_span = tensor_shape[1] // (self._env_fps // self._model_fps + (self._frame_context - 1))
    windows = [
      self._archive[tensor_name][:, offset:offset + channel_span]
      for offset in np.linspace(0, tensor_shape[1] - channel_span, self._frame_context, dtype=int)
    ]
    return np.concatenate(windows, axis=1)

  def _emit_pulse_history(self, tensor_name: str, tensor_shape: tuple[int, ...]) -> np.ndarray:
    return self._archive[tensor_name].reshape(
      (tensor_shape[0], tensor_shape[1] * self._model_fps // self._env_fps, self._env_fps // self._model_fps, -1)
    ).max(axis=2)

  def _emit_regular_history(self, tensor_name: str, tensor_shape: tuple[int, ...]) -> np.ndarray:
    stride = self._env_fps // self._model_fps
    indices = np.arange(-1, -tensor_shape[1], -stride)[::-1]
    return self._archive[tensor_name][:, indices]

  def export(self, *tensor_names: str) -> dict[str, np.ndarray]:
    if self._env_fps == self._model_fps:
      return {name: self._archive[name] for name in tensor_names}

    exported: dict[str, np.ndarray] = {}
    for tensor_name in tensor_names:
      tensor_shape = self._shapes[tensor_name]
      if "img" in tensor_name:
        exported[tensor_name] = self._emit_image_history(tensor_name, tensor_shape)
      elif "pulse" in tensor_name:
        exported[tensor_name] = self._emit_pulse_history(tensor_name, tensor_shape)
      else:
        exported[tensor_name] = self._emit_regular_history(tensor_name, tensor_shape)
    return exported


class LegacyVisionPolicyRuntime(InferenceStateBase):
  frames: dict[str, DrivingModelFrame]

  def __init__(self, gpu_context: CLContext):
    super().__init__()
    self.LAT_SMOOTH_SECONDS = LAT_SMOOTH_SECONDS

    vision_metadata = _metadata_blob(VISION_METADATA_PATH)
    policy_metadata = _metadata_blob(POLICY_METADATA_PATH)

    self.vision_input_shapes = vision_metadata["input_shapes"]
    self.vision_input_names = list(self.vision_input_shapes.keys())
    self.vision_output_slices = vision_metadata["output_slices"]
    self.policy_input_shapes = policy_metadata["input_shapes"]
    self.policy_output_slices = policy_metadata["output_slices"]

    vision_output_size = vision_metadata["output_shapes"]["outputs"][1]
    policy_output_size = policy_metadata["output_shapes"]["outputs"][1]

    frame_history = ModelConstants.MODEL_RUN_FREQ // ModelConstants.MODEL_CONTEXT_FREQ
    self.frames = {
      stream_name: DrivingModelFrame(gpu_context, frame_history)
      for stream_name in self.vision_input_names
    }

    self._pulse_memory = np.zeros(ModelConstants.DESIRE_LEN, dtype=np.float32)
    self.numpy_inputs = {
      name: np.zeros(shape, dtype=np.float32)
      for name, shape in self.policy_input_shapes.items()
    }
    self._archive = SlidingTensorArchive(
      ModelConstants.MODEL_CONTEXT_FREQ,
      ModelConstants.MODEL_RUN_FREQ,
      ModelConstants.N_FRAMES,
    )
    for tensor_name in ("desire_pulse", "features_buffer"):
      self._archive.register(tensor_name, self.numpy_inputs[tensor_name].dtype, self.numpy_inputs[tensor_name].shape)
    self._archive.reset()

    self._vision_tensors: dict[str, Tensor] = {}
    self._vision_output = np.zeros(vision_output_size, dtype=np.float32)
    self._policy_output = np.zeros(policy_output_size, dtype=np.float32)
    self._policy_views = {name: Tensor(array, device="NPY").realize() for name, array in self.numpy_inputs.items()}
    self._parser = Parser()

    with open(VISION_PKL_PATH, "rb") as handle:
      self._vision_runner = pickle.load(handle)
    with open(POLICY_PKL_PATH, "rb") as handle:
      self._policy_runner = pickle.load(handle)

  @staticmethod
  def _slice_model_output(flat_output: np.ndarray, output_slices: dict[str, slice]) -> dict[str, np.ndarray]:
    return {name: flat_output[np.newaxis, data_slice] for name, data_slice in output_slices.items()}

  def _rising_edge_pulse(self, pulse_values: np.ndarray) -> np.ndarray:
    pulse_values[0] = 0
    rising = np.where(pulse_values - self._pulse_memory > 0.99, pulse_values, 0)
    self._pulse_memory[:] = pulse_values
    return rising

  def _prepare_cl_frames(self, vision_bufs: dict[str, VisionBuf], transform_map: dict[str, np.ndarray]) -> dict[str, Any]:
    return {
      stream_name: self.frames[stream_name].prepare(vision_bufs[stream_name], transform_map[stream_name].flatten())
      for stream_name in self.vision_input_names
    }

  def _refresh_vision_inputs(self, frame_handles: dict[str, Any]) -> None:
    from openpilot.system.hardware import TICI

    if TICI and not USBGPU:
      for stream_name, frame_handle in frame_handles.items():
        if stream_name not in self._vision_tensors:
          self._vision_tensors[stream_name] = qcom_tensor_from_opencl_address(
            frame_handle.mem_address,
            self.vision_input_shapes[stream_name],
            dtype=dtypes.uint8,
          )
      return

    for stream_name, frame_handle in frame_handles.items():
      numpy_frame = self.frames[stream_name].buffer_from_cl(frame_handle).reshape(self.vision_input_shapes[stream_name])
      self._vision_tensors[stream_name] = Tensor(numpy_frame, dtype=dtypes.uint8).realize()

  def _run_vision(self) -> dict[str, np.ndarray]:
    self._vision_output = self._vision_runner(**self._vision_tensors).contiguous().realize().uop.base.buffer.numpy()
    sliced = self._slice_model_output(self._vision_output, self.vision_output_slices)
    return self._parser.parse_vision_outputs(sliced)

  def _refresh_policy_inputs(self, desire_pulse: np.ndarray, hidden_state: np.ndarray, traffic_convention: np.ndarray) -> None:
    self._archive.push({"features_buffer": hidden_state, "desire_pulse": desire_pulse})
    exported = self._archive.export("desire_pulse", "features_buffer")
    self.numpy_inputs["desire_pulse"][:] = exported["desire_pulse"]
    self.numpy_inputs["features_buffer"][:] = exported["features_buffer"]
    self.numpy_inputs["traffic_convention"][:] = traffic_convention

  def _run_policy(self) -> dict[str, np.ndarray]:
    self._policy_output = self._policy_runner(**self._policy_views).contiguous().realize().uop.base.buffer.numpy()
    sliced = self._slice_model_output(self._policy_output, self.policy_output_slices)
    return self._parser.parse_policy_outputs(sliced)

  def evaluate(self, vision_bufs: dict[str, VisionBuf], transform_map: dict[str, np.ndarray],
               fresh_inputs: dict[str, np.ndarray], prepare_only: bool) -> dict[str, np.ndarray] | None:
    desire_pulse = self._rising_edge_pulse(fresh_inputs["desire_pulse"])
    frame_handles = self._prepare_cl_frames(vision_bufs, transform_map)
    self._refresh_vision_inputs(frame_handles)
    if prepare_only:
      return None

    vision_outputs = self._run_vision()
    self._refresh_policy_inputs(desire_pulse, vision_outputs["hidden_state"], fresh_inputs["traffic_convention"])
    policy_outputs = self._run_policy()

    outputs = {**vision_outputs, **policy_outputs}
    if SEND_RAW_PRED:
      outputs["raw_pred"] = np.concatenate([self._vision_output.copy(), self._policy_output.copy()])
    return outputs


def _discover_stream_layout() -> CameraStreamLayout:
  while True:
    available = VisionIpcClient.available_streams("camerad", block=False)
    if available:
      dual_camera = VisionStreamType.VISION_STREAM_WIDE_ROAD in available and VisionStreamType.VISION_STREAM_ROAD in available
      primary_is_wide = VisionStreamType.VISION_STREAM_ROAD not in available
      primary_stream = VisionStreamType.VISION_STREAM_WIDE_ROAD if primary_is_wide else VisionStreamType.VISION_STREAM_ROAD
      return CameraStreamLayout(dual_camera=dual_camera, primary_stream=primary_stream, primary_is_wide=primary_is_wide)
    time.sleep(0.1)


def _connect_streams(gpu_context: CLContext, layout: CameraStreamLayout) -> tuple[VisionIpcClient, VisionIpcClient]:
  main_client = VisionIpcClient("camerad", layout.primary_stream, True, gpu_context)
  extra_client = VisionIpcClient("camerad", VisionStreamType.VISION_STREAM_WIDE_ROAD, False, gpu_context)

  cloudlog.warning(
    f"vision stream set up, main_wide_camera: {layout.primary_is_wide}, use_extra_client: {layout.dual_camera}"
  )
  while not main_client.connect(False):
    time.sleep(0.1)
  while layout.dual_camera and not extra_client.connect(False):
    time.sleep(0.1)

  cloudlog.warning(
    f"connected main cam with buffer size: {main_client.buffer_len} ({main_client.width} x {main_client.height})"
  )
  if layout.dual_camera:
    cloudlog.warning(
      f"connected extra cam with buffer size: {extra_client.buffer_len} ({extra_client.width} x {extra_client.height})"
    )
  return main_client, extra_client


def _receive_frame_pair(main_client: VisionIpcClient, extra_client: VisionIpcClient,
                        previous_extra_meta: CaptureStamp, dual_camera: bool) -> FramePair | None:
  main_meta = CaptureStamp()
  extra_meta = previous_extra_meta
  main_buf = None

  while main_meta.timestamp_sof < extra_meta.timestamp_sof + 25_000_000:
    main_buf = main_client.recv()
    main_meta = CaptureStamp.from_client(main_client)
    if main_buf is None:
      return None

  if not dual_camera:
    return FramePair(main_buf=main_buf, extra_buf=main_buf, main_meta=main_meta, extra_meta=main_meta)

  extra_buf = None
  while True:
    extra_buf = extra_client.recv()
    extra_meta = CaptureStamp.from_client(extra_client)
    if extra_buf is None or main_meta.timestamp_sof < extra_meta.timestamp_sof + 25_000_000:
      break

  if extra_buf is None:
    return None

  if abs(main_meta.timestamp_sof - extra_meta.timestamp_sof) > 10_000_000:
    cloudlog.error(
      f"frames out of sync! main: {main_meta.frame_id} ({main_meta.timestamp_sof / 1e9:.5f}), "
      f"extra: {extra_meta.frame_id} ({extra_meta.timestamp_sof / 1e9:.5f})"
    )
  return FramePair(main_buf=main_buf, extra_buf=extra_buf, main_meta=main_meta, extra_meta=extra_meta)


def _zero_warp_state() -> WarpState:
  return WarpState(
    main_transform=np.zeros((3, 3), dtype=np.float32),
    extra_transform=np.zeros((3, 3), dtype=np.float32),
    live_calib_seen=False,
  )


def _refresh_warp_state(sm: SubMaster, layout: CameraStreamLayout, state: WarpState) -> WarpState:
  if not (sm.seen["liveCalibration"] and sm.seen["roadCameraState"] and sm.seen["deviceState"]):
    return state

  calibrated_rpy = get_calibrated_rpy(sm["liveCalibration"])
  if calibrated_rpy is None and not state.live_calib_seen:
    live_calib = sm["liveCalibration"]
    cal_status = getattr(live_calib.calStatus, "raw", live_calib.calStatus)
    log_issue_limited(
      "modeld_waiting_for_calibration",
      "calibration",
      f"modeld using identity warp while calibration stabilizes status={cal_status} "
      f"perc={float(live_calib.calPerc):.1f} rpy={list(live_calib.rpyCalib)} height={list(live_calib.height)}",
      interval_sec=2.0,
    )
    calibrated_rpy = np.zeros(3, dtype=np.float32)

  if calibrated_rpy is None:
    return state

  camera_block = DEVICE_CAMERAS[(str(sm["deviceState"].deviceType), str(sm["roadCameraState"].sensor))]
  primary_intrinsics = camera_block.ecam.intrinsics if layout.primary_is_wide else camera_block.fcam.intrinsics
  extra_intrinsics = camera_block.ecam.intrinsics if layout.dual_camera or layout.primary_is_wide else camera_block.fcam.intrinsics
  next_state = WarpState(
    main_transform=get_warp_matrix(calibrated_rpy, primary_intrinsics, False).astype(np.float32),
    extra_transform=get_warp_matrix(calibrated_rpy, extra_intrinsics, True).astype(np.float32),
    live_calib_seen=True,
  )
  if not state.live_calib_seen:
    log_issue("calibration", f"modeld accepted calibrated warp rpy={calibrated_rpy.tolist()}")
  return next_state


def _traffic_convention_vector(is_rhd: bool) -> np.ndarray:
  convention = np.zeros(2, dtype=np.float32)
  convention[int(is_rhd)] = 1.0
  return convention


def _drop_statistics(frame_filter: FirstOrderFilter, dropped_frames: int, run_count: int) -> tuple[float, int]:
  filtered = frame_filter.update(min(dropped_frames, 10))
  if run_count < 10:
    frame_filter.x = 0.0
    filtered = 0.0
  run_count += 1
  return filtered / (1 + filtered), run_count


def _vision_inputs(runtime: LegacyVisionPolicyRuntime, frame_pair: FramePair) -> dict[str, VisionBuf]:
  return {
    stream_name: frame_pair.extra_buf if "big" in stream_name else frame_pair.main_buf
    for stream_name in runtime.vision_input_names
  }


def _warp_inputs(runtime: LegacyVisionPolicyRuntime, warp_state: WarpState) -> dict[str, np.ndarray]:
  return {
    stream_name: warp_state.extra_transform if "big" in stream_name else warp_state.main_transform
    for stream_name in runtime.vision_input_names
  }


def _publish_outputs(pm: PubMaster, publish_state: PublishState, frame_pair: FramePair, frame_id: int,
                     frame_drop_ratio: float, model_execution_time: float, live_calib_seen: bool,
                     outputs: dict[str, np.ndarray], action: log.ModelDataV2.Action,
                     desire_logic: DesireHelper, dropped_frames: int) -> float:
  model_msg = messaging.new_message("modelV2")
  drive_msg = messaging.new_message("drivingModelData")
  pose_msg = messaging.new_message("cameraOdometry")
  iq_msg = messaging.new_message("iqDriveModelData")

  fill_model_msg(
    drive_msg,
    model_msg,
    outputs,
    action,
    publish_state,
    frame_pair.main_meta.frame_id,
    frame_pair.extra_meta.frame_id,
    frame_id,
    frame_drop_ratio,
    frame_pair.main_meta.timestamp_eof,
    model_execution_time,
    live_calib_seen,
  )

  desire_state = model_msg.modelV2.meta.desireState
  lane_change_prob = desire_state[log.Desire.laneChangeLeft] + desire_state[log.Desire.laneChangeRight]
  dh_start = time.perf_counter()
  return lane_change_prob, dh_start, model_msg, drive_msg, pose_msg, iq_msg


def _apply_desire_logic(sm: SubMaster, desire_logic: DesireHelper, lane_change_prob: float,
                        model_msg, drive_msg, iq_msg, dh_start: float) -> None:
  desire_logic.update(
    sm["carState"],
    sm["carControl"].latActive,
    lane_change_prob,
    sm["iqNavState"],
    model_msg.modelV2,
    sm["radarState"],
  )
  dh_ms = (time.perf_counter() - dh_start) * 1000.0
  if dh_ms > 1.0 or getattr(sm["iqNavState"], "active", False):
    log_issue_limited(
      "modeld_desire_helper",
      "Lag",
      f"modeld desire helper dh_ms={dh_ms:.2f} nav_active={getattr(sm['iqNavState'], 'active', False)} "
      f"lane_turn={int(desire_logic.lane_turn_direction)} nav_turn={int(desire_logic.nav_turn_direction)}",
      interval_sec=1.0,
    )

  model_msg.modelV2.meta.laneChangeState = desire_logic.lane_change_state
  model_msg.modelV2.meta.laneChangeDirection = desire_logic.lane_change_direction
  drive_msg.drivingModelData.meta.laneChangeState = desire_logic.lane_change_state
  drive_msg.drivingModelData.meta.laneChangeDirection = desire_logic.lane_change_direction
  iq_msg.iqDriveModelData.turnSignalDirection = desire_logic.lane_turn_direction


def run_stock_modeld(demo: bool = False):
  cloudlog.warning("modeld init")

  if not USBGPU:
    config_realtime_process(7, 54)

  boot_start = time.monotonic()
  cloudlog.warning("setting up CL context")
  gpu_context = CLContext()
  cloudlog.warning("CL context ready; loading model")
  runtime = LegacyVisionPolicyRuntime(gpu_context)
  cloudlog.warning(f"models loaded in {time.monotonic() - boot_start:.1f}s, modeld starting")

  layout = _discover_stream_layout()
  main_client, extra_client = _connect_streams(gpu_context, layout)

  pm = PubMaster(["modelV2", "drivingModelData", "cameraOdometry", "iqDriveModelData"])
  sm = SubMaster([
    "deviceState", "carState", "roadCameraState", "liveCalibration",
    "driverMonitoringState", "carControl", "liveDelay", "iqNavState", "radarState",
  ])

  publish_state = PublishState()
  params = Params()
  frame_filter = FirstOrderFilter(0.0, 10.0, 1.0 / ModelConstants.MODEL_RUN_FREQ)
  warp_state = _zero_warp_state()
  last_main_frame_id = 0
  loop_count = 0
  extra_meta = CaptureStamp()

  car_params = get_demo_car_params() if demo else messaging.log_from_bytes(params.get("CarParams", block=True), car.CarParams)
  cloudlog.info("modeld got CarParams: %s", car_params.brand)

  long_horizon = car_params.longitudinalActuatorDelay + LONG_SMOOTH_SECONDS
  previous_action = log.ModelDataV2.Action()
  desire_logic = DesireHelper()

  model_smoothing_max_extra_sec = 0.0
  lat_smooth_extra_sec = 0.0

  while True:
    frame_pair = _receive_frame_pair(main_client, extra_client, extra_meta, layout.dual_camera)
    if frame_pair is None:
      cloudlog.debug("vipc_client no frame")
      continue
    extra_meta = frame_pair.extra_meta

    sm.update(0)
    current_desire = desire_logic.desire
    vehicle_speed = max(sm["carState"].vEgo, 0.0)
    frame_id = sm["roadCameraState"].frameId
    if sm.frame % 60 == 0:
      runtime.lat_delay = resolve_steer_delay(params, sm["liveDelay"].lateralDelay)
      model_smoothing_max_extra_sec = _model_lat_smooth_max_sec(params)
    lat_horizon = runtime.lat_delay + LAT_SMOOTH_SECONDS + lat_smooth_extra_sec

    warp_state = _refresh_warp_state(sm, layout, warp_state)
    fresh_inputs = {
      "desire_pulse": _zero_one_hot(current_desire, ModelConstants.DESIRE_LEN),
      "traffic_convention": _traffic_convention_vector(sm["driverMonitoringState"].isRHD),
    }

    dropped_frames = max(0, frame_pair.main_meta.frame_id - last_main_frame_id - 1)
    frame_drop_ratio, loop_count = _drop_statistics(frame_filter, dropped_frames, loop_count)
    prepare_only = dropped_frames > 0
    if prepare_only:
      cloudlog.error(f"skipping model eval. Dropped {dropped_frames} frames")

    start_time = time.perf_counter()
    outputs = runtime.evaluate(
      _vision_inputs(runtime, frame_pair),
      _warp_inputs(runtime, warp_state),
      fresh_inputs,
      prepare_only,
    )
    model_execution_time = time.perf_counter() - start_time

    if outputs is not None:
      lat_smooth_extra_sec = dynamic_lat_smooth_extra_seconds(_plan_y_std_1s(outputs), model_smoothing_max_extra_sec)
      lat_smooth_total_sec = min(LAT_SMOOTH_SECONDS + lat_smooth_extra_sec, MODEL_SMOOTHING_MAX_TOTAL_SEC)
      action = _compute_action(outputs, previous_action, lat_horizon + DT_MDL, long_horizon + DT_MDL, vehicle_speed,
                               lat_smooth_total_sec)
      previous_action = action

      lane_change_prob, dh_start, model_msg, drive_msg, pose_msg, iq_msg = _publish_outputs(
        pm,
        publish_state,
        frame_pair,
        frame_id,
        frame_drop_ratio,
        model_execution_time,
        warp_state.live_calib_seen,
        outputs,
        action,
        desire_logic,
        dropped_frames,
      )
      _apply_desire_logic(sm, desire_logic, lane_change_prob, model_msg, drive_msg, iq_msg, dh_start)
      fill_pose_msg(
        pose_msg,
        outputs,
        frame_pair.main_meta.frame_id,
        dropped_frames,
        frame_pair.main_meta.timestamp_eof,
        warp_state.live_calib_seen,
      )

      pm.send("modelV2", model_msg)
      pm.send("drivingModelData", drive_msg)
      pm.send("cameraOdometry", pose_msg)
      pm.send("iqDriveModelData", iq_msg)

    last_main_frame_id = frame_pair.main_meta.frame_id
