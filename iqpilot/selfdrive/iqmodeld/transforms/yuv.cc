/*
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
*/
#include "iqpilot/selfdrive/iqmodeld/transforms/yuv.h"

#include <assert.h>
#include <cstdio>
#include <cstring>

namespace {

void clear_kernel_bundle(PackedFrameKernels *kernels) {
  memset(kernels, 0, sizeof(*kernels));
}

void bind_kernel_bundle(PackedFrameKernels *kernels, cl_program cl_program_handle) {
  kernels->y_pair_kernel = CL_CHECK_ERR(clCreateKernel(cl_program_handle, "packLumaHalves", &err));
  kernels->uv_lane_kernel = CL_CHECK_ERR(clCreateKernel(cl_program_handle, "packChromaPlane", &err));
  kernels->span_copy_kernel = CL_CHECK_ERR(clCreateKernel(cl_program_handle, "copyPlaneBytes", &err));
}

void launch_linear_kernel(cl_command_queue queue, cl_kernel kernel, size_t work_items) {
  CL_CHECK(clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &work_items, nullptr, 0, 0, nullptr));
}

}  // namespace

void packed_frame_kernels_init(PackedFrameKernels *kernels, cl_context ctx, cl_device_id device_id, int width, int height) {
  clear_kernel_bundle(kernels);
  kernels->raster_width = width;
  kernels->raster_height = height;

  char compiler_args[1024];
  snprintf(compiler_args, sizeof(compiler_args),
           "-cl-fast-relaxed-math -cl-denorms-are-zero "
           "-DTRANSFORMED_WIDTH=%d -DTRANSFORMED_HEIGHT=%d",
           width, height);

  cl_program program_handle = cl_program_from_file(ctx, device_id, LOADYUV_PATH, compiler_args);
  bind_kernel_bundle(kernels, program_handle);
  CL_CHECK(clReleaseProgram(program_handle));
}

void packed_frame_kernels_release(PackedFrameKernels *kernels) {
  CL_CHECK(clReleaseKernel(kernels->y_pair_kernel));
  CL_CHECK(clReleaseKernel(kernels->uv_lane_kernel));
  CL_CHECK(clReleaseKernel(kernels->span_copy_kernel));
}

void packed_frame_emit(PackedFrameKernels *kernels, cl_command_queue queue,
                       cl_mem y_plane_cl, cl_mem u_plane_cl, cl_mem v_plane_cl,
                       cl_mem packed_frame_cl) {
  cl_int output_offset = 0;
  const size_t luma_work_items = (kernels->raster_width * kernels->raster_height) / 8;
  const size_t chroma_work_items = ((kernels->raster_width / 2) * (kernels->raster_height / 2)) / 8;

  CL_CHECK(clSetKernelArg(kernels->y_pair_kernel, 0, sizeof(cl_mem), &y_plane_cl));
  CL_CHECK(clSetKernelArg(kernels->y_pair_kernel, 1, sizeof(cl_mem), &packed_frame_cl));
  CL_CHECK(clSetKernelArg(kernels->y_pair_kernel, 2, sizeof(cl_int), &output_offset));
  launch_linear_kernel(queue, kernels->y_pair_kernel, luma_work_items);

  output_offset += kernels->raster_width * kernels->raster_height;
  CL_CHECK(clSetKernelArg(kernels->uv_lane_kernel, 0, sizeof(cl_mem), &u_plane_cl));
  CL_CHECK(clSetKernelArg(kernels->uv_lane_kernel, 1, sizeof(cl_mem), &packed_frame_cl));
  CL_CHECK(clSetKernelArg(kernels->uv_lane_kernel, 2, sizeof(cl_int), &output_offset));
  launch_linear_kernel(queue, kernels->uv_lane_kernel, chroma_work_items);

  output_offset += (kernels->raster_width / 2) * (kernels->raster_height / 2);
  CL_CHECK(clSetKernelArg(kernels->uv_lane_kernel, 0, sizeof(cl_mem), &v_plane_cl));
  CL_CHECK(clSetKernelArg(kernels->uv_lane_kernel, 1, sizeof(cl_mem), &packed_frame_cl));
  CL_CHECK(clSetKernelArg(kernels->uv_lane_kernel, 2, sizeof(cl_int), &output_offset));
  launch_linear_kernel(queue, kernels->uv_lane_kernel, chroma_work_items);
}

void packed_frame_clone_range(PackedFrameKernels *kernels, cl_command_queue queue, cl_mem src, cl_mem dst,
                              size_t src_offset, size_t dst_offset, size_t size) {
  CL_CHECK(clSetKernelArg(kernels->span_copy_kernel, 0, sizeof(cl_mem), &src));
  CL_CHECK(clSetKernelArg(kernels->span_copy_kernel, 1, sizeof(cl_mem), &dst));
  CL_CHECK(clSetKernelArg(kernels->span_copy_kernel, 2, sizeof(cl_int), &src_offset));
  CL_CHECK(clSetKernelArg(kernels->span_copy_kernel, 3, sizeof(cl_int), &dst_offset));
  launch_linear_kernel(queue, kernels->span_copy_kernel, size / 8);
}
