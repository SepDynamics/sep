#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

using namespace sep::memory;

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryThresholdCompliance, STMtoMTM) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(512, MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.75f, 0.75f, 6, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(MemoryTierEnum::STM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
}

TEST(MemoryThresholdCompliance, MTMtoLTM) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(512, MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.95f, 0.95f, 150, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::LTM), 0.0f);
}

TEST(MemoryThresholdCompliance, DemotionBelowThreshold) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(512, MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.2f, 0.5f, 10, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::STM), 0.0f);
}
