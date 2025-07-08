#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

TEST(MemoryTierManagerPromotion, PromoteDemote) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryBlock* blk = mgr.allocate(128, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    sep::memory::MemoryBlock* promoted = mgr.updateBlockMetrics(blk, 0.8f, 0.8f, 10, 1.0f);
    ASSERT_NE(promoted, nullptr);
    EXPECT_EQ(promoted->tier, sep::memory::MemoryTierEnum::MTM);

    sep::memory::MemoryBlock* demoted = mgr.updateBlockMetrics(promoted, 0.2f, 0.2f, 10, 1.0f);
    ASSERT_NE(demoted, nullptr);
    EXPECT_EQ(demoted->tier, sep::memory::MemoryTierEnum::STM);
}
