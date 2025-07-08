#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

TEST(MemoryTierManagerPromotion, PromoteDemote) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryBlock* blk = mgr.allocate(128, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    mgr.updateBlockMetrics(blk, 0.8f, 0.8f, 10, 1.0f);
    sep::memory::MemoryBlock* promoted = mgr.findBlockByPtr(blk->ptr);
    ASSERT_EQ(promoted->tier, sep::memory::MemoryTierEnum::MTM);

    mgr.updateBlockMetrics(promoted, 0.2f, 0.2f, 10, 1.0f);
    sep::memory::MemoryBlock* demoted = mgr.findBlockByPtr(promoted->ptr);
    ASSERT_EQ(demoted->tier, sep::memory::MemoryTierEnum::STM);
}
