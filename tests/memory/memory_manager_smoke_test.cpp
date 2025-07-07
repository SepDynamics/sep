#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

using namespace sep;

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryManagerSmokeTest, AllocateAndFree) {
    memory::MemoryTierManager manager;
    memory::MemoryBlock* block = manager.allocate(64, memory::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_GT(manager.getTierUtilization(memory::MemoryTierEnum::STM), 0.0f);
    manager.deallocate(block);
    EXPECT_NEAR(manager.getTierUtilization(memory::MemoryTierEnum::STM), 0.0f, EPSILON);
}
