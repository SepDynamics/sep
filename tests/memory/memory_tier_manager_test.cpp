#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"
#include "memory/memory_tier.hpp"

using namespace sep::memory;
using sep::MemoryTierEnum;

TEST(MemoryTierManagerTest, BasicInitialization) {
    MemoryTierManager mgr;
    EXPECT_NE(nullptr, mgr.getTier(MemoryTierEnum::STM));
    EXPECT_NE(nullptr, mgr.getTier(MemoryTierEnum::MTM));
    EXPECT_NE(nullptr, mgr.getTier(MemoryTierEnum::LTM));
}

TEST(MemoryTierManagerTest, AllocationAndDeallocation) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::STM), 0.0f);
    mgr.deallocate(block);
    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::STM), 0.0f);
}

TEST(MemoryTierManagerTest, PromotionAndDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.9f, 0.9f, 6, 1.0f); // trigger promotion

    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::LTM), 0.0f);

    MemoryBlock* promoted = nullptr;
    for (const auto& b : mgr.getTier(MemoryTierEnum::LTM)->getBlocks()) {
        if (b.allocated) {
            promoted = const_cast<MemoryBlock*>(&b);
            break;
        }
    }
    ASSERT_NE(promoted, nullptr);
    mgr.updateBlockMetrics(promoted, 0.0f, 0.0f, 0, 1.0f); // trigger demotion

    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::LTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
}

TEST(MemoryTierManagerTest, DefragmentationTriggersPromotionDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.95f, 0.95f, 100, 1.0f); // should be promoted to LTM after defrag
    mgr.defragmentTier(MemoryTierEnum::MTM);

    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::LTM), 0.0f);

    MemoryBlock* promoted = nullptr;
    for (const auto& b : mgr.getTier(MemoryTierEnum::LTM)->getBlocks()) {
        if (b.allocated) {
            promoted = const_cast<MemoryBlock*>(&b);
            break;
        }
    }
    ASSERT_NE(promoted, nullptr);
    mgr.updateBlockMetrics(promoted, 0.0f, 0.0f, 0, 1.0f); // should be demoted to MTM
    mgr.defragmentTier(MemoryTierEnum::LTM);

    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::LTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
}



TEST(MemoryTierManagerTest, OptimizeBlocksPromotionDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);

    mgr.updateBlockMetrics(block, 0.9f, 0.9f, 6, 1.0f); // expect promotion
    mgr.optimizeBlocks();

    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::STM), 0.0f);

    MemoryBlock* promoted = nullptr;
    for (const auto& b : mgr.getTier(MemoryTierEnum::STM)->getBlocks()) {
        if (b.allocated) {
            promoted = const_cast<MemoryBlock*>(&b);
            break;
        }
    }
    ASSERT_NE(promoted, nullptr);
    mgr.updateBlockMetrics(promoted, 0.0f, 0.0f, 0, 1.0f); // expect demotion
    mgr.optimizeBlocks();

    EXPECT_EQ(mgr.getTierUtilization(MemoryTierEnum::STM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(MemoryTierEnum::MTM), 0.0f);
}

TEST(MemoryTierManagerTest, AllocationNearDefragmentBoundary) {
    MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    cfg.use_unified_memory = true;
    cfg.enable_compression = false;

    MemoryTierManager mgr(cfg);

    MemoryBlock* block1 = mgr.allocate(2048, MemoryTierEnum::STM);
    ASSERT_NE(block1, nullptr);
    MemoryBlock* block2 = mgr.allocate(1024, MemoryTierEnum::STM);
    ASSERT_NE(block2, nullptr);
    MemoryBlock* block3 = mgr.allocate(512, MemoryTierEnum::STM);
    ASSERT_NE(block3, nullptr);

    mgr.deallocate(block2);

    MemoryBlock* block4 = mgr.allocate(1536, MemoryTierEnum::STM);
    ASSERT_NE(block4, nullptr);

    EXPECT_TRUE(block1->allocated);
    bool found = false;
    for (auto& b : mgr.getTier(MemoryTierEnum::STM)->getBlocks()) {
        if (&b == block1) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);

    mgr.deallocate(block1);
    mgr.deallocate(block3);
    mgr.deallocate(block4);
}

TEST(MemoryTierManagerTest, AutoDefragmentationThreshold) {
    MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    cfg.fragmentation_threshold = 0.1f;

    MemoryTierManager mgr(cfg);

    MemoryBlock* b1 = mgr.allocate(1024, MemoryTierEnum::STM);
    ASSERT_NE(b1, nullptr);
    MemoryBlock* b2 = mgr.allocate(1024, MemoryTierEnum::STM);
    ASSERT_NE(b2, nullptr);

    mgr.deallocate(b1);

    float frag_before = mgr.getTierFragmentation(MemoryTierEnum::STM);
    EXPECT_GT(frag_before, cfg.fragmentation_threshold);

    mgr.optimizeTiers();

    float frag_after = mgr.getTierFragmentation(MemoryTierEnum::STM);
    EXPECT_LT(frag_after, frag_before);

    mgr.deallocate(b2);
}

TEST(MemoryTierManagerTest, TotalMetrics) {
    MemoryTierManager mgr;
    MemoryBlock* a = mgr.allocate(128, MemoryTierEnum::STM);
    MemoryBlock* b = mgr.allocate(128, MemoryTierEnum::MTM);
    MemoryBlock* c = mgr.allocate(128, MemoryTierEnum::LTM);

    float util = mgr.getTotalUtilization();
    EXPECT_GT(util, 0.0f);
    EXPECT_LT(util, 1.0f);

    mgr.deallocate(a);
    mgr.deallocate(b);
    mgr.deallocate(c);

    float frag = mgr.getTotalFragmentation();
    EXPECT_GE(frag, 0.0f);
    EXPECT_LE(frag, 1.0f);
}

TEST(MemoryTierManagerTest, CalculateRelationshipCoherence) {
    MemoryTierManager mgr;
    sep::pattern::PatternData a;
    a.id = "1";
    sep::pattern::PatternData b;
    b.id = "2";
    mgr.registerPattern(1, a);
    mgr.registerPattern(2, b);
    mgr.updateRelationship(1, 2, 0);
    mgr.calculateRelationshipCoherence();
    const auto* pa = mgr.getPatternData(1);
    const auto* pb = mgr.getPatternData(2);
    ASSERT_NE(pa, nullptr);
    ASSERT_NE(pb, nullptr);
    EXPECT_FLOAT_EQ(pa->coherence, 1.0f);
    EXPECT_FLOAT_EQ(pb->coherence, 1.0f);
}

TEST(MemoryTierManagerTest, CleanupExpiredPatterns) {
    MemoryTierManager mgr;
    sep::pattern::PatternData p1;
    p1.id = "1";
    p1.coherence = 0.1f;
    sep::pattern::PatternData p2;
    p2.id = "2";
    p2.coherence = 0.8f;
    mgr.registerPattern(1, p1);
    mgr.registerPattern(2, p2);
    mgr.cleanupExpiredPatterns();
    EXPECT_EQ(mgr.getPatternData(1), nullptr);
    EXPECT_NE(mgr.getPatternData(2), nullptr);
}

TEST(MemoryTierManagerTest, PrunePatternsByPriority) {
    MemoryTierManager mgr;
    for (size_t i = 0; i < 5; ++i) {
        sep::persistence::PatternData pdata;
        pdata.coherence = static_cast<float>(i) / 5.0f;
        mgr.getLTM().addPattern(i, pdata);
        sep::pattern::PatternData pat;
        pat.id = std::to_string(i);
        pat.coherence = pdata.coherence;
        pat.memory_tier = sep::memory::MemoryTierEnum::LTM;
        mgr.registerPattern(i, pat);
    }
    mgr.prunePatternsByPriority(MemoryTierEnum::LTM, 2);
    size_t remaining = 0;
    for (size_t i = 0; i < 5; ++i) {
        if (mgr.getLTM().getPattern(i))
            ++remaining;
    }
    EXPECT_LE(remaining, 2u);
}

