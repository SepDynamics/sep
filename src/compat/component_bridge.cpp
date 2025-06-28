#include "compat/component_bridge.h"

#if defined(SEP_HAS_PIPEWIRE) && SEP_HAS_PIPEWIRE
#include "audio/pipewire_capture.h"
#endif

#if defined(SEP_HAS_BLENDER) && SEP_HAS_BLENDER
#include "blender/bridge.h"
#endif

#if defined(SEP_HAS_CYCLES) && SEP_HAS_CYCLES
#include "blender/cycles_renderer.h"
#endif

namespace sep {
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture() {
#if defined(SEP_HAS_PIPEWIRE) && SEP_HAS_PIPEWIRE
    return std::make_unique<audio::PipeWireCapture>();
#else
    return nullptr;
#endif
}

std::shared_ptr<pattern::BlenderBridge> createBlenderBridge() {
#if defined(SEP_HAS_BLENDER) && SEP_HAS_BLENDER
    return std::make_shared<pattern::BlenderBridge>();
#else
    return nullptr;
#endif
}

std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() {
#if defined(SEP_HAS_CYCLES) && SEP_HAS_CYCLES
    return std::make_unique<blender::CyclesRenderer>();
#else
    return nullptr;
#endif
}

} // namespace compat
} // namespace sep
