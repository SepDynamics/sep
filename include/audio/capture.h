#pragma once

#include <memory>
#include "audio/types.h"

namespace sep {
namespace audio {

// Forward declarations for non-audio builds
#ifndef SEP_HAS_AUDIO
class AudioCapture {
public:
  virtual ~AudioCapture() = default;
  static std::unique_ptr<AudioCapture> create();
protected:
  AudioCapture() = default;
};
#else
#include <functional>

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
  virtual SEPAudioMetrics getMetrics() const = 0;

  // Factory method to create PipeWire implementation
  static std::unique_ptr<AudioCapture> create();

protected:
  AudioCapture() = default;
};
#endif // SEP_HAS_AUDIO

} // namespace audio
} // namespace sep
