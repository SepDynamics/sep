#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

namespace {
constexpr float EPSILON = 1e-3f;
}

namespace mem = sep::memory;

TEST(MemoryManager, BasicSTMAllocation) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* blk = mgr.allocate(128, mem::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f, EPSILON);
}

TEST(MemoryManager, BasicMTMAllocation) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* blk = mgr.allocate(256, mem::MemoryTierEnum::MTM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f, EPSILON);
}

TEST(MemoryManager, BasicLTMAllocation) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* blk = mgr.allocate(512, mem::MemoryTierEnum::LTM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f, EPSILON);
}

TEST(MemoryManager, MultipleAllocations) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* s = mgr.allocate(64, mem::MemoryTierEnum::STM);
    mem::MemoryBlock* m = mgr.allocate(128, mem::MemoryTierEnum::MTM);
    mem::MemoryBlock* l = mgr.allocate(256, mem::MemoryTierEnum::LTM);
    ASSERT_NE(s, nullptr);
    ASSERT_NE(m, nullptr);
    ASSERT_NE(l, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f);
    mgr.deallocate(s);
    mgr.deallocate(m);
    mgr.deallocate(l);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f, EPSILON);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f, EPSILON);
}

TEST(MemoryManager, TotalAllocatedMemory) {
    mem::MemoryTierManager mgr;
    EXPECT_EQ(mgr.getTotalAllocated(), 0u);
    mem::MemoryBlock* a = mgr.allocate(64, mem::MemoryTierEnum::STM);
    mem::MemoryBlock* b = mgr.allocate(128, mem::MemoryTierEnum::MTM);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_GT(mgr.getTotalAllocated(), 0u);
    mgr.deallocate(a);
    mgr.deallocate(b);
    EXPECT_EQ(mgr.getTotalAllocated(), 0u);
}
