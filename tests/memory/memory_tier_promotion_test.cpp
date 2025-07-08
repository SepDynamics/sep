#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>


namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryTierManagerPromotion, DemoteLTMtoMTM) {
    sep::memory::MemoryTierManager mgr;
    sep::memory::MemoryBlock* block = mgr.allocate(1024, sep::memory::MemoryTierEnum::LTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.0f, 0.0f, 0, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(sep::memory::MemoryTierEnum::LTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::MemoryTierEnum::MTM), 0.0f);
}
