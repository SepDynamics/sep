#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

using namespace sep::quantum::mcp;

TEST(PatternEvolutionRegression, DeterministicOutput) {
    nlohmann::json cfg = {{"quantum_state", {0,1,2,3}}, {"amplitudes", {0.25,0.25,0.25,0.25}}, {"initial", true}};
    auto p1 = PatternEvolution::evolvePattern(cfg, "pat_id");
    auto p2 = PatternEvolution::evolvePattern(cfg, "pat_id");
    EXPECT_EQ(PatternEvolution::toJson(p1).dump(), PatternEvolution::toJson(p2).dump());
}
