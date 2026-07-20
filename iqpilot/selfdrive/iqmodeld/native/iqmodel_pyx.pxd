# -*- coding: utf-8 -*-
# distutils: language = c++
# Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/

from msgq.visionipc.visionipc cimport cl_mem
from msgq.visionipc.visionipc_pyx cimport CLContext as VisionIpcContextBase

cdef class WarpContext(VisionIpcContextBase):
  pass

cdef class GpuMemorySlot:
  cdef cl_mem * handle_ptr
