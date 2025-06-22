#include <gtest/gtest.h>
#include "api/sep_engine.h"

TEST(OptionalReturn, InitializeTwice){
    auto& eng = sep::api::SepEngine::getInstance();
    sep::config::APIConfig cfg{};
    auto res1 = eng.initialize(cfg);
    auto res2 = eng.initialize(cfg);
    EXPECT_FALSE(res2["success"].get<bool>());
    eng.shutdown();
}
