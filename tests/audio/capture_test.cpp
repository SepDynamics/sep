#include <gtest/gtest.h>
#include "audio/pipewire_capture.h"

using namespace sep::audio;

TEST(PipeWireCaptureTest, MetricsAccess) {
    PipeWireCapture cap;
    AudioMetrics metrics = cap.getMetrics();
    EXPECT_EQ(metrics.total_samples, 0u);
}
