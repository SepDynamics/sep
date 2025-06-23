#include <gtest/gtest.h>
#include "audio/pipeline.h"
#include "compat/shim.h"
#include <cmath>
#include <vector>

using namespace sep::audio;

TEST(AudioPipelineTestbed, ExtractsFundamentalFrequency){
    AudioPipeline pipeline(48000,1);
    const float freq=1000.0f;
    for(int i=0;i<2048;i++){
        float sample = std::sinf(2.0f*M_PI*freq*(float)i/48000.0f);
        sep::shim::vector<float> frame{static_cast<size_t>(static_cast<float>(sample))};
        pipeline.processAudioFrame(sep::shim::to_std_vector(frame));
    }
    auto patterns = pipeline.getPatterns();
    ASSERT_FALSE(patterns.empty());
    float expected = freq/(48000.0f/2.0f);
    EXPECT_NEAR(patterns[0].x, expected, 0.05f);

}