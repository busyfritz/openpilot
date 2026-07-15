/*
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
*/
#pragma once

#include "common/clutil.h"

struct PackedFrameKernels {
  int raster_width;
  int raster_height;
  cl_kernel y_pair_kernel;
  cl_kernel uv_lane_kernel;
  cl_kernel span_copy_kernel;
};

void packed_frame_kernels_init(PackedFrameKernels *kernels, cl_context ctx, cl_device_id device_id, int width, int height);
void packed_frame_kernels_release(PackedFrameKernels *kernels);

void packed_frame_emit(PackedFrameKernels *kernels, cl_command_queue queue,
                       cl_mem y_plane_cl, cl_mem u_plane_cl, cl_mem v_plane_cl,
                       cl_mem packed_frame_cl);

void packed_frame_clone_range(PackedFrameKernels *kernels, cl_command_queue queue, cl_mem src, cl_mem dst,
                              size_t src_offset, size_t dst_offset, size_t size);
