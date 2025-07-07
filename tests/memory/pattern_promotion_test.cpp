#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

using namespace sep::memory;

TEST(MemoryTierManagerPromotion, PromoteDemote) {
    MemoryTierManager mgr;
    MemoryBlock* blk = mgr.allocate(128, MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    mgr.updateBlockMetrics(blk, 0.8f, 0.8f, 10, 1.0f);
    MemoryBlock* promoted = mgr.findBlockByPtr(blk->ptr);
    ASSERT_EQ(promoted->tier, MemoryTierEnum::MTM);

    mgr.updateBlockMetrics(promoted, 0.2f, 0.2f, 10, 1.0f);
    MemoryBlock* demoted = mgr.findBlockByPtr(promoted->ptr);
    ASSERT_EQ(demoted->tier, MemoryTierEnum::STM);
}
