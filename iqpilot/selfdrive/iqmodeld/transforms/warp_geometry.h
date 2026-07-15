/*
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
*/
#pragma once

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "common/mat.h"

struct WarpSamplerState {
  cl_kernel bilinear_kernel;
  cl_mem full_res_matrix_cl;
  cl_mem half_res_matrix_cl;
};

void warp_sampler_init(WarpSamplerState *sampler, cl_context ctx, cl_device_id device_id);
void warp_sampler_release(WarpSamplerState *sampler);

void warp_sampler_dispatch(WarpSamplerState *sampler, cl_command_queue queue,
                           cl_mem yuv, int in_width, int in_height, int in_stride, int in_uv_offset,
                           cl_mem out_y, cl_mem out_u, cl_mem out_v,
                           int out_width, int out_height,
                           const mat3 &projection);
