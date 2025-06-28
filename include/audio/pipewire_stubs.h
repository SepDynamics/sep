#pragma once

#include "audio/capture.h"
#include "audio/config.h"
#include <spdlog/spdlog.h>

namespace sep {
namespace audio {

class PipeWireCaptureStub : public AudioCapture {
 public:
  AudioError init(const AudioConfig& config) override {
    (void)config;
    spdlog::warn("PipeWire not available, using stub implementation");
    return AudioError::INIT_FAILED;
  }
  AudioError start() override { return AudioError::STREAM_FAILED; }
  AudioError stop() override { return AudioError::STREAM_FAILED; }
  void setCallback(AudioCallback) override {}
  AudioMetrics getMetrics() const override { return {}; }
};

} // namespace audio
} // namespace sep
