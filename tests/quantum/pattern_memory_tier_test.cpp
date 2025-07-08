#include <gtest/gtest.h>
#include "quantum/pattern_evolution.h"
#include "quantum/types.h"

using namespace sep::quantum::mcp;

TEST(PatternEvolution, MemoryTierAssignment) {
    sep::pattern::PatternData in;
    in.coherence = 0.95f;
    in.stability = 0.9f;
    in.generation = 100;
    std::vector<sep::pattern::PatternData> out;
    sep::pattern::PatternConfig cfg{};
    ASSERT_EQ(PatternEvolution::processPatterns({in}, cfg, out), sep::pattern::PatternResult::SUCCESS);
    ASSERT_EQ(out.size(), 1u);
    EXPECT_EQ(out[0].memory_tier, ::sep::memory::MemoryTierEnum::LTM);
}
