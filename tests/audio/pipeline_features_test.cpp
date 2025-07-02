#include <gtest/gtest.h>
#include "audio/pipeline.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

using namespace sep::audio;

TEST(AudioPipelineTest, SineWaveFeatureExtraction) {
    const size_t sample_rate = 48000;
    AudioPipeline pipeline(sample_rate, 1);

    const float frequency = 440.0f; // A4
    for (int i = 0; i < 2048; ++i) {
        float sample = std::sin(2.0f * glm::pi<float>() * frequency * i / sample_rate);
        pipeline.processAudioFrame(std::vector<float>{sample});
    }

    auto patterns = pipeline.getPatterns();
    ASSERT_FALSE(patterns.empty());
    glm::vec3 p = patterns.front();
    float fundamental = p.x * (sample_rate / 2.0f);
    EXPECT_NEAR(fundamental, frequency, 10.0f); // Allow some tolerance due to simplified FFT
}
