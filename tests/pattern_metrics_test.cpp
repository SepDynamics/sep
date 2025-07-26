#include <gtest/gtest.h>
#include "quantum/pattern_metric_engine.h"

using namespace sep::quantum;

TEST(PatternMetrics, StabilityConstantSequence) {
    std::vector<float> data(8, 1.0f);
    float result = calculateStability(data, 1.0f);
    EXPECT_NEAR(result, 1.0f, 1e-5f);
}

TEST(PatternMetrics, EntropyUniformDistribution) {
    std::vector<float> data = {0.0f, 1.0f, 2.0f, 3.0f};
    float result = calculateEntropy(data);
    EXPECT_NEAR(result, 1.0f, 0.01f);
}

