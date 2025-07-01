#include "memory/memory_tier_manager.h"
#include <gtest/gtest.h>

using namespace sep::memory;

TEST(MemoryTierManagerPromotion, DemoteLTMtoMTM) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, TierType::LTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.0f, 0.0f, 0, 1.0f);
    EXPECT_EQ(mgr.getTierUtilization(TierType::LTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(TierType::MTM), 0.0f);
}
