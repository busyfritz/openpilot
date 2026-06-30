// Copyright © IQ.Lvbs, apart of Project Teal Lvbs, All Rights Reserved, licensed under https://konn3kt.com/tos/
// clang++ -O2 repro.cc && ./a.out

#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace {

constexpr int kModelWidth = 320;
constexpr int kModelHeight = 640;
constexpr int kRetryWindow = 20;
constexpr int kSlowThresholdMs = 10;

double millis_since_boot() {
  timespec stamp{};
#ifdef CLOCK_BOOTTIME
  clock_gettime(CLOCK_BOOTTIME, &stamp);
#else
  clock_gettime(CLOCK_MONOTONIC, &stamp);
#endif
  return stamp.tv_sec * 1000.0 + stamp.tv_nsec * 1e-6;
}

inline float identity_input(uint8_t value) {
  return value;
}

void pack_monitoring_tensor(uint8_t *nv12_frame, float *tensor_out) {
  const int half_h = kModelHeight / 2;
  const int half_w = kModelWidth / 2;
  const int plane_area = half_w * half_h;
  const int uv_base = kModelWidth * kModelHeight;

  for (int row = 0; row < half_h; ++row) {
    for (int col = 0; col < half_w; ++col) {
      const int slot = col * half_h + row;
      const int y_row = row * 2;
      const int y_col = col * 2;

      tensor_out[slot] = identity_input(nv12_frame[(y_row * kModelWidth) + y_col]);
      tensor_out[slot + plane_area] = identity_input(nv12_frame[((y_row + 1) * kModelWidth) + y_col]);
      tensor_out[slot + (plane_area * 2)] = identity_input(nv12_frame[(y_row * kModelWidth) + y_col + 1]);
      tensor_out[slot + (plane_area * 3)] = identity_input(nv12_frame[((y_row + 1) * kModelWidth) + y_col + 1]);
      tensor_out[slot + (plane_area * 4)] = identity_input(nv12_frame[uv_base + (row * half_w) + col]);
      tensor_out[slot + (plane_area * 5)] = identity_input(nv12_frame[uv_base + plane_area + (row * half_w) + col]);
    }
  }
}

double average_runtime_ms(uint8_t *nv12_frame, float *tensor_out) {
  double total_ms = 0.0;
  for (int i = 0; i < kRetryWindow; ++i) {
    const double start_ms = millis_since_boot();
    pack_monitoring_tensor(nv12_frame, tensor_out);
    total_ms += millis_since_boot() - start_ms;
  }
  return total_ms / static_cast<double>(kRetryWindow);
}

void dump_stall_trace(uint8_t *nv12_frame, float *tensor_out) {
  for (int i = 0; i < 200; ++i) {
    const double start_ms = millis_since_boot();
    pack_monitoring_tensor(nv12_frame, tensor_out);
    printf("%.2f   ", millis_since_boot() - start_ms);
  }
  printf("\n");
}

}  // namespace

int main() {
  const size_t nv12_bytes = kModelWidth * kModelHeight * 3 / 2;
  const size_t tensor_floats = (kModelWidth / 2) * (kModelHeight / 2) * 6;

  while (true) {
    auto *nv12_frame = static_cast<uint8_t *>(malloc(nv12_bytes));
    auto *tensor_out = static_cast<float *>(malloc(tensor_floats * sizeof(float)));
    printf("allocate -- %p 0x%zx -- %p 0x%zx\n", nv12_frame, nv12_bytes, tensor_out, tensor_floats * sizeof(float));

    const double mean_ms = average_runtime_ms(nv12_frame, tensor_out);
    if (mean_ms > kSlowThresholdMs) {
      printf("HIT %.2f\n", mean_ms);
      printf("BAD\n");
      dump_stall_trace(nv12_frame, tensor_out);
      return 0;
    }

    printf("got %.2f\n", mean_ms);
  }
}
