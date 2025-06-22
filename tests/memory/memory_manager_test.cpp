#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

using namespace sep::memory;

TEST(MemoryManager, BasicSTMAllocation) {
    MemoryTierManager mgr;
    MemoryBlock* blk = mgr.allocate(128, sep::memory::TierType::STM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
}

TEST(MemoryManager, BasicMTMAllocation) {
    MemoryTierManager mgr;
    MemoryBlock* blk = mgr.allocate(256, sep::memory::TierType::MTM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
}

TEST(MemoryManager, BasicLTMAllocation) {
    MemoryTierManager mgr;
    MemoryBlock* blk = mgr.allocate(512, sep::memory::TierType::LTM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);
}

TEST(MemoryManager, MultipleAllocations) {
    MemoryTierManager mgr;
    MemoryBlock* s = mgr.allocate(64, sep::memory::TierType::STM);
    MemoryBlock* m = mgr.allocate(128, sep::memory::TierType::MTM);
    MemoryBlock* l = mgr.allocate(256, sep::memory::TierType::LTM);
    ASSERT_NE(s, nullptr);
    ASSERT_NE(m, nullptr);
    ASSERT_NE(l, nullptr);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);
    mgr.deallocate(s);
    mgr.deallocate(m);
    mgr.deallocate(l);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);
}
