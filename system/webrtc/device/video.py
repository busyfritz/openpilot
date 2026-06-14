import asyncio
import logging
import time

import av
from teleoprtc.tracks import TiciVideoStreamTrack

from cereal import messaging
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL, DT_DMON


class LiveStreamVideoStreamTrack(TiciVideoStreamTrack):
  livestream_camera_to_sock_mapping = {
    "driver": "livestreamDriverEncodeData",
    "wideRoad": "livestreamWideRoadEncodeData",
    "road": "livestreamRoadEncodeData",
  }
  main_camera_to_sock_mapping = {
    "driver": "driverEncodeData",
    "wideRoad": "wideRoadEncodeData",
    "road": "roadEncodeData",
  }

  def __init__(self, camera_type: str):
    dt = DT_DMON if camera_type == "driver" else DT_MDL
    super().__init__(camera_type, dt)

    self._params = Params()
    self._camera_type = camera_type
    self._candidate_topics = [
      self.main_camera_to_sock_mapping[camera_type],
      self.livestream_camera_to_sock_mapping[camera_type],
    ]
    # Avoid conflating at the socket level: dropping keyframes can cause the decoder to never start
    # (resulting in a "connected but black" stream in some browsers).
    self._socks = {topic: messaging.sub_sock(topic, conflate=False) for topic in self._candidate_topics}
    self._active_topic = self._preferred_topics()[0]
    self._pts = 0
    self._cached_header: bytes = b""
    self._sent_keyframe = False
    self._frame_count = 0
    self._last_frame_time = 0.0
    self._last_preference_refresh = 0.0
    self._logger = logging.getLogger("LiveStreamVideoStreamTrack")

  def _preferred_topics(self) -> list[str]:
    # WebRTC currently forces H.264. The dedicated livestream topics are the H.264 feeds,
    # while the main encode topics are the full-resolution HEVC recordings. Prefer the
    # livestream feeds both onroad and offroad, and keep the main topics only as fallback.
    return [
      self.livestream_camera_to_sock_mapping[self._camera_type],
      self.main_camera_to_sock_mapping[self._camera_type],
    ]

  def _reset_decoder_state(self, topic: str) -> None:
    if topic == self._active_topic:
      return
    self._logger.info("[%s] switching video source from %s to %s", self._id, self._active_topic, topic)
    self._active_topic = topic
    self._cached_header = b""
    self._sent_keyframe = False

  def _is_keyframe(self, data: bytes) -> bool:
    """Check if H.264 NAL unit contains an IDR keyframe (NAL type 5)."""
    i = 0
    while i < len(data) - 4:
      # Look for Annex B start codes: 0x000001 or 0x00000001
      if data[i:i+3] == b'\x00\x00\x01':
        nal_type = data[i+3] & 0x1f
        if nal_type == 5:  # IDR slice
          return True
        i += 3
      elif data[i:i+4] == b'\x00\x00\x00\x01':
        nal_type = data[i+4] & 0x1f
        if nal_type == 5:  # IDR slice
          return True
        i += 4
      else:
        i += 1
    return False

  async def recv(self):
    while True:
      now = time.monotonic()
      if now - self._last_preference_refresh > 0.5:
        preferred_topics = self._preferred_topics()
        self._last_preference_refresh = now
      else:
        preferred_topics = [self._active_topic, *[t for t in self._candidate_topics if t != self._active_topic]]

      msg = None
      msg_topic = None
      for topic in preferred_topics:
        maybe_msg = messaging.recv_one_or_none(self._socks[topic])
        if maybe_msg is not None:
          msg = maybe_msg
          msg_topic = topic
          break

      if msg is not None and msg_topic is not None:
        self._reset_decoder_state(msg_topic)
        self._last_frame_time = now
        break

      await asyncio.sleep(0.005)

    evta = getattr(msg, msg.which())

    header = bytes(evta.header)
    data = bytes(evta.data)
    self._frame_count += 1

    # Cache SPS/PPS header when it arrives
    if header:
      self._cached_header = header
      self._logger.debug(f"[{self._id}] cached SPS/PPS header ({len(header)} bytes)")

    # CRITICAL: Cannot decode without SPS/PPS. Wait for it.
    if not self._cached_header:
      self._logger.debug(f"[{self._id}] frame {self._frame_count}: no SPS/PPS yet, skipping")
      return await self.recv()

    is_keyframe = self._is_keyframe(data)

    # Wait for first keyframe before sending any frames
    # Browser decoder needs IDR to initialize properly
    if not self._sent_keyframe:
      if not is_keyframe:
        self._logger.debug(f"[{self._id}] frame {self._frame_count}: waiting for keyframe")
        return await self.recv()
      self._sent_keyframe = True
      self._logger.info(f"[{self._id}] first keyframe received, starting stream")

    # Prepend SPS/PPS header to keyframes (required by some decoders)
    # For non-keyframes, header is optional but safe to include
    if is_keyframe:
      payload = self._cached_header + data
    else:
      payload = data

    packet = av.Packet(payload)
    packet.time_base = self._time_base
    packet.pts = self._pts
    packet.dts = self._pts
    packet.duration = int(self._dt * self._clock_rate)

    if is_keyframe:
      packet.is_keyframe = True

    self.log_debug("track sending frame %s (keyframe=%s, size=%d)", self._pts, is_keyframe, len(payload))
    self._pts += int(self._dt * self._clock_rate)

    return packet

  def codec_preference(self) -> str | None:
    return "H264"
