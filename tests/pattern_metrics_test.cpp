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

TEST(PatternMetrics, EnergyComputation)
{
    PatternMetricEngine engine;
    engine.clear();

    sep::compat::PatternData p;
    std::strncpy(p.id, "p2", sizeof(p.id) - 1);
    p.data = {1.0f, 2.0f, 3.0f};

    engine.addPattern(p);
    const auto& metrics = engine.computeMetrics();

    ASSERT_EQ(metrics.size(), 1u);
    EXPECT_NEAR(metrics[0].energy, 14.0f, 1e-5f); // 1^2 + 2^2 + 3^2
}

TEST(PatternMetrics, EmptyPatternMetrics)
{
    PatternMetricEngine engine;
    engine.clear();

    sep::compat::PatternData p;
    std::strncpy(p.id, "empty", sizeof(p.id) - 1);

    engine.addPattern(p);
    const auto& metrics = engine.computeMetrics();

    ASSERT_EQ(metrics.size(), 1u);
    EXPECT_EQ(metrics[0].energy, 0.0f);
    EXPECT_EQ(metrics[0].coherence, 0.0f);
    EXPECT_EQ(metrics[0].stability, 0.0f);
    EXPECT_NEAR(metrics[0].entropy, 0.5f, 1e-5f);
}

TEST(PatternMetrics, AggregateMetrics)
{
    PatternMetricEngine engine;
    engine.clear();

    sep::compat::PatternData p1;
    std::strncpy(p1.id, "p1", sizeof(p1.id) - 1);
    p1.data = {1.0f, 1.0f};

    sep::compat::PatternData p2;
    std::strncpy(p2.id, "p2", sizeof(p2.id) - 1);
    p2.data = {2.0f, 2.0f};

    engine.addPattern(p1);
    engine.addPattern(p2);

    const auto& metrics = engine.computeMetrics();
    auto agg = engine.computeAggregateMetrics();

    float avg_coh = (metrics[0].coherence + metrics[1].coherence) / 2.0f;
    float avg_stab = (metrics[0].stability + metrics[1].stability) / 2.0f;
    float avg_ent = (metrics[0].entropy + metrics[1].entropy) / 2.0f;

    EXPECT_NEAR(agg.average_coherence, avg_coh, 1e-5f);
    EXPECT_NEAR(agg.average_stability, avg_stab, 1e-5f);
    EXPECT_NEAR(agg.average_entropy, avg_ent, 1e-5f);
}
