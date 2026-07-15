/*
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
*/
#include "iqpilot/selfdrive/iqmodeld/transforms/warp_geometry.h"

#include <assert.h>
#include <cstring>

#include "common/clutil.h"

namespace {

void reset_sampler_state(WarpSamplerState *sampler) {
  memset(sampler, 0, sizeof(*sampler));
}

void write_projection(cl_command_queue queue, cl_mem dst, const mat3 &projection) {
  CL_CHECK(clEnqueueWriteBuffer(queue, dst, CL_TRUE, 0, 3 * 3 * sizeof(float), (void *)projection.v, 0, NULL, NULL));
}

void configure_sample_window(WarpSamplerState *sampler, cl_mem src, int src_stride, int src_px_stride,
                             int src_offset, int src_rows, int src_cols,
                             cl_mem dst, int dst_stride, int dst_offset, int dst_rows, int dst_cols,
                             cl_mem projection_cl) {
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 0, sizeof(cl_mem), &src));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 1, sizeof(cl_int), &src_stride));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 2, sizeof(cl_int), &src_px_stride));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 3, sizeof(cl_int), &src_offset));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 4, sizeof(cl_int), &src_rows));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 5, sizeof(cl_int), &src_cols));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 6, sizeof(cl_mem), &dst));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 7, sizeof(cl_int), &dst_stride));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 8, sizeof(cl_int), &dst_offset));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 9, sizeof(cl_int), &dst_rows));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 10, sizeof(cl_int), &dst_cols));
  CL_CHECK(clSetKernelArg(sampler->bilinear_kernel, 11, sizeof(cl_mem), &projection_cl));
}

void enqueue_sample_window(cl_command_queue queue, cl_kernel kernel, int width, int height) {
  const size_t work_size[2] = {static_cast<size_t>(width), static_cast<size_t>(height)};
  CL_CHECK(clEnqueueNDRangeKernel(queue, kernel, 2, NULL, work_size, NULL, 0, 0, NULL));
}

}  // namespace

void warp_sampler_init(WarpSamplerState *sampler, cl_context ctx, cl_device_id device_id) {
  reset_sampler_state(sampler);
  cl_program program_handle = cl_program_from_file(ctx, device_id, TRANSFORM_PATH, "");
  sampler->bilinear_kernel = CL_CHECK_ERR(clCreateKernel(program_handle, "projectPlaneBilinear", &err));
  CL_CHECK(clReleaseProgram(program_handle));

  sampler->full_res_matrix_cl = CL_CHECK_ERR(clCreateBuffer(ctx, CL_MEM_READ_WRITE, 3 * 3 * sizeof(float), NULL, &err));
  sampler->half_res_matrix_cl = CL_CHECK_ERR(clCreateBuffer(ctx, CL_MEM_READ_WRITE, 3 * 3 * sizeof(float), NULL, &err));
}

void warp_sampler_release(WarpSamplerState *sampler) {
  CL_CHECK(clReleaseMemObject(sampler->full_res_matrix_cl));
  CL_CHECK(clReleaseMemObject(sampler->half_res_matrix_cl));
  CL_CHECK(clReleaseKernel(sampler->bilinear_kernel));
}

void warp_sampler_dispatch(WarpSamplerState *sampler, cl_command_queue queue,
                           cl_mem yuv, int in_width, int in_height, int in_stride, int in_uv_offset,
                           cl_mem out_y, cl_mem out_u, cl_mem out_v,
                           int out_width, int out_height,
                           const mat3 &projection) {
  const mat3 luma_projection = projection;
  const mat3 chroma_projection = transform_scale_buffer(projection, 0.5);

  write_projection(queue, sampler->full_res_matrix_cl, luma_projection);
  write_projection(queue, sampler->half_res_matrix_cl, chroma_projection);

  configure_sample_window(sampler, yuv, in_stride, 1, 0, in_height, in_width,
                          out_y, out_width, 0, out_height, out_width,
                          sampler->full_res_matrix_cl);
  enqueue_sample_window(queue, sampler->bilinear_kernel, out_width, out_height);

  const int chroma_width = in_width / 2;
  const int chroma_height = in_height / 2;
  const int out_chroma_width = out_width / 2;
  const int out_chroma_height = out_height / 2;
  const int in_u_offset = in_uv_offset;
  const int in_v_offset = in_uv_offset + 1;

  configure_sample_window(sampler, yuv, in_stride, 2, in_u_offset, chroma_height, chroma_width,
                          out_u, out_chroma_width, 0, out_chroma_height, out_chroma_width,
                          sampler->half_res_matrix_cl);
  enqueue_sample_window(queue, sampler->bilinear_kernel, out_chroma_width, out_chroma_height);

  configure_sample_window(sampler, yuv, in_stride, 2, in_v_offset, chroma_height, chroma_width,
                          out_v, out_chroma_width, 0, out_chroma_height, out_chroma_width,
                          sampler->half_res_matrix_cl);
  enqueue_sample_window(queue, sampler->bilinear_kernel, out_chroma_width, out_chroma_height);
}
