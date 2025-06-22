#include <gtest/gtest.h>
#include "api/sep_engine.h"
#include <nlohmann/json.hpp>

using namespace sep::api;

TEST(SepEngineDeterminism, ValidateContextsDeterministic) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));

    nlohmann::json req = { {"contexts", {"test input"}} };

    auto r1 = engine.validateContexts(req);
    auto r2 = engine.validateContexts(req);

    EXPECT_EQ(r1, r2);
}

TEST(SepEngineDeterminism, ProcessPatternsDeterministic) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));

    nlohmann::json pattern = { {"pattern_data", {0.1f, 0.2f, 0.3f}} };

    auto r1 = engine.processPatterns(pattern);
    auto r2 = engine.processPatterns(pattern);

    EXPECT_EQ(r1, r2);
}

TEST(SepEngineDeterminism, MemoryMetricsDeterministic) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));

    auto r1 = SepEngine::getMemoryMetrics();
    auto r2 = SepEngine::getMemoryMetrics();

    EXPECT_EQ(r1, r2);
}

TEST(SepEngineDeterminism, CalculateSimilarityDeterministic) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));

    nlohmann::json req = {
        {"embedding1", {0.1, 0.2, 0.3}},
        {"embedding2", {0.1, 0.2, 0.3}}
    };

    auto r1 = engine.calculateSimilarity(req);
    auto r2 = engine.calculateSimilarity(req);

    EXPECT_EQ(r1, r2);
}
