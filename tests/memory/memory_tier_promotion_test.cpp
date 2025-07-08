#include "memory/memory_tier_manager.hpp"
#include <gtest/gtest.h>

namespace mem = sep::memory;

namespace {
constexpr float EPSILON = 1e-3f;
}

TEST(MemoryTierManagerPromotion, DemoteLTMtoMTM) {
    mem::MemoryTierManager mgr;
    mem::MemoryBlock* block = mgr.allocate(1024, mem::MemoryTierEnum::LTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.0f, 0.0f, 0, 1.0f);
    EXPECT_NEAR(mgr.getTierUtilization(mem::MemoryTierEnum::LTM), 0.0f, EPSILON);
    EXPECT_GT(mgr.getTierUtilization(mem::MemoryTierEnum::MTM), 0.0f);
}
