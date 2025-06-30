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
namespace blender {
class CyclesRenderer; // Forward declaration if header not available
}
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture();
#if SEP_HAS_BLENDER
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer();
#else
inline std::unique_ptr<SEPBlenderBridge> createBlenderBridge() { return nullptr; }
inline std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() { return nullptr; }
#endif

} // namespace compat
} // namespace sep
