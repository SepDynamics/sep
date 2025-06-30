#include <gtest/gtest.h>
#include "quantum/pattern_evolution.h"

using namespace sep::quantum::mcp;

TEST(PatternEvolutionProcess, UpdatesGenerationAndIds) {
    sep::pattern::PatternData p{};
    p.id = "orig";
    p.coherence = 0.5f;
    p.stability = 0.5f;
    std::vector<sep::pattern::PatternData> input{p};
    sep::pattern::PatternConfig cfg{};
    std::vector<sep::pattern::PatternData> output;
    ASSERT_EQ(PatternEvolution::processPatterns(input, cfg, output), sep::pattern::PatternResult::SUCCESS);
    ASSERT_EQ(output.size(), 1u);
    EXPECT_NE(output[0].id, p.id);
    EXPECT_EQ(output[0].generation, p.generation + 1);
}

