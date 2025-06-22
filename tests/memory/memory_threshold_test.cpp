#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

using namespace sep::memory;

TEST(MemoryThresholdCompliance, STMtoMTM) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(512, sep::memory::TierType::STM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.75f, 0.75f, 6, 1.0f);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
}

TEST(MemoryThresholdCompliance, MTMtoLTM) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(512, sep::memory::TierType::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.95f, 0.95f, 150, 1.0f);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);
}

TEST(MemoryThresholdCompliance, DemotionBelowThreshold) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(512, sep::memory::TierType::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.2f, 0.5f, 10, 1.0f);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
}
