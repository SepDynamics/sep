#include <gtest/gtest.h>
#include "api/sep_engine.h"

using namespace sep::api;

TEST(SepEngineEmbeddings, DeterministicOutput) {
    auto &engine = SepEngine::getInstance();
    ASSERT_TRUE(engine.initialize({}));

    nlohmann::json req = {{"text", "hello"}};

    auto r1 = engine.extractEmbeddings(req);
    auto r2 = engine.extractEmbeddings(req);

    EXPECT_EQ(r1, r2);
}

