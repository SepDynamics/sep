#pragma once

#include <cstdint>
#include <string>

namespace sep {
namespace audio {

struct AudioConfig {
  std::string source = "alsa_input.usb-TC-Helicon_GoXLR-00.HiFi__Headset__source";
  std::string description = "GoXLR Chat Mic";
  uint32_t rate = 16000;
  uint32_t channels = 1;
  uint32_t buffer_frames = 2048;
  uint32_t period_size = 512;
  bool auto_connect = true;
  bool rt_process = true;
};

struct AudioMetrics {
  uint64_t total_samples = 0;
  uint64_t dropped_samples = 0;
  float peak_level = 0.0f;
  float rms_level = 0.0f;
  uint32_t xruns = 0;
  float latency_ms = 0.0f;
};

enum class AudioError {
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