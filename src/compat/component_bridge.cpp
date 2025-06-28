#include "compat/component_bridge.h"
#include "audio/pipewire_capture.h"
#include "blender/bridge.h"
#include "blender/cycles_renderer.h"

namespace sep {
namespace compat {

std::unique_ptr<audio::AudioCapture> createAudioCapture() {
    return std::make_unique<audio::PipeWireCapture>();
}
std::shared_ptr<pattern::BlenderBridge> createBlenderBridge() {
    return std::make_shared<pattern::BlenderBridge>();
}

std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() {
    return std::make_unique<blender::CyclesRenderer>();
}
} // namespace compat
} // namespace sep
