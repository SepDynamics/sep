#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

using namespace sep::quantum::mcp;
using nlohmann::json;

TEST(PatternDeterminism, ProcessPatternsConsistent) {
    PatternData in;
    in.id = "det_pat";
    in.position = sep::glm_impl::make_vec4(0.f);
    in.velocity = sep::glm_impl::make_vec4(0.f);
    in.attributes = sep::glm_impl::make_vec4(0.5f);
    std::vector<PatternData> input{in};
    sep::pattern::PatternConfig cfg{0.1f, false, 1, 1};
    std::vector<PatternData> out1;
    std::vector<PatternData> out2;
    ASSERT_EQ(PatternEvolution::processPatterns(input, cfg, out1), sep::pattern::PatternResult::SUCCESS);
    ASSERT_EQ(PatternEvolution::processPatterns(input, cfg, out2), sep::pattern::PatternResult::SUCCESS);
    ASSERT_EQ(out1.size(), out2.size());
    if (!out1.empty()) {
        EXPECT_EQ(PatternEvolution::toJson(out1[0]).dump(), PatternEvolution::toJson(out2[0]).dump());
    }
}

TEST(PatternDeterminism, JsonRoundTripStable) {
    json cfg = {{"quantum_state", {0,1,2,3}}, {"initial", true}};
    auto p = PatternEvolution::evolvePattern(cfg, "pat_round");
    auto j = PatternEvolution::toJson(p);
    auto r1 = PatternEvolution::fromJson(j);
    auto r2 = PatternEvolution::fromJson(j);
    EXPECT_EQ(PatternEvolution::toJson(r1).dump(), PatternEvolution::toJson(r2).dump());
}

