#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

namespace {
constexpr float EPSILON = 1e-3f;
}

namespace mem = sep::memory;


TEST(MemoryManager, BasicSTMAllocation) {
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    sep::memory::MemoryTierManager mgr(cfg);
    sep::memory::MemoryBlock* blk = mgr.allocate(128, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f, EPSILON);
}

TEST(MemoryManager, BasicMTMAllocation) {
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    sep::memory::MemoryTierManager mgr(cfg);
    sep::memory::MemoryBlock* blk = mgr.allocate(256, sep::memory::MemoryTierEnum::MTM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f, EPSILON);
}

TEST(MemoryManager, BasicLTMAllocation) {
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    sep::memory::MemoryTierManager mgr(cfg);
    sep::memory::MemoryBlock* blk = mgr.allocate(512, sep::memory::MemoryTierEnum::LTM);
    ASSERT_NE(blk, nullptr);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f);
    mgr.deallocate(blk);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f, EPSILON);
}

TEST(MemoryManager, MultipleAllocations) {
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    sep::memory::MemoryTierManager mgr(cfg);
    sep::memory::MemoryBlock* s = mgr.allocate(64, sep::memory::MemoryTierEnum::STM);
    sep::memory::MemoryBlock* m = mgr.allocate(128, sep::memory::MemoryTierEnum::MTM);
    sep::memory::MemoryBlock* l = mgr.allocate(256, sep::memory::MemoryTierEnum::LTM);
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
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    sep::memory::MemoryTierManager mgr(cfg);
    EXPECT_EQ(mgr.getTotalAllocated(), 0u);
    sep::memory::MemoryBlock* a = mgr.allocate(64, sep::memory::MemoryTierEnum::STM);
    sep::memory::MemoryBlock* b = mgr.allocate(128, sep::memory::MemoryTierEnum::MTM);
    ASSERT_NE(a, nullptr);
    ASSERT_NE(b, nullptr);
    EXPECT_GT(mgr.getTotalAllocated(), 0u);
    mgr.deallocate(a);
    mgr.deallocate(b);
    EXPECT_EQ(mgr.getTotalAllocated(), 0u);
}
