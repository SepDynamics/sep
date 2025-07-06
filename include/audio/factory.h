#pragma once
#include <memory>

namespace sep {
namespace audio {

class AudioCapture;

std::unique_ptr<AudioCapture> createAudioCapture();

} // namespace audio
} // namespace sep

