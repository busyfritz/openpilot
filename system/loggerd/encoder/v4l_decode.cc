// Standalone hardware HEVC decoder for the offroad route viewer, built on comma's tested Venus
// (msm_vidc) decoder from tools/replay/qcom_decoder.cc. ffmpeg's generic hevc_v4l2m2m can't drive
// the downstream msm_vidc interface; MsmVidc has the exact firmware recipe (S_EXT_CTRLS
// STREAM_OUTPUT_MODE/DPB_COLOR_FORMAT, the msm_vidc PORT_SETTINGS_CHANGED event, ION USERPTR
// buffers). We demux the .hevc with libav, feed packets, and stream decoded NV12->rgb24 to stdout.
//
// Usage:  v4l_decode <input.hevc>
//   stdout: "DIM <w> <h>\n" once, then raw rgb24 frames (w*h*3 bytes each).
//   Offroad the decoder + GPU are idle (camerad/modeld stopped), so the viewer can use it.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "tools/replay/qcom_decoder.h"
#include "third_party/libyuv/include/libyuv.h"
#include "third_party/linux/include/msm_media_info.h"

extern "C" {
  #include <libavformat/avformat.h>
  #include <libavcodec/avcodec.h>
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <input.hevc>\n", argv[0]);
    return 2;
  }

  AVFormatContext *fmt = avformat_alloc_context();
  fmt->probesize = 10 * 1024 * 1024;  // raw .hevc needs a big probe to index all frames
  if (avformat_open_input(&fmt, argv[1], nullptr, nullptr) != 0 ||
      avformat_find_stream_info(fmt, nullptr) < 0) {
    fprintf(stderr, "v4l_decode: cannot open %s\n", argv[1]);
    return 1;
  }
  int vstream = av_find_best_stream(fmt, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
  if (vstream < 0) { fprintf(stderr, "v4l_decode: no video stream\n"); return 1; }
  AVCodecParameters *cp = fmt->streams[vstream]->codecpar;
  if (cp->codec_id != AV_CODEC_ID_HEVC) {
    fprintf(stderr, "v4l_decode: only HEVC supported (got codec %d)\n", cp->codec_id);
    return 3;
  }
  const int w = cp->width, h = cp->height;

  MsmVidc dec;
  if (!dec.init(VIDEO_DEVICE, w, h, V4L2_PIX_FMT_HEVC)) {
    fprintf(stderr, "v4l_decode: msm_vidc init failed\n");
    return 1;
  }
  dec.avctx = fmt;

  // Decoded-frame target buffer (Venus NV12 layout), matching how MsmVidc copies the capture plane.
  const size_t y_stride = VENUS_Y_STRIDE(COLOR_FMT_NV12, w);
  const size_t uv_offset = y_stride * h;
  VisionBuf out;
  out.allocate(uv_offset + y_stride * ((h + 1) / 2));
  out.init_yuv(w, h, y_stride, uv_offset);

  std::vector<uint8_t> argb((size_t)w * h * 4), rgb((size_t)w * h * 3);
  // stdout carries only raw rgb24 frames (so this is a drop-in for the ffmpeg rgb pipe); dims go
  // to stderr. The consumer already knows WxH from ffprobe.
  fprintf(stderr, "v4l_decode: HW decode %dx%d y_stride=%zu\n", w, h, y_stride);

  AVPacket pkt;
  int npkt = 0, nframe = 0, rr;
  while ((rr = av_read_frame(fmt, &pkt)) == 0) {
    if (pkt.stream_index == vstream && pkt.size > 0) {
      npkt++;
      VisionBuf *res = dec.decodeFrame(&pkt, &out);
      if (res != nullptr) {
        nframe++;
        libyuv::NV12ToARGB(out.y, out.stride, out.uv, out.stride, argb.data(), w * 4, w, h);
        // ARGBToRAW gives R,G,B byte order (matches the R8G8B8 texture / ffmpeg rgb24). ARGBToRGB24
        // would give B,G,R -> the blue tint / swapped red-blue.
        libyuv::ARGBToRAW(argb.data(), w * 4, rgb.data(), w * 3, w, h);
        if (fwrite(rgb.data(), 1, rgb.size(), stdout) != rgb.size()) {
          av_packet_unref(&pkt);
          break;  // downstream (player) closed the pipe
        }
      }
    }
    av_packet_unref(&pkt);
  }
  fprintf(stderr, "v4l_decode: done rr=%d pkts=%d frames=%d\n", rr, npkt, nframe);

  fflush(stdout);
  avformat_close_input(&fmt);
  return 0;
}
