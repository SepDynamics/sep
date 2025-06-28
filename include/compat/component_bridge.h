#pragma once
#include <memory>

namespace sep {
namespace audio {
class AudioCapture;
}
namespace pattern {
class BlenderBridge;
}
namespace blender {
class CyclesRenderer; // Forward declaration if header not available
}
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture();
std::shared_ptr<pattern::BlenderBridge> createBlenderBridge();
std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer();

} // namespace compat
} // namespace sep
