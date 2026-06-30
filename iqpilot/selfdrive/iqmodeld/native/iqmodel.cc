/*
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
*/
#include "iqpilot/selfdrive/iqmodeld/native/iqmodel.h"

#include <cstring>

#include "common/clutil.h"

namespace {

void rotate_history_window(cl_command_queue queue, cl_mem timeline_cl, uint8_t history_slots, size_t frame_bytes) {
  for (int slot = 0; slot < (history_slots - 1); slot++) {
    CL_CHECK(clEnqueueCopyBuffer(queue, timeline_cl, timeline_cl,
                                 (slot + 1) * frame_bytes, slot * frame_bytes,
                                 frame_bytes, 0, nullptr, nullptr));
  }
}

}  // namespace

FrameCropperBase::FrameCropperBase(cl_device_id device_id, cl_context context) {
  work_queue_ = CL_CHECK_ERR(clCreateCommandQueue(context, device_id, 0, &err));
}

FrameCropperBase::~FrameCropperBase() {
  CL_CHECK(clReleaseCommandQueue(work_queue_));
}

void FrameCropperBase::configure_planar_tiles(cl_device_id device_id, cl_context context, int output_width, int output_height) {
  y_plane_tile_cl_ = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE, output_width * output_height, NULL, &err));
  u_plane_tile_cl_ = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE, (output_width / 2) * (output_height / 2), NULL, &err));
  v_plane_tile_cl_ = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE, (output_width / 2) * (output_height / 2), NULL, &err));
  warp_sampler_init(&sampler_state_, context, device_id);
}

void FrameCropperBase::release_planar_tiles() {
  warp_sampler_release(&sampler_state_);
  CL_CHECK(clReleaseMemObject(v_plane_tile_cl_));
  CL_CHECK(clReleaseMemObject(u_plane_tile_cl_));
  CL_CHECK(clReleaseMemObject(y_plane_tile_cl_));
}

void FrameCropperBase::project_frame(cl_mem yuv_cl, int output_width, int output_height,
                                     int frame_width, int frame_height, int frame_stride, int frame_uv_offset,
                                     const mat3 &projection) {
  warp_sampler_dispatch(&sampler_state_, work_queue_,
                        yuv_cl, frame_width, frame_height, frame_stride, frame_uv_offset,
                        y_plane_tile_cl_, u_plane_tile_cl_, v_plane_tile_cl_,
                        output_width, output_height, projection);
}

RoadHistoryAssembler::RoadHistoryAssembler(cl_device_id device_id, cl_context context, uint8_t history_slots)
    : FrameCropperBase(device_id, context), frame_bytes_(kFrameBytes * sizeof(uint8_t)), history_slots_(history_slots) {
  buf_size = kExportBytes;
  staging_bytes_ = std::make_unique<uint8_t[]>(buf_size);
  publish_pair_cl_ = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE, buf_size, NULL, &err));
  timeline_cl_ = CL_CHECK_ERR(clCreateBuffer(context, CL_MEM_READ_WRITE, history_slots_ * frame_bytes_, NULL, &err));

  latest_region_.origin = (history_slots_ - 1) * frame_bytes_;
  latest_region_.size = frame_bytes_;
  latest_slot_cl_ = CL_CHECK_ERR(clCreateSubBuffer(timeline_cl_, CL_MEM_READ_WRITE, CL_BUFFER_CREATE_TYPE_REGION, &latest_region_, &err));

  packed_frame_kernels_init(&packer_, context, device_id, kOutputWidth, kOutputHeight);
  configure_planar_tiles(device_id, context, kOutputWidth, kOutputHeight);
}

cl_mem *RoadHistoryAssembler::project_to_cl(cl_mem yuv_cl, int frame_width, int frame_height, int frame_stride, int frame_uv_offset, const mat3 &projection) {
  project_frame(yuv_cl, kOutputWidth, kOutputHeight, frame_width, frame_height, frame_stride, frame_uv_offset, projection);
  rotate_history_window(work_queue_, timeline_cl_, history_slots_, frame_bytes_);
  packed_frame_emit(&packer_, work_queue_, y_plane_tile_cl_, u_plane_tile_cl_, v_plane_tile_cl_, latest_slot_cl_);
  packed_frame_clone_range(&packer_, work_queue_, timeline_cl_, publish_pair_cl_, 0, 0, frame_bytes_);
  packed_frame_clone_range(&packer_, work_queue_, latest_slot_cl_, publish_pair_cl_, 0, frame_bytes_, frame_bytes_);
  clFinish(work_queue_);
  return &publish_pair_cl_;
}

RoadHistoryAssembler::~RoadHistoryAssembler() {
  release_planar_tiles();
  packed_frame_kernels_release(&packer_);
  CL_CHECK(clReleaseMemObject(publish_pair_cl_));
  CL_CHECK(clReleaseMemObject(timeline_cl_));
  CL_CHECK(clReleaseMemObject(latest_slot_cl_));
}

CabinFrameSampler::CabinFrameSampler(cl_device_id device_id, cl_context context) : FrameCropperBase(device_id, context) {
  buf_size = kExportBytes;
  staging_bytes_ = std::make_unique<uint8_t[]>(buf_size);
  configure_planar_tiles(device_id, context, kOutputWidth, kOutputHeight);
}

cl_mem *CabinFrameSampler::project_to_cl(cl_mem yuv_cl, int frame_width, int frame_height, int frame_stride, int frame_uv_offset, const mat3 &projection) {
  project_frame(yuv_cl, kOutputWidth, kOutputHeight, frame_width, frame_height, frame_stride, frame_uv_offset, projection);
  clFinish(work_queue_);
  return &y_plane_tile_cl_;
}

CabinFrameSampler::~CabinFrameSampler() {
  release_planar_tiles();
}
