#pragma once
#include "audio/capture.h"

namespace sep {
namespace audio {

class PipeWireCaptureStub : public AudioCapture {
public:
    AudioError init(const AudioConfig&) override { return AudioError::INIT_FAILED; }
    AudioError start() override { return AudioError::STREAM_FAILED; }
    AudioError stop() override { return AudioError::NONE; }
    void setCallback(AudioCallback) override {}
    AudioMetrics getMetrics() const override { return {}; }
};

} // namespace audio
} // namespace sep
