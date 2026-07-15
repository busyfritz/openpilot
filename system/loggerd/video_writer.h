#pragma once

#include <string>
#include <deque>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
}

#include "cereal/messaging/messaging.h"

class VideoWriter {
public:
  VideoWriter(const char *path, const char *filename, bool remuxing, int width, int height, int fps, cereal::EncodeIndex::Type codec);
  void write(uint8_t *data, int len, long long timestamp, bool codecconfig, bool keyframe);
  void write_audio(uint8_t *data, int len, long long timestamp, int sample_rate);
  void durable_sync(bool force);

  ~VideoWriter();

private:
  void initialize_audio(int sample_rate);
  void encode_and_write_audio_frame(AVFrame* frame);
  void process_remaining_audio();

  std::string vid_path, lock_path, dir_path;
  FILE *of = nullptr;
  off_t wb_synced_ = 0;   // io_writeback.h cursors
  off_t wb_dropped_ = 0;
  int durable_fd = -1;          // remux path: second fd on vid_path for fdatasync
  double last_durable_ms = 0.;  // io_writeback.h durable-checkpoint cursor

  AVCodecContext *codec_ctx;
  AVFormatContext *ofmt_ctx;
  AVStream *out_stream;
  int out_stream_index = -1;

  bool audio_initialized = false;
  bool audio_failed = false;
  bool header_written = false;
  AVStream *audio_stream = nullptr;
  int audio_stream_index = -1;
  AVCodecContext *audio_codec_ctx = nullptr;
  AVFrame *audio_frame = nullptr;
  uint64_t audio_pts = 0;
  std::deque<float> audio_buffer;

  bool remuxing;
};
