#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryThresholdCompliance, STMtoMTM) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 2048;
    cfg.mtm_size = 4096;
    mgr.resetForTesting(cfg);
    sep::memory::MemoryBlock* block = mgr.allocate(512, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.75f, 0.75f, 6, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(sep::memory::MemoryTierEnum::STM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::MemoryTierEnum::MTM), 0.0f);
}

TEST(MemoryThresholdCompliance, MTMtoLTM) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryTierManager::Config cfg;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 8192;
    mgr.resetForTesting(cfg);
    sep::memory::MemoryBlock* block = mgr.allocate(512, sep::memory::MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.95f, 0.95f, 150, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(sep::memory::MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::MemoryTierEnum::LTM), 0.0f);
}

TEST(MemoryThresholdCompliance, DemotionBelowThreshold) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryTierManager::Config cfg;
    cfg.stm_size = 2048;
    cfg.mtm_size = 4096;
    mgr.resetForTesting(cfg);
    sep::memory::MemoryBlock* block = mgr.allocate(512, sep::memory::MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.2f, 0.5f, 10, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(sep::memory::MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::MemoryTierEnum::STM), 0.0f);
}
