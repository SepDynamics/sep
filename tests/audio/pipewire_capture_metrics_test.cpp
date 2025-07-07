#include <gtest/gtest.h>
#include "audio/pipewire_capture.h"
#include <chrono>
#include <thread>

#ifdef SEP_HAS_AUDIO

using namespace sep::audio;

TEST(PipeWireCaptureMetrics, StartStopUpdatesMetrics) {
    PipeWireCapture cap;
    AudioConfig cfg{};
    auto initErr = cap.init(cfg);
    if (initErr != AudioError::NONE) {
        GTEST_SKIP() << "PipeWire unavailable";
    }
    
    ASSERT_EQ(cap.start(), AudioError::NONE);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_GT(cap.getMetrics().total_samples, 0u);
    EXPECT_EQ(cap.stop(), AudioError::NONE);
}

#endif // SEP_HAS_AUDIO
