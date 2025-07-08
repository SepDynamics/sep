#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"
#include "memory/memory_tier.hpp"

using namespace sep::memory;

static MemoryBlock* findAllocated(MemoryTier& tier) {
    for (const auto& b : tier.getBlocks()) {
        if (b.allocated) return const_cast<MemoryBlock*>(&b);
    }
    return nullptr;
}

TEST(MemoryPromotionRegression, StablePromotionDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(256, ::sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);

    mgr.updateBlockMetrics(block, 0.8f, 0.8f, 6, 1.0f); // promote to MTM
    MemoryBlock* promoted = findAllocated(mgr.getMTM());
    ASSERT_NE(promoted, nullptr);
    float weight = promoted->weight;
    uint64_t wait = promoted->wait;

    mgr.updateBlockMetrics(promoted, 0.8f, 0.8f, 6, 1.0f); // should remain MTM
    EXPECT_EQ(promoted->tier, ::sep::memory::MemoryTierEnum::MTM);
    EXPECT_FLOAT_EQ(weight, promoted->weight);
    EXPECT_EQ(wait, promoted->wait);

    mgr.updateBlockMetrics(promoted, 0.1f, 0.1f, 6, 1.0f); // demote to STM
    MemoryBlock* demoted = findAllocated(mgr.getSTM());
    ASSERT_NE(demoted, nullptr);
    weight = demoted->weight;
    wait = demoted->wait;
    mgr.updateBlockMetrics(demoted, 0.1f, 0.1f, 6, 1.0f); // remain STM
    EXPECT_EQ(demoted->tier, ::sep::memory::MemoryTierEnum::STM);
    EXPECT_FLOAT_EQ(weight, demoted->weight);
    EXPECT_EQ(wait, demoted->wait);
}

