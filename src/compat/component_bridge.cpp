#include "compat/component_bridge.h"
#include "audio/pipewire_capture.h"
#include "audio/pipewire_stubs.h"
#include "blender/bridge.h"
#include "blender/api.h"
#include "blender/cycles_renderer.h"
#include "blender/types.h"

namespace sep {
namespace compat {


std::unique_ptr<audio::AudioCapture> createAudioCapture() {
#ifdef SEP_HAS_PIPEWIRE
    return std::make_unique<audio::PipeWireCapture>();
#else
    return std::make_unique<audio::PipeWireCaptureStub>();
#endif
}

std::unique_ptr<audio::AudioCapture> createAudioCaptureStub() { // Fix: Add missing stub definition
    return std::make_unique<audio::PipeWireCaptureStub>();
}
std::unique_ptr<SEPBlenderBridge> createBlenderBridge() {
    auto bridge = std::make_unique<SEPBlenderBridge>();
    bridge->impl = std::make_shared<pattern::BlenderBridge>();
    return bridge;
}

std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() {
    return std::make_unique<blender::CyclesRenderer>();
}
} // namespace compat
} // namespace sep
