#include "audio/capture.h"
#include "audio/pipewire_capture.h"

namespace sep {
namespace audio {

std::unique_ptr<AudioCapture> AudioCapture::create() {
    return std::make_unique<PipeWireCapture>();
}

} // namespace audio
} // namespace sep