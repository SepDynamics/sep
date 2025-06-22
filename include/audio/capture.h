#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "types.h"

namespace sep {
namespace audio {

class AudioCapture {
 public:
  using AudioCallback = std::function<void(const float*, size_t)>;

  virtual ~AudioCapture() = default;

  // Initialize audio capture with given configuration
  virtual AudioError init(const AudioConfig& config) = 0;

  // Start capturing audio
  virtual AudioError start() = 0;

  // Stop capturing audio
  virtual AudioError stop() = 0;

  // Set callback for audio data
  virtual void setCallback(AudioCallback callback) = 0;

  // Get current metrics
  virtual AudioMetrics getMetrics() const = 0;

  // Factory method to create PipeWire implementation
  static std::unique_ptr<AudioCapture> create();

 protected:
  AudioCapture() = default;
};

}  // namespace audio
}  // namespace sep
