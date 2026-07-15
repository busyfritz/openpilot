#pragma once

#include <zstd.h>

#include <sys/types.h>
#include <string>
#include <vector>
#include <capnp/common.h>

class ZstdFileWriter {
public:
  ZstdFileWriter(const std::string &filename, int compression_level);
  ~ZstdFileWriter();
  void write(void* data, size_t size);
  inline void write(kj::ArrayPtr<capnp::byte> array) { write(array.begin(), array.size()); }
  void durable_flush(bool force);

private:
  void flushCache(ZSTD_EndDirective mode);

  size_t input_cache_capacity_ = 0;
  std::vector<char> input_cache_;
  std::vector<char> output_buffer_;
  ZSTD_CStream *cstream_;
  FILE* file_ = nullptr;
  off_t wb_synced_ = 0;   // io_writeback.h cursors
  off_t wb_dropped_ = 0;
  double last_durable_ms_ = 0.;  // io_writeback.h durable-checkpoint cursor
};
