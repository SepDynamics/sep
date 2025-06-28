#pragma once

#include "audio/capture.h"
#include "audio/types.h"

namespace sep {
namespace audio {

class PipeWireCaptureStub : public AudioCapture {
public:
    PipeWireCaptureStub() = default;
    ~PipeWireCaptureStub() override = default;

    AudioError init(const AudioConfig&) override { return AudioError::INIT_FAILED; }
    AudioError start() override { return AudioError::STREAM_FAILED; }
    AudioError stop() override { return AudioError::STREAM_FAILED; }
    void setCallback(AudioCallback) override {}
    AudioMetrics getMetrics() const override { return {}; }
};

} // namespace audio
} // namespace sep
