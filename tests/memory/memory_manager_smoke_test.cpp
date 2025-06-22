#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

using namespace sep;

TEST(MemoryManagerSmokeTest, AllocateAndFree) {
    MemoryTierManager manager;
    MemoryBlock* block = manager.allocate(64, sep::memory::TierType::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_GT(manager.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    manager.deallocate(block);
    EXPECT_EQ(manager.getTierUtilization(sep::memory::TierType::STM), 0.0f);
}
