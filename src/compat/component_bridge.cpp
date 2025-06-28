#include "compat/component_bridge.h"
#include "audio/pipewire_capture.h"
#include "audio/pipewire_stubs.h"
#include "blender/bridge.h"
#include "blender/cycles_renderer.h"

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
std::shared_ptr<pattern::BlenderBridge> createBlenderBridge() {
    return std::make_shared<pattern::BlenderBridge>();
}

std::unique_ptr<blender::CyclesRenderer> createCyclesRenderer() {
    return std::make_unique<blender::CyclesRenderer>();
}
} // namespace compat
} // namespace sep
