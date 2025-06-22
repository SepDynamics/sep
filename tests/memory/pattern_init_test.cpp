#include "memory/types.h"
#include <gtest/gtest.h>

using namespace sep::memory;

TEST(MemoryPattern, DefaultInitialization)
{
    Pattern p;

    EXPECT_EQ(p.id, 0u);
    EXPECT_FLOAT_EQ(p.data.x, 0.0f);
    EXPECT_FLOAT_EQ(p.data.y, 0.0f);
    EXPECT_FLOAT_EQ(p.data.z, 0.0f);
    EXPECT_FLOAT_EQ(p.coherence, 0.0f);
    EXPECT_FLOAT_EQ(p.stability_score, 0.0f);
    EXPECT_EQ(p.generation_count, 0);
    EXPECT_FLOAT_EQ(p.access_frequency, 0.0f);
    EXPECT_EQ(p.memory_tier, sep::memory::TierType::STM);
    EXPECT_TRUE(p.relationships.empty());
    EXPECT_TRUE(p.mutation_history.empty());
}
