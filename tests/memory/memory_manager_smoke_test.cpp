#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

namespace mem = sep::memory;

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryManagerSmokeTest, AllocateAndFree) {
    mem::MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    mem::MemoryTierManager manager(cfg);
    mem::MemoryBlock* block = manager.allocate(64, mem::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_GT(manager.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f);
    manager.deallocate(block);
    EXPECT_NEAR(manager.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f, EPSILON);
}
