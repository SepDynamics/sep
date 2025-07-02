#include <gtest/gtest.h>
#include "api/sep_engine.h"
#include <nlohmann/json.hpp>

using namespace sep::api;

TEST(SepEngineContextValidation, ValidContexts) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));

    nlohmann::json req = {
        {"contexts", {{
            {"type", "message"},
            {"content", {{"text", "hello"}}},
            {"metadata", {{"timestamp", 1}}}
        }}}
    };

    auto res = engine.validateContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    EXPECT_EQ(res["context_count"].get<size_t>(), 1u);
}

TEST(SepEngineContextValidation, InvalidContextMissingType) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));
    nlohmann::json req = {
        {"contexts", {{
            {"content", {1,2,3}}
        }}}
    };
    auto res = engine.validateContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    EXPECT_FALSE(res["valid"].get<bool>());
    ASSERT_EQ(res["invalid_indices"].size(), 1u);
    EXPECT_EQ(res["invalid_indices"][0].get<size_t>(), 0u);
}

TEST(SepEngineContextValidation, InvalidContextBadTimestamp) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));
    nlohmann::json req = {
        {"contexts", {{
            {"type", "message"},
            {"content", {1,2,3}},
            {"metadata", {{"timestamp", "bad"}}}
        }}}
    };
    auto res = engine.validateContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    EXPECT_FALSE(res["valid"].get<bool>());
    ASSERT_EQ(res["invalid_indices"].size(), 1u);
    EXPECT_EQ(res["invalid_indices"][0].get<size_t>(), 0u);
}

TEST(SepEngineBlendContexts, BasicBlend) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));
    nlohmann::json req = {
        {"contexts", {
            {{"content", {0.0, 1.0}}, {"metadata", {{"timestamp", 1}}}},
            {{"content", {1.0, 0.0}}, {"metadata", {{"timestamp", 3}}}}
        }},
        {"weights", {0.25, 0.75}}
    };
    auto res = engine.blendContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    auto result = res["result"];
    auto emb    = result["embedding"].get<std::vector<double>>();
    ASSERT_EQ(emb.size(), 2u);
    EXPECT_NEAR(emb[0], 1.5, 1e-6);
    EXPECT_NEAR(emb[1], 0.25, 1e-6);
    EXPECT_NEAR(result["metadata"]["timestamp"].get<double>(), 2.5, 1e-6);
}

TEST(SepEngineBlendContexts, DimensionMismatch) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));
    nlohmann::json req = {
        {"contexts", {
            {{"content", {0.0, 1.0}}, {"metadata", {{"timestamp", 1}}}},
            {{"content", {1.0}}, {"metadata", {{"timestamp", 2}}}}
        }}
    };
    auto res = engine.blendContexts(req);
    EXPECT_FALSE(res["success"].get<bool>());
}
