#include "blender/types.h"  // Must come first for SEPBlenderBridge definition
#include "compat/component_bridge.h"
#include "audio/pipewire_capture.h"
#if SEP_HAS_BLENDER
#include "blender/bridge.h"
#include "blender/api.h"
#include "blender/cycles_renderer.h"
#endif

namespace sep {
namespace compat {


std::unique_ptr<audio::AudioCapture> createAudioCapture() {
    return std::make_unique<audio::PipeWireCapture>();
}

#if SEP_HAS_BLENDER
std::unique_ptr<SEPBlenderBridge> createBlenderBridge() {
    auto bridge = std::make_unique<SEPBlenderBridge>();
    bridge->impl = std::make_shared<pattern::BlenderBridge>();
    return bridge;
}

std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() {
    return std::make_unique<blender::CyclesRenderer>();
}
#else
std::unique_ptr<SEPBlenderBridge> createBlenderBridge() {
    return nullptr;
}

std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() {
    return nullptr;
}
#endif
} // namespace compat
} // namespace sep
