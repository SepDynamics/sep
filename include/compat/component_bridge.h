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
namespace ccl {
class CyclesRenderer;
}  // namespace ccl
}  // namespace blender
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture();
std::unique_ptr<SEPBlenderBridge> createBlenderBridge();
std::unique_ptr<blender::ccl::CyclesRenderer> createCyclesRenderer();


} // namespace compat
} // namespace sep
