#pragma once

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>

// Bounded write-behind for loggerd's append-only FILE* (rlog/qlog/video): async
// writeback of new bytes (sync_file_range) + drop the now-clean prior range
// (posix_fadvise DONTNEED) so dirty pages don't pile up and force reclaim stalls.
// Never fsyncs, never drops dirty pages. Linux-only, no-op elsewhere, best-effort.
// Disable: LOGGERD_NO_WRITEBACK_TUNING=1. synced/dropped are caller-owned cursors.
static inline void stream_writeback(FILE *f, off_t &synced, off_t &dropped,
                                    off_t chunk = (off_t)4 << 20) {
#if defined(__linux__)
  static const bool disabled = getenv("LOGGERD_NO_WRITEBACK_TUNING") != nullptr;
  if (disabled || f == nullptr) return;

  off_t pos = ftello(f);
  if (pos < 0 || pos - synced < chunk) return;

  if (fflush(f) != 0) return;
  int fd = fileno(f);
  sync_file_range(fd, synced, pos - synced, SYNC_FILE_RANGE_WRITE);
  if (synced > dropped) {
    posix_fadvise(fd, dropped, synced - dropped, POSIX_FADV_DONTNEED);
    dropped = synced;
  }
  synced = pos;
#else
  (void)f; (void)synced; (void)dropped; (void)chunk;
#endif
}

static inline double writeback_now_ms() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

static inline int durable_fsync(int fd) {
#if defined(__APPLE__)
  return fsync(fd);
#else
  return fdatasync(fd);
#endif
}

// Durable checkpoint on a caller-owned time cursor: at most once per interval_ms
// (always when force), commit data AND inode size through the journal so every
// byte written before the checkpoint survives a power cut. last_ms == 0 means
// never synced. Returns true if a sync was issued.
static inline bool fd_durable(int fd, double &last_ms, bool force, double interval_ms = 2000.0) {
  if (fd < 0) return false;
  double now = writeback_now_ms();
  if (!force && last_ms != 0.0 && now - last_ms < interval_ms) return false;
  durable_fsync(fd);
  last_ms = now;
  return true;
}

// FILE* variant: flush stdio buffers first.
static inline bool stream_durable(FILE *f, double &last_ms, bool force, double interval_ms = 2000.0) {
  if (f == nullptr) return false;
  double now = writeback_now_ms();
  if (!force && last_ms != 0.0 && now - last_ms < interval_ms) return false;
  if (fflush(f) != 0) return false;
  durable_fsync(fileno(f));
  last_ms = now;
  return true;
}

// fsync a directory so entries for newly created files survive a power cut.
static inline void fsync_dir(const char *path) {
  int fd = open(path, O_RDONLY);
  if (fd >= 0) {
    fsync(fd);
    close(fd);
  }
}
