#include "audio/capture.h"

#ifdef SEP_HAS_AUDIO
#include "audio/pipewire_capture.h"
#endif

namespace sep {
namespace audio {

std::unique_ptr<AudioCapture> createAudioCapture() {
#ifdef SEP_HAS_AUDIO
    return std::make_unique<PipeWireCapture>();
#else
    return nullptr;
#endif
}

} // namespace audio
} // namespace sep
