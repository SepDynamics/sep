#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

namespace mem = sep::memory;

TEST(MemoryTierManagerBlockTest, PromoteAndDemoteBlock) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* blk = mgr.allocate(256, mem::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    mem::MemoryBlock* promoted = nullptr;
    EXPECT_EQ(mgr.promoteBlock(blk, promoted), sep::SEPResult::SUCCESS);
    ASSERT_NE(promoted, nullptr);
    EXPECT_EQ(promoted->tier, mem::MemoryTierEnum::MTM);

    mem::MemoryBlock* demoted = nullptr;
    EXPECT_EQ(mgr.demoteBlock(promoted, demoted), sep::SEPResult::SUCCESS);
    ASSERT_NE(demoted, nullptr);
    EXPECT_EQ(demoted->tier, mem::MemoryTierEnum::STM);

    mgr.deallocate(demoted);
}
