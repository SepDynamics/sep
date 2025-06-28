#pragma once
#include <memory>

namespace sep {
namespace audio {
class AudioCapture;
}
namespace pattern {
class BlenderBridge;
}
struct SEPBlenderBridge;
namespace blender {
class CyclesRenderer; // Forward declaration if header not available
}
struct SEPBlenderBridge;
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture();
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer();

} // namespace compat
} // namespace sep
