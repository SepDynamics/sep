#include <gtest/gtest.h>
#include "audio/pipewire_capture.h"

#ifdef SEP_HAS_AUDIO

using namespace sep::audio;

TEST(PipeWireCaptureTest, MetricsAccess) {
    PipeWireCapture cap;
    AudioMetrics metrics = cap.getMetrics();
    EXPECT_EQ(metrics.total_samples, 0u);
}

TEST(PipeWireCaptureTest, StartWithoutInit) {
    PipeWireCapture cap;
    EXPECT_EQ(cap.start(), AudioError::INIT_FAILED);
}

TEST(PipeWireCaptureTest, StopWithoutInit) {
    PipeWireCapture cap;
    EXPECT_EQ(cap.stop(), AudioError::NONE);
}

TEST(PipeWireCaptureTest, InitStartStopSequence) {
    PipeWireCapture cap;
    AudioConfig cfg{};
    AudioError err = cap.init(cfg);
    if (err == AudioError::NONE) {
        EXPECT_EQ(cap.start(), AudioError::NONE);
        EXPECT_EQ(cap.stop(), AudioError::NONE);
    } else {
        EXPECT_EQ(err, AudioError::INIT_FAILED);
    }
}

#endif // SEP_HAS_AUDIO
