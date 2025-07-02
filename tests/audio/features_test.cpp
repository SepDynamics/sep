#include <gtest/gtest.h>
#include "audio/pipeline.h"
#include <vector>
#include <cmath>

using namespace sep::audio;

TEST(AudioPipelineTestbed, ExtractsFundamentalFrequency){
    AudioPipeline pipeline(48000,1);
    const float freq=1000.0f;
    for(int i=0;i<2048;i++){
        float sample = std::sinf(2.0f*M_PI*freq*(float)i/48000.0f);
        std::vector<float> frame{static_cast<float>(sample)};
        pipeline.processAudioFrame(frame);
    }
    auto patterns = pipeline.getPatterns();
    ASSERT_FALSE(patterns.empty());
    float expected = freq/(48000.0f/2.0f);
    EXPECT_NEAR(patterns[0].x, expected, 0.05f);

}
