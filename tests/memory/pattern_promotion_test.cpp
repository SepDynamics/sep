#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

TEST(MemoryTierManagerPromotion, PromoteDemote) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryBlock* blk = mgr.allocate(128, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    blk = mgr.updateBlockMetrics(blk, 0.8f, 0.8f, 10, 1.0f);
    ASSERT_NE(blk, nullptr);
    ASSERT_EQ(blk->tier, sep::memory::MemoryTierEnum::MTM);

    blk = mgr.updateBlockMetrics(blk, 0.2f, 0.2f, 10, 1.0f);
    ASSERT_NE(blk, nullptr);
    ASSERT_EQ(blk->tier, sep::memory::MemoryTierEnum::STM);
}
