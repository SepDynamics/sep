#pragma once

#ifdef SEP_HAS_AUDIO
#include <cstdint>
#include <string>
#include "audio/export.h"

namespace sep {
namespace audio {

struct SEP_AUDIO_API AudioConfig {
  std::string source = "alsa_input.usb-TC-Helicon_GoXLR-00.HiFi__Headset__source";
  std::string description = "GoXLR Chat Mic";
  uint32_t rate = 16000;
  uint32_t channels = 1;
  uint32_t buffer_frames = 2048;
  uint32_t period_size = 512;
  bool auto_connect = true;
  bool rt_process = true;
};

struct SEP_AUDIO_API AudioMetrics {
  uint64_t total_samples = 0;
  uint64_t dropped_samples = 0;
  float peak_level = 0.0f;
  float rms_level = 0.0f;
  uint32_t xruns = 0;
  float latency_ms = 0.0f;
};

enum SEP_AUDIO_API AudioError {
  NONE,
  INIT_FAILED,
  STREAM_FAILED,
  FORMAT_MISMATCH,
  BUFFER_OVERFLOW,
  BUFFER_UNDERFLOW,
  DEVICE_DISCONNECTED,
  INTERNAL_ERROR
};

}  // namespace audio
}  // namespace sep
#endif // SEP_HAS_AUDIO