#include <gtest/gtest.h>
#include "memory/memory_tier_manager.hpp"
#include "memory/memory_tier.hpp"

using namespace sep::memory;

TEST(MemoryTierManagerTest, BasicInitialization) {
    MemoryTierManager mgr;
    EXPECT_NE(nullptr, mgr.getTier(sep::memory::TierType::STM));
    EXPECT_NE(nullptr, mgr.getTier(sep::memory::TierType::MTM));
    EXPECT_NE(nullptr, mgr.getTier(sep::memory::TierType::LTM));
}

TEST(MemoryTierManagerTest, AllocationAndDeallocation) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, sep::memory::TierType::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    mgr.deallocate(block);
    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
}

TEST(MemoryTierManagerTest, PromotionAndDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, sep::memory::TierType::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.9f, 0.9f, 6, 1.0f); // trigger promotion

    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);

    MemoryBlock* promoted = nullptr;
    for (const auto& b : mgr.getTier(sep::memory::TierType::LTM)->getBlocks()) {
        if (b.allocated) {
            promoted = const_cast<MemoryBlock*>(&b);
            break;
        }
    }
    ASSERT_NE(promoted, nullptr);
    mgr.updateBlockMetrics(promoted, 0.0f, 0.0f, 0, 1.0f); // trigger demotion

    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::LTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
}

TEST(MemoryTierManagerTest, DefragmentationTriggersPromotionDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, sep::memory::TierType::MTM);
    ASSERT_NE(block, nullptr);
    mgr.updateBlockMetrics(block, 0.9f, 0.9f, 6, 1.0f); // should be promoted after defrag
    mgr.defragmentTier(sep::memory::TierType::MTM);

    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);

    MemoryBlock* promoted = nullptr;
    for (const auto& b : mgr.getTier(sep::memory::TierType::STM)->getBlocks()) {
        if (b.allocated) {
            promoted = const_cast<MemoryBlock*>(&b);
            break;
        }
    }
    ASSERT_NE(promoted, nullptr);
    mgr.updateBlockMetrics(promoted, 0.0f, 0.0f, 0, 1.0f);
    mgr.defragmentTier(sep::memory::TierType::STM);

    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
}



TEST(MemoryTierManagerTest, OptimizeBlocksPromotionDemotion) {
    MemoryTierManager mgr;
    MemoryBlock* block = mgr.allocate(1024, sep::memory::TierType::MTM);
    ASSERT_NE(block, nullptr);

    mgr.updateBlockMetrics(block, 0.9f, 0.9f, 6, 1.0f); // expect promotion
    mgr.optimizeBlocks();

    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);

    MemoryBlock* promoted = nullptr;
    for (const auto& b : mgr.getTier(sep::memory::TierType::STM)->getBlocks()) {
        if (b.allocated) {
            promoted = const_cast<MemoryBlock*>(&b);
            break;
        }
    }
    ASSERT_NE(promoted, nullptr);
    mgr.updateBlockMetrics(promoted, 0.0f, 0.0f, 0, 1.0f); // expect demotion
    mgr.optimizeBlocks();

    EXPECT_EQ(mgr.getTierUtilization(sep::memory::TierType::STM), 0.0f);
    EXPECT_GT(mgr.getTierUtilization(sep::memory::TierType::MTM), 0.0f);
}

TEST(MemoryTierManagerTest, AllocationNearDefragmentBoundary) {
    MemoryTierManager::Config cfg;
    cfg.stm_size = 4096;
    cfg.mtm_size = 4096;
    cfg.ltm_size = 4096;
    cfg.use_unified_memory = true;
    cfg.enable_compression = false;

    MemoryTierManager mgr(cfg);

    MemoryBlock* block1 = mgr.allocate(2048, sep::memory::TierType::STM);
    ASSERT_NE(block1, nullptr);
    MemoryBlock* block2 = mgr.allocate(1024, sep::memory::TierType::STM);
    ASSERT_NE(block2, nullptr);
    MemoryBlock* block3 = mgr.allocate(512, sep::memory::TierType::STM);
    ASSERT_NE(block3, nullptr);

    mgr.deallocate(block2);

    MemoryBlock* block4 = mgr.allocate(1536, sep::memory::TierType::STM);
    ASSERT_NE(block4, nullptr);

    EXPECT_TRUE(block1->allocated);
    bool found = false;
    for (auto& b : mgr.getTier(sep::memory::TierType::STM)->getBlocks()) {
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

    MemoryBlock* b1 = mgr.allocate(1024, sep::memory::TierType::STM);
    ASSERT_NE(b1, nullptr);
    MemoryBlock* b2 = mgr.allocate(1024, sep::memory::TierType::STM);
    ASSERT_NE(b2, nullptr);

    mgr.deallocate(b1);

    float frag_before = mgr.getTierFragmentation(sep::memory::TierType::STM);
    EXPECT_GT(frag_before, cfg.fragmentation_threshold);

    mgr.optimizeTiers();

    float frag_after = mgr.getTierFragmentation(sep::memory::TierType::STM);
    EXPECT_LT(frag_after, frag_before);

    mgr.deallocate(b2);
}

TEST(MemoryTierManagerTest, TotalMetrics) {
    MemoryTierManager mgr;
    MemoryBlock* a = mgr.allocate(128, sep::memory::TierType::STM);
    MemoryBlock* b = mgr.allocate(128, sep::memory::TierType::MTM);
    MemoryBlock* c = mgr.allocate(128, sep::memory::TierType::LTM);

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

