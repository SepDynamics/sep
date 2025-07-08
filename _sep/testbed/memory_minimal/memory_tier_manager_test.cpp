#include <gtest/gtest.h>

#include "memory/memory_tier.hpp"
#include "memory/memory_tier_manager.hpp"

using namespace sep::memory;

class MemoryTierManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        MemoryTierManager::Config cfg;
        cfg.stm_size = 4096;  // 4KB for each tier
        cfg.mtm_size = 4096;
        cfg.ltm_size = 4096;
        cfg.promote_stm_to_mtm = 0.7f;
        cfg.promote_mtm_to_ltm = 0.8f;
        cfg.demote_threshold = 0.3f;
        cfg.stm_to_mtm_min_gen = 2;
        cfg.mtm_to_ltm_min_gen = 4;
        manager.resetForTesting(cfg);
    }

    MemoryTierManager& manager = MemoryTierManager::getInstance();
};

TEST_F(MemoryTierManagerTest, PromotionAndDemotion)
{
    // Allocate a block in STM
    MemoryBlock* block = manager.allocate(1024, MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_EQ(block->tier, MemoryTierEnum::STM);

    // Update metrics to trigger promotion to MTM
    block->coherence = 0.8f;
    block->stability = 0.8f;
    block->generation = 3;
    MemoryBlock* promoted = manager.updateBlockMetrics(block, block->coherence, block->stability,
                                                       block->generation, 1.0f);
    ASSERT_NE(promoted, nullptr);
    EXPECT_EQ(promoted->tier, MemoryTierEnum::MTM);

    // Update metrics to trigger demotion back to STM
    promoted->coherence = 0.2f;
    promoted->stability = 0.2f;
    MemoryBlock* demoted = manager.updateBlockMetrics(
        promoted, promoted->coherence, promoted->stability, promoted->generation, 1.0f);
    ASSERT_NE(demoted, nullptr);
    EXPECT_EQ(demoted->tier, MemoryTierEnum::STM);

    // Verify utilization is exactly 0.0f when tier is empty
    manager.deallocate(demoted);
    EXPECT_EQ(manager.getTierUtilization(MemoryTierEnum::STM), 0.0f);
}

TEST_F(MemoryTierManagerTest, DefragmentationTriggersPromotionDemotion)
{
    // Allocate multiple blocks in STM
    MemoryBlock* block1 = manager.allocate(1024, MemoryTierEnum::STM);
    MemoryBlock* block2 = manager.allocate(1024, MemoryTierEnum::STM);
    ASSERT_NE(block1, nullptr);
    ASSERT_NE(block2, nullptr);

    // Set metrics to trigger promotion
    block1->coherence = 0.8f;
    block1->stability = 0.8f;
    block1->generation = 3;
    block2->coherence = 0.8f;
    block2->stability = 0.8f;
    block2->generation = 3;

    // Defragment STM tier
    manager.defragmentTier(MemoryTierEnum::STM);

    // Verify blocks were promoted during defragmentation
    MemoryBlock* found1 = manager.findBlockByPtr(block1->ptr);
    MemoryBlock* found2 = manager.findBlockByPtr(block2->ptr);
    ASSERT_NE(found1, nullptr);
    ASSERT_NE(found2, nullptr);
    EXPECT_EQ(found1->tier, MemoryTierEnum::MTM);
    EXPECT_EQ(found2->tier, MemoryTierEnum::MTM);
}

TEST_F(MemoryTierManagerTest, OptimizeBlocksPromotionDemotion)
{
    // Allocate blocks in different tiers
    MemoryBlock* stm_block = manager.allocate(1024, MemoryTierEnum::STM);
    MemoryBlock* mtm_block = manager.allocate(1024, MemoryTierEnum::MTM);
    ASSERT_NE(stm_block, nullptr);
    ASSERT_NE(mtm_block, nullptr);

    // Set metrics to trigger promotion/demotion
    stm_block->coherence = 0.8f;
    stm_block->stability = 0.8f;
    stm_block->generation = 3;
    mtm_block->coherence = 0.2f;
    mtm_block->stability = 0.2f;
    mtm_block->generation = 3;

    // Optimize all blocks
    manager.optimizeBlocks();

    // Verify blocks were moved to correct tiers
    MemoryBlock* found1 = manager.findBlockByPtr(stm_block->ptr);
    MemoryBlock* found2 = manager.findBlockByPtr(mtm_block->ptr);
    ASSERT_NE(found1, nullptr);
    ASSERT_NE(found2, nullptr);
    EXPECT_EQ(found1->tier, MemoryTierEnum::MTM);
    EXPECT_EQ(found2->tier, MemoryTierEnum::STM);
}