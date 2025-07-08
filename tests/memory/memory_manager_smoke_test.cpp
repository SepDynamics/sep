#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

namespace mem = sep::memory;

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryManagerSmokeTest, AllocateAndFree) {
    sep::memory::MemoryTierManager manager;
    sep::memory::MemoryBlock* block = manager.allocate(64, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_GT(manager.getTierUtilization(sep::memory::MemoryTierEnum::STM), 0.0f);
    manager.deallocate(block);
    EXPECT_NEAR(manager.getTierUtilization(sep::memory::MemoryTierEnum::STM), 0.0f, EPSILON);
}
