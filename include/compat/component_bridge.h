#pragma once
#include <memory>
#if SEP_HAS_BLENDER
#include "blender/types.h"
#else
struct SEPBlenderBridge;
#endif

namespace sep {
namespace audio {
class AudioCapture;
}
namespace pattern {
class BlenderBridge;
}  // namespace pattern
namespace blender {
class CyclesRenderer;
}  // namespace blender
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture();

#if SEP_HAS_BLENDER
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer();
#else
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer();
#endif

} // namespace compat
} // namespace sep
