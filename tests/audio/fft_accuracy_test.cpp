#include <gtest/gtest.h>
#include "audio/pipeline.h"
#include <glm/gtc/constants.hpp>

using namespace sep::audio;

TEST(AudioPipelineFFT, AccurateSineFrequency) {
    const size_t sample_rate = 48000;
    AudioPipeline pipeline(sample_rate, 1);
    const float freq = 440.0f;
    for (int i = 0; i < 2048; ++i) {
        float sample = std::sin(2.0f * glm::pi<float>() * freq * i / sample_rate);
        pipeline.processAudioFrame(std::vector<float>{sample});
    }
    auto patterns = pipeline.getPatterns();
    ASSERT_FALSE(patterns.empty());
    float fundamental = patterns.front().x * (sample_rate / 2.0f);
    EXPECT_NEAR(fundamental, freq, 1.0f);
}
