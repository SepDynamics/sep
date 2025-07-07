#include <gtest/gtest.h>
#include "quantum/pattern_evolution.h"
#include <nlohmann/json.hpp>

TEST(PatternEvolutionRegression, DeterministicOutput) {
    nlohmann::json cfg = {{"quantum_state", {0,1,2,3}}, {"amplitudes", {0.25,0.25,0.25,0.25}}, {"initial", true}};
    auto p1 = sep::quantum::mcp::PatternEvolution::evolvePattern(cfg, "pat_id");
    auto p2 = sep::quantum::mcp::PatternEvolution::evolvePattern(cfg, "pat_id");
    EXPECT_EQ(sep::quantum::mcp::PatternEvolution::toJson(p1).dump(), sep::quantum::mcp::PatternEvolution::toJson(p2).dump());
}
