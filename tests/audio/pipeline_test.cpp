#include <gtest/gtest.h>
#include "audio/pipeline.h"
#include "sep/shim.h"

using namespace sep::audio;

TEST(AudioPipelineTest, GeneratesPatterns) {
    AudioPipeline pipeline(48000, 1);
    sep::shim::vector<float> frame(1, 0.0f);
    for (int i=0; i<2048; ++i)
        pipeline.processAudioFrame(frame);
    auto patterns = pipeline.getPatterns();
    EXPECT_FALSE(patterns.empty());
}
