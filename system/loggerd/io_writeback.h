#pragma once

#include <cstdio>
#include <cstdlib>
#include <sys/types.h>

#if defined(__linux__)
#include <fcntl.h>
#endif

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
