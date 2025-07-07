#include <gtest/gtest.h>
#include "audio/pipewire_capture.h"

#ifdef SEP_HAS_AUDIO

using namespace sep::audio;

TEST(PipeWireCaptureLifecycle, StartBeforeInitFails) {
    PipeWireCapture cap;
    EXPECT_EQ(cap.start(), AudioError::INIT_FAILED);
}

TEST(PipeWireCaptureLifecycle, InitAndStop) {
    PipeWireCapture cap;
    AudioConfig cfg;
    auto result = cap.init(cfg);
    EXPECT_NE(result, AudioError::INTERNAL_ERROR);
    EXPECT_EQ(cap.stop(), AudioError::NONE);
}

TEST(PipeWireCaptureLifecycle, CallbackSettable) {
    PipeWireCapture cap;
    bool called = false;
    cap.setCallback([&called](const float*, size_t){ called = true; });
    EXPECT_FALSE(called);
    EXPECT_EQ(cap.getMetrics().total_samples, 0u);
}

#endif // SEP_HAS_AUDIO
