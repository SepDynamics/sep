#include "audio/pipeline.h"
#include <gtest/gtest.h>

TEST(AudioPipelineTest, ApplyHannWindowPreservesSize) {
    sep::audio::AudioPipeline pipeline(48000, 2);
    std::vector<float> samples(8, 1.0f);
    pipeline.applyHannWindow(samples);
    ASSERT_EQ(samples.size(), 8u);
}
