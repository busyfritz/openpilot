// _GNU_SOURCE for sync_file_range() (io_writeback.h); must precede all includes
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "system/loggerd/zstd_writer.h"

#include <cassert>

#include "common/util.h"
#include "system/loggerd/io_writeback.h"

// Constructor: Initializes compression stream and opens file
ZstdFileWriter::ZstdFileWriter(const std::string& filename, int compression_level) {
  // Create the compression stream
  cstream_ = ZSTD_createCStream();
  assert(cstream_);

  size_t initResult = ZSTD_initCStream(cstream_, compression_level);
  assert(!ZSTD_isError(initResult));

  input_cache_capacity_ = ZSTD_CStreamInSize();
  input_cache_.reserve(input_cache_capacity_);
  output_buffer_.resize(ZSTD_CStreamOutSize());

  file_ = util::safe_fopen(filename.c_str(), "wb");
  assert(file_ != nullptr);
}

// Destructor: Finalizes compression and closes file
ZstdFileWriter::~ZstdFileWriter() {
  flushCache(ZSTD_e_end);
  util::safe_fflush(file_);
  durable_fsync(fileno(file_));

  int err = fclose(file_);
  assert(err == 0);

  ZSTD_freeCStream(cstream_);
}

// Compresses and writes data to file
void ZstdFileWriter::write(void* data, size_t size) {
  // Add data to the input cache
  input_cache_.insert(input_cache_.end(), (uint8_t*)data, (uint8_t*)data + size);

  // If the cache is full, compress and write to the file
  if (input_cache_.size() >= input_cache_capacity_) {
    flushCache(ZSTD_e_continue);
  }

  durable_flush(false);
}

// Durable checkpoint: end a zstd block (stream stays decodable up to here even
// if the file is truncated later), then commit it to storage.
void ZstdFileWriter::durable_flush(bool force) {
  double now = writeback_now_ms();
  if (!force && last_durable_ms_ != 0.0 && now - last_durable_ms_ < 2000.0) return;
  flushCache(ZSTD_e_flush);
  if (fflush(file_) == 0) durable_fsync(fileno(file_));
  last_durable_ms_ = now;
}

// Compress and flush the input cache to the file
void ZstdFileWriter::flushCache(ZSTD_EndDirective mode) {
  ZSTD_inBuffer input = {input_cache_.data(), input_cache_.size(), 0};
  int finished = 0;

  do {
    ZSTD_outBuffer output = {output_buffer_.data(), output_buffer_.size(), 0};
    size_t remaining = ZSTD_compressStream2(cstream_, &output, &input, mode);
    assert(!ZSTD_isError(remaining));

    size_t written = util::safe_fwrite(output_buffer_.data(), 1, output.pos, file_);
    assert(written == output.pos);

    finished = (mode == ZSTD_e_continue) ? (input.pos == input.size) : (remaining == 0);
  } while (!finished);

  input_cache_.clear();  // Clear cache after compression

  if (mode == ZSTD_e_continue) {
    stream_writeback(file_, wb_synced_, wb_dropped_);
  }
}
