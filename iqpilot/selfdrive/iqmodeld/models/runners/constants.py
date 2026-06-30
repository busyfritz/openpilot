"""
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
"""

from __future__ import annotations

import os
from typing import TYPE_CHECKING, Any

import numpy as np
from cereal import custom
from openpilot.system.hardware.hw import Paths

if TYPE_CHECKING:
  from openpilot.iqpilot.selfdrive.iqmodeld.native.iqmodel_pyx import GpuMemorySlot, RoadProjector
else:
  def _resolve_native_types() -> tuple[Any, Any]:
    try:
      from openpilot.iqpilot.selfdrive.iqmodeld.native.iqmodel_pyx import GpuMemorySlot as iq_clmem
      from openpilot.iqpilot.selfdrive.iqmodeld.native.iqmodel_pyx import RoadProjector as iq_frame
      return iq_clmem, iq_frame
    except (ModuleNotFoundError, ImportError):
      return Any, Any

  GpuMemorySlot, RoadProjector = _resolve_native_types()

NumpyDict = dict[str, np.ndarray]
ShapeDict = dict[str, tuple[int, ...]]
SliceDict = dict[str, slice]
CLMemDict = dict[str, GpuMemorySlot]
FrameDict = dict[str, RoadProjector]

ModelType = custom.IQModelManager.Model.Type
Model = custom.IQModelManager.Model

SEND_RAW_PRED = os.getenv("SEND_RAW_PRED")
CUSTOM_MODEL_PATH = Paths.model_root()
