#include <gtest/gtest.h>

#include "quantum/pattern_metric_engine.h"

using namespace sep::quantum;

TEST(PatternMetrics, StabilityConstantSequence)
{
    std::vector<float> data(8, 1.0f);
    float result = calculateStability(data, 1.0f);
    EXPECT_NEAR(result, 1.0f, 1e-5f);
}

TEST(PatternMetrics, EntropyUniformDistribution)
{
    std::vector<float> data = {0.0f, 1.0f, 2.0f, 3.0f};
    float result = calculateEntropy(data);
    EXPECT_NEAR(result, 1.0f, 0.01f);
}

TEST(PatternMetrics, LengthIsRecorded)
{
    PatternMetricEngine engine;
    engine.clear();

    // create simple pattern
    sep::compat::PatternData p;
    std::strncpy(p.id, "p1", sizeof(p.id) - 1);
    p.data = {1.0f, 2.0f, 3.0f, 4.0f};

    engine.addPattern(p);
    const auto& metrics = engine.computeMetrics();

    ASSERT_EQ(metrics.size(), 1u);
    EXPECT_EQ(metrics[0].length, 4u);
}
