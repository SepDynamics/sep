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
    EXPECT_FALSE(res["success"].get<bool>());
}

TEST(SepEngineBlendContexts, BasicBlend) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));
    nlohmann::json req = {
        {"contexts", {
            {{"content", {0.0, 1.0}}},
            {{"content", {1.0, 0.0}}}
        }},
        {"weights", {0.5, 0.5}}
    };
    auto res = engine.blendContexts(req);
    EXPECT_TRUE(res["success"].get<bool>());
    auto coherence = res["result"]["coherence"].get<double>();
    EXPECT_GT(coherence, 0.0);
}

TEST(SepEngineBlendContexts, DimensionMismatch) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));
    nlohmann::json req = {
        {"contexts", {
            {{"content", {0.0, 1.0}}},
            {{"content", {1.0}}}
        }}
    };
    auto res = engine.blendContexts(req);
    EXPECT_FALSE(res["success"].get<bool>());
}
