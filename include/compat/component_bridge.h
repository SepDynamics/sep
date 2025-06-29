#pragma once
#include <memory>
#include "blender/types.h"

namespace sep {
namespace audio {
class AudioCapture;
}
namespace pattern {
class BlenderBridge;
}  // namespace pattern
// SEPBlenderBridge is fully defined in blender/types.h
namespace blender {
class CyclesRenderer; // Forward declaration if header not available
}
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture();
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer();

} // namespace compat
} // namespace sep
