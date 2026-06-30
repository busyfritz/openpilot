/*
Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
*/
#pragma once

#include <cassert>
#include <cfloat>
#include <cstdlib>
#include <memory>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "common/mat.h"
#include "iqpilot/selfdrive/iqmodeld/transforms/warp_geometry.h"
#include "iqpilot/selfdrive/iqmodeld/transforms/yuv.h"

class FrameCropperBase {
public:
  FrameCropperBase(cl_device_id device_id, cl_context context);
  virtual ~FrameCropperBase();
  virtual cl_mem *project_to_cl(cl_mem yuv_cl, int frame_width, int frame_height, int frame_stride, int frame_uv_offset, const mat3 &projection) = 0;

  uint8_t *copy_to_host(cl_mem *source_frames, int buffer_size) {
    CL_CHECK(clEnqueueReadBuffer(work_queue_, *source_frames, CL_TRUE, 0, buffer_size, staging_bytes_.get(), 0, nullptr, nullptr));
    clFinish(work_queue_);
    return &staging_bytes_[0];
  }

  int buf_size;

protected:
  cl_command_queue work_queue_;
  std::unique_ptr<uint8_t[]> staging_bytes_;
  cl_mem y_plane_tile_cl_;
  cl_mem u_plane_tile_cl_;
  cl_mem v_plane_tile_cl_;
  WarpSamplerState sampler_state_;

  void configure_planar_tiles(cl_device_id device_id, cl_context context, int output_width, int output_height);
  void release_planar_tiles();
  void project_frame(cl_mem yuv_cl, int output_width, int output_height,
                     int frame_width, int frame_height, int frame_stride, int frame_uv_offset,
                     const mat3 &projection);
};

class RoadHistoryAssembler : public FrameCropperBase {
public:
  RoadHistoryAssembler(cl_device_id device_id, cl_context context, uint8_t history_slots);
  ~RoadHistoryAssembler() override;
  cl_mem *project_to_cl(cl_mem yuv_cl, int frame_width, int frame_height, int frame_stride, int frame_uv_offset, const mat3 &projection) override;

  static constexpr int kOutputWidth = 512;
  static constexpr int kOutputHeight = 256;
  static constexpr int kFrameBytes = kOutputWidth * kOutputHeight * 3 / 2;
  static constexpr int kExportBytes = kFrameBytes * 2;

private:
  PackedFrameKernels packer_;
  cl_mem timeline_cl_;
  cl_mem latest_slot_cl_;
  cl_mem publish_pair_cl_;
  cl_buffer_region latest_region_;
  size_t frame_bytes_;
  uint8_t history_slots_;
};

class CabinFrameSampler : public FrameCropperBase {
public:
  CabinFrameSampler(cl_device_id device_id, cl_context context);
  ~CabinFrameSampler() override;
  cl_mem *project_to_cl(cl_mem yuv_cl, int frame_width, int frame_height, int frame_stride, int frame_uv_offset, const mat3 &projection) override;

  static constexpr int kOutputWidth = 1440;
  static constexpr int kOutputHeight = 960;
  static constexpr int kExportBytes = kOutputWidth * kOutputHeight;
};
