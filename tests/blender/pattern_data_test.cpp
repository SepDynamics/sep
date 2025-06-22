#include "quantum/types.h"

#include <gtest/gtest.h>

#include <cstring>
#include <vector>



// Use specific using declarations for the renamed struct
using sep::pattern::GPUPatternData;
using sep::pattern::PatternFlags;

class PatternDataTest : public ::testing::Test
{
protected:
    GPUPatternData createTestPattern()
    {
        GPUPatternData pattern;
        pattern.coherence      = 0.75f;
        pattern.entropy        = 0.5f;
        pattern.stability      = 0.9f;
        pattern.mutation_rate  = 0.1f;
        pattern.generation     = 42;
        pattern.mutations      = 10;
        pattern.promotions     = 2;
        pattern.flags          = 0x1;
        pattern.offset_in_tier = 1024;
        pattern.size_in_bytes  = 4096;
        pattern.position       = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        pattern.velocity       = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        pattern.attributes     = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        return pattern;
    }
};

TEST_F(PatternDataTest, SizeAndAlignment)
{
    // Verify size is 96 bytes
    EXPECT_EQ(96, sizeof(GPUPatternData));

    // Verify 16-byte alignment
    EXPECT_EQ(16, alignof(GPUPatternData));
}

TEST_F(PatternDataTest, MemoryLayout)
{
    // Verify field offsets
    EXPECT_EQ(0, offsetof(GPUPatternData, position));
    EXPECT_EQ(16, offsetof(GPUPatternData, velocity));
    EXPECT_EQ(32, offsetof(GPUPatternData, attributes));
    EXPECT_EQ(48, offsetof(GPUPatternData, coherence));
    EXPECT_EQ(52, offsetof(GPUPatternData, entropy));
    EXPECT_EQ(56, offsetof(GPUPatternData, stability));
    EXPECT_EQ(60, offsetof(GPUPatternData, mutation_rate));
    EXPECT_EQ(64, offsetof(GPUPatternData, generation));
    EXPECT_EQ(68, offsetof(GPUPatternData, mutations));
    EXPECT_EQ(72, offsetof(GPUPatternData, promotions));
    EXPECT_EQ(76, offsetof(GPUPatternData, flags));
    EXPECT_EQ(80, offsetof(GPUPatternData, offset_in_tier));
    EXPECT_EQ(88, offsetof(GPUPatternData, size_in_bytes));
}

TEST_F(PatternDataTest, VectorOperations)
{
    // Test vector operations
    GPUPatternData pattern = createTestPattern();

    // Position
    pattern.position = glm::vec4(1.0f, 2.0f, 3.0f, 1.0f);
    EXPECT_FLOAT_EQ(1.0f, pattern.position.x);
    EXPECT_FLOAT_EQ(2.0f, pattern.position.y);
    EXPECT_FLOAT_EQ(3.0f, pattern.position.z);
    EXPECT_FLOAT_EQ(1.0f, pattern.position.w);

    // Velocity
    pattern.velocity = glm::vec4(0.1f, 0.2f, 0.3f, 0.0f);
    EXPECT_FLOAT_EQ(0.1f, pattern.velocity.x);
    EXPECT_FLOAT_EQ(0.2f, pattern.velocity.y);
    EXPECT_FLOAT_EQ(0.3f, pattern.velocity.z);
    EXPECT_FLOAT_EQ(0.0f, pattern.velocity.w);
}

TEST_F(PatternDataTest, FlagOperations)
{
    GPUPatternData pattern = createTestPattern();

    // Test flag operations
    pattern.flags = 0;
    EXPECT_EQ(0, pattern.flags & static_cast<uint32_t>(PatternFlags::ACTIVE));

    // Set flag
    pattern.flags |= static_cast<uint32_t>(PatternFlags::ACTIVE);
    EXPECT_NE(0, pattern.flags & static_cast<uint32_t>(PatternFlags::ACTIVE));

    // Clear flag
    pattern.flags &= ~static_cast<uint32_t>(PatternFlags::ACTIVE);
    EXPECT_EQ(0, pattern.flags & static_cast<uint32_t>(PatternFlags::ACTIVE));
}

TEST_F(PatternDataTest, CopyAndMove)
{
    GPUPatternData original = createTestPattern();

    // Test copy constructor
    GPUPatternData copied(original);
    EXPECT_EQ(0, std::memcmp(&original, &copied, sizeof(GPUPatternData)));

    // Test move constructor
    GPUPatternData moved(std::move(copied));
    EXPECT_EQ(0, std::memcmp(&original, &moved, sizeof(GPUPatternData)));

    // Test copy assignment
    GPUPatternData assigned;
    assigned = original;
    EXPECT_EQ(0, std::memcmp(&original, &assigned, sizeof(GPUPatternData)));
}

TEST_F(PatternDataTest, ArrayOperations)
{
    constexpr size_t            COUNT = 1000;
    std::vector<GPUPatternData> patterns(COUNT);

    // Verify vector allocation alignment
    EXPECT_EQ(0, reinterpret_cast<uintptr_t>(patterns.data()) % alignof(GPUPatternData));

    // Test array operations
    for (size_t i = 0; i < COUNT; ++i)
    {
        patterns[i]            = createTestPattern();
        patterns[i].generation = i;
    }

    // Verify data
    for (size_t i = 0; i < COUNT; ++i)
    {
        EXPECT_EQ(i, patterns[i].generation);
        EXPECT_FLOAT_EQ(0.75f, patterns[i].coherence);
    }
}

TEST_F(PatternDataTest, Initialization)
{
    // Test default initialization
    GPUPatternData pattern{};  // zero-initialize to avoid uninitialized warnings
    EXPECT_FLOAT_EQ(0.0f, pattern.coherence);
    EXPECT_FLOAT_EQ(0.0f, pattern.entropy);
    EXPECT_FLOAT_EQ(0.0f, pattern.stability);
    EXPECT_FLOAT_EQ(0.0f, pattern.mutation_rate);
    EXPECT_EQ(0u, pattern.generation);
    EXPECT_EQ(0u, pattern.mutations);
    EXPECT_EQ(0u, pattern.promotions);
    EXPECT_EQ(0u, pattern.flags);
    EXPECT_EQ(0ull, pattern.offset_in_tier);
    EXPECT_EQ(0u, pattern.size_in_bytes);
}
