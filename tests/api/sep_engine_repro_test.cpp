#include <gtest/gtest.h>
#include "api/sep_engine.h"
#include <nlohmann/json.hpp>

using namespace sep::api;

TEST(SepEngineDeterminism, RepeatedInitialization) {
    auto& engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize());

    nlohmann::json req = {{"context", "stable"}};
    auto out1 = engine.processContext(req);
    engine.shutdown();

    ASSERT_TRUE(engine.initialize());
    auto out2 = engine.processContext(req);

    EXPECT_EQ(out1, out2);
    engine.shutdown();
}

