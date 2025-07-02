#include <gtest/gtest.h>
#include "api/sep_engine.h"
#include <nlohmann/json.hpp>

using namespace sep::api;

class SepEngineContextOpsTest : public ::testing::Test {
protected:
    void SetUp() override { engine.initialize({}); }
    void TearDown() override { engine.shutdown(); }
    SepEngine& engine = SepEngine::getInstance();
};

TEST_F(SepEngineContextOpsTest, ValidateContextsSuccess) {
    nlohmann::json req = {
        {"contexts", {
            {{"type","message"}, {"content","hi"}, {"metadata",{{"timestamp","1"}}}},
            {{"type","message"}, {"content","bye"}, {"metadata",{{"timestamp","2"}}}}
        }}
    };
    auto res = engine.validateContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    EXPECT_TRUE(res["valid"].get<bool>());
    EXPECT_EQ(res["context_count"].get<size_t>(), 2u);
    EXPECT_TRUE(res["invalid_indices"].empty());
}

TEST_F(SepEngineContextOpsTest, ValidateContextsError) {
    nlohmann::json req = {
        {"contexts", {
            {{"type","message"}, {"content","hi"}, {"metadata",{{"timestamp","1"}}}},
            {{"type","message"}, {"content","bye"}}
        }}
    };
    auto res = engine.validateContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    EXPECT_FALSE(res["valid"].get<bool>());
    ASSERT_EQ(res["invalid_indices"].size(), 1u);
    EXPECT_EQ(res["invalid_indices"][0].get<size_t>(), 1u);
}

TEST_F(SepEngineContextOpsTest, BlendContextsSuccess) {
    nlohmann::json req = {
        {"contexts", {
            {0.0, 0.0, 0.0},
            {1.0, 1.0, 1.0}
        }},
        {"weights", {0.5, 0.5}}
    };
    auto res = engine.blendContexts(req);
    ASSERT_TRUE(res["success"].get<bool>());
    auto result = res["result"];
    auto embedding = result["embedding"].get<std::vector<double>>();
    ASSERT_EQ(embedding.size(), 3u);
    for (double v : embedding) {
        EXPECT_NEAR(v, 0.5, 1e-6);
    }
    double coherence = result["coherence"].get<double>();
    EXPECT_GT(coherence, 0.0);
    EXPECT_LE(coherence, 1.0);
}

TEST_F(SepEngineContextOpsTest, BlendContextsDimensionMismatch) {
    nlohmann::json req = {
        {"contexts", {
            {0.0, 0.0},
            {1.0, 1.0, 1.0}
        }},
        {"weights", {0.5, 0.5}}
    };
    auto res = engine.blendContexts(req);
    EXPECT_FALSE(res["success"].get<bool>());
}

