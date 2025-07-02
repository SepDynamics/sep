#include <gtest/gtest.h>
#include "audio/pipewire_capture.h"
#include <chrono>
#include <thread>

using namespace sep::audio;

TEST(PipeWireCaptureIntegration, MetricsIncrease)
{
    PipeWireCapture cap;
    AudioConfig cfg{};
    auto initErr = cap.init(cfg);
    if (initErr != AudioError::NONE) {
        GTEST_SKIP() << "PipeWire unavailable";
    }
    ASSERT_EQ(cap.start(), AudioError::NONE);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    auto metrics = cap.getMetrics();
    EXPECT_GT(metrics.total_samples, 0u);
    EXPECT_GT(metrics.peak_level, 0.0f);
    EXPECT_GT(metrics.rms_level, 0.0f);
    EXPECT_EQ(cap.stop(), AudioError::NONE);
}
