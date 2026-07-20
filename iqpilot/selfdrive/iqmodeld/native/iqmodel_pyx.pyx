# -*- coding: utf-8 -*-
# distutils: language = c++
# cython: c_string_encoding=ascii, language_level=3
# Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/

import numpy as np
cimport numpy as cnp
from libc.string cimport memcpy
from libc.stdint cimport uintptr_t

from msgq.visionipc.visionipc cimport cl_mem
from msgq.visionipc.visionipc_pyx cimport VisionBuf, CLContext as VisionIpcContextBase
from .iqmodel cimport CL_DEVICE_TYPE_DEFAULT, cl_get_device_id, cl_create_context, cl_release_context
from .iqmodel cimport mat3, NativeFrameBridge, RoadFrameBridge, CabinFrameBridge


cdef inline mat3 _projection_to_mat3(float[:] values):
  cdef mat3 warp_matrix
  memcpy(warp_matrix.v, &values[0], 9 * sizeof(float))
  return warp_matrix


cdef inline GpuMemorySlot _borrow_cl_slot(void * raw_handle):
  cdef GpuMemorySlot carrier = GpuMemorySlot()
  carrier.handle_ptr = <cl_mem*>raw_handle
  return carrier


cdef inline object _read_u8_view(unsigned char * payload, int length):
  return np.asarray(<cnp.uint8_t[:length]> payload)


cdef class WarpContext(VisionIpcContextBase):
  def __cinit__(self):
    self.device_id = cl_get_device_id(CL_DEVICE_TYPE_DEFAULT)
    self.context = cl_create_context(self.device_id)

  def __dealloc__(self):
    if self.context:
      cl_release_context(self.context)

cdef class GpuMemorySlot:
  @property
  def mem_address(self):
    return <uintptr_t>(self.handle_ptr)


def cl_from_visionbuf(VisionBuf buf):
  return _borrow_cl_slot(<void*>&buf.buf.buf_cl)


cdef class _ProjectionBridge:
  cdef NativeFrameBridge * _native_ptr
  cdef int _export_bytes

  def __dealloc__(self):
    del self._native_ptr

  cdef void _attach(self, NativeFrameBridge * native_frame, int export_bytes):
    self._native_ptr = native_frame
    self._export_bytes = export_bytes

  cdef GpuMemorySlot _stage_image(self, VisionBuf buf, float[:] projection):
    cdef mat3 projection_spec = _projection_to_mat3(projection)
    cdef cl_mem * exported_slot = self._native_ptr.project_to_cl(
      buf.buf.buf_cl,
      buf.width,
      buf.height,
      buf.stride,
      buf.uv_offset,
      projection_spec,
    )
    return _borrow_cl_slot(exported_slot)

  cdef object _export_host_bytes(self, GpuMemorySlot opencl_slot):
    cdef unsigned char * payload = self._native_ptr.copy_to_host(opencl_slot.handle_ptr, self._export_bytes)
    return _read_u8_view(payload, self._export_bytes)


cdef class FrameProjector(_ProjectionBridge):
  def stage(self, VisionBuf buf, float[:] projection):
    return self._stage_image(buf, projection)

  def as_numpy(self, GpuMemorySlot in_frames):
    return self._export_host_bytes(in_frames)


cdef class RoadProjector(FrameProjector):
  cdef RoadFrameBridge * _road_ptr

  def __cinit__(self, WarpContext context, int buffer_length=2):
    self._road_ptr = new RoadFrameBridge(context.device_id, context.context, buffer_length)
    self._attach(<NativeFrameBridge*>self._road_ptr, self._road_ptr.buf_size)

cdef class CabinProjector(FrameProjector):
  cdef CabinFrameBridge * _cabin_ptr

  def __cinit__(self, WarpContext context):
    self._cabin_ptr = new CabinFrameBridge(context.device_id, context.context)
    self._attach(<NativeFrameBridge*>self._cabin_ptr, self._cabin_ptr.buf_size)
