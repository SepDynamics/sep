#include "audio/pipewire_includes.h"
#include "audio/types.h"
#include "audio/capture.h"
#include <memory>

#ifdef SEP_HAS_AUDIO
#include "audio/pipewire_capture.h"
#endif

namespace sep {
namespace audio {

std::unique_ptr<AudioCapture> AudioCapture::create() {
#ifdef SEP_HAS_AUDIO
    return std::make_unique<PipeWireCapture>();
#else
    return nullptr;
#endif
}

// Standalone factory function used by executables
std::unique_ptr<AudioCapture> createAudioCapture() {
    return AudioCapture::create();
}

} // namespace audio
} // namespace sep
