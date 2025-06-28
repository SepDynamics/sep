#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

using namespace sep::memory;

TEST(MemoryTierManagerBlockTest, PromoteAndDemoteBlock) {
    MemoryTierManager mgr;
    MemoryBlock* blk = mgr.allocate(256, TierType::STM);
    ASSERT_NE(blk, nullptr);

    MemoryBlock* promoted = nullptr;
    EXPECT_EQ(mgr.promoteBlock(blk, promoted), sep::SEPResult::SUCCESS);
    ASSERT_NE(promoted, nullptr);
    EXPECT_EQ(promoted->tier, TierType::MTM);

    MemoryBlock* demoted = nullptr;
    EXPECT_EQ(mgr.demoteBlock(promoted, demoted), sep::SEPResult::SUCCESS);
    ASSERT_NE(demoted, nullptr);
    EXPECT_EQ(demoted->tier, TierType::STM);

    mgr.deallocate(demoted);
}
