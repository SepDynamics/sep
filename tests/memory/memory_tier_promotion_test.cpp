#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

using namespace sep::memory;

TEST(MemoryTierManagerPromotion, DemoteLTMtoMTM) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, MemoryTierEnum::LTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.0f, 0.0f, 0, 1.0f);
    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::LTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
}
