#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

namespace mem = sep::memory;

TEST(MemoryTierManagerPromotion, PromoteDemote) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* blk = mgr.allocate(128, mem::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    mgr.updateBlockMetrics(blk, 0.8f, 0.8f, 10, 1.0f);
    mem::MemoryBlock* promoted = mgr.findBlockByPtr(blk->ptr);
    ASSERT_EQ(promoted->tier, mem::MemoryTierEnum::MTM);

    mgr.updateBlockMetrics(promoted, 0.2f, 0.2f, 10, 1.0f);
    mem::MemoryBlock* demoted = mgr.findBlockByPtr(promoted->ptr);
    ASSERT_EQ(demoted->tier, mem::MemoryTierEnum::STM);
}
