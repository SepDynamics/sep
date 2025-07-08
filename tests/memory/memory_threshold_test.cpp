#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"

namespace mem = sep::memory;

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryThresholdCompliance, STMtoMTM) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* block = mgr.allocate(512, mem::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.75f, 0.75f, 6, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f);
}

TEST(MemoryThresholdCompliance, MTMtoLTM) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* block = mgr.allocate(512, mem::MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.95f, 0.95f, 150, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f);
}

TEST(MemoryThresholdCompliance, DemotionBelowThreshold) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* block = mgr.allocate(512, mem::MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.2f, 0.5f, 10, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::STM), 0.0f);
}
