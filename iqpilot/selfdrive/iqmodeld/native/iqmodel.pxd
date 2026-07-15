# -*- coding: utf-8 -*-
# distutils: language = c++
# Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/

from msgq.visionipc.visionipc cimport cl_device_id, cl_context, cl_mem

cdef extern from "common/mat.h":
  cdef struct mat3:
    float v[9]

cdef extern from "common/clutil.h":
  cdef unsigned long CL_DEVICE_TYPE_DEFAULT
  cl_device_id cl_get_device_id(unsigned long)
  cl_context cl_create_context(cl_device_id)
  void cl_release_context(cl_context)

cdef extern from "iqpilot/selfdrive/iqmodeld/native/iqmodel.h":
  cppclass NativeFrameBridge "FrameCropperBase":
    int buf_size
    unsigned char * copy_to_host(cl_mem*, int);
    cl_mem * project_to_cl(cl_mem, int, int, int, int, mat3)

  cppclass RoadFrameBridge "RoadHistoryAssembler":
    int buf_size
    RoadFrameBridge(cl_device_id, cl_context, unsigned char)

  cppclass CabinFrameBridge "CabinFrameSampler":
    int buf_size
    CabinFrameBridge(cl_device_id, cl_context)
