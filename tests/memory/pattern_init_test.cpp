#include "quantum/data.hpp"
#include <gtest/gtest.h>

using namespace sep::pattern;

TEST(MemoryPattern, DefaultInitialization)
{
    PatternData p;

    EXPECT_TRUE(p.id.empty());
    EXPECT_EQ(p.generation, 0);
    EXPECT_EQ(p.position, glm::vec4(0.0f));
    EXPECT_EQ(p.velocity, glm::vec4(0.0f));
    EXPECT_EQ(p.attributes, glm::vec4(0.0f));
    EXPECT_EQ(p.amplitude, std::complex<float>(0.0f));
    EXPECT_EQ(p.state, ::sep::quantum::QuantumState::Status::SUPERPOSITION);
    EXPECT_FLOAT_EQ(p.phase, 0.0f);
    EXPECT_FLOAT_EQ(p.coherence, 0.0f);
    EXPECT_FLOAT_EQ(p.stability, 0.0f);
    EXPECT_FLOAT_EQ(p.entropy, 0.0f);
    EXPECT_FLOAT_EQ(p.mutation_rate, 0.0f);
    EXPECT_EQ(p.mutation_count, 0u);
    EXPECT_EQ(p.memory_tier, ::sep::memory::MemoryTierEnum::STM);
    EXPECT_TRUE(p.relationships.empty());
    EXPECT_TRUE(p.data.empty());
}
