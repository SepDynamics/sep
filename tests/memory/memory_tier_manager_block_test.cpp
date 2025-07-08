#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

TEST(MemoryTierManagerBlockTest, PromoteAndDemoteBlock) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryBlock* blk = mgr.allocate(256, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);

    sep::memory::MemoryBlock* promoted = nullptr;
    EXPECT_EQ(mgr.promoteBlock(blk, promoted), sep::SEPResult::SUCCESS);
    ASSERT_NE(promoted, nullptr);
    EXPECT_EQ(promoted->tier, sep::memory::MemoryTierEnum::MTM);

    sep::memory::MemoryBlock* demoted = nullptr;
    EXPECT_EQ(mgr.demoteBlock(promoted, demoted), sep::SEPResult::SUCCESS);
    ASSERT_NE(demoted, nullptr);
    EXPECT_EQ(demoted->tier, sep::memory::MemoryTierEnum::STM);

    mgr.deallocate(demoted);
}
