#include "../memory/memory_tier_manager.hpp"
#include "../memory/memory_tier.hpp"
#include "../core/types.h"
#include "../compat/shim.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace sep::memory;
using sep::memory::MemoryTierEnum;
using sep::SEPResult;

class MemoryTierManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup a test configuration with small tiers
        MemoryTierManager::Config config;
        config.stm_size = 1024;  // 1KB
        config.mtm_size = 2048;  // 2KB
        config.ltm_size = 4096;  // 4KB
        
        // Promotion thresholds
        config.promote_stm_to_mtm = 0.7f;
        config.promote_mtm_to_ltm = 0.8f;
        config.stm_to_mtm_min_gen = 5;
        config.mtm_to_ltm_min_gen = 10;
        
        // Demotion threshold
        config.demote_threshold = 0.3f;
        
        // Reset the manager with our test config
        MemoryTierManager::getInstance().resetForTesting(config);
    }
    
    void TearDown() override {
        // Clean up
        MemoryTierManager::getInstance().shutdown();
    }
};

// Test basic allocation and deallocation
TEST_F(MemoryTierManagerTest, BasicAllocationAndDeallocation) {
    auto& manager = MemoryTierManager::getInstance();
    
    // Allocate a block in STM
    MemoryBlock* block = manager.allocate(128, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    EXPECT_EQ(block->tier, sep::memory::MemoryTierEnum::STM);
    EXPECT_EQ(block->size, 128);
    EXPECT_TRUE(block->allocated);
    
    // Write some data
    std::memset(block->ptr, 0xAA, 128);
    
    // Verify we can find the block by pointer
    MemoryBlock* found = manager.findBlockByPtr(block->ptr);
    EXPECT_EQ(found, block);
    
    // Deallocate
    manager.deallocate(block);
    
    // Block should no longer be findable
    found = manager.findBlockByPtr(block->ptr);
    EXPECT_EQ(found, nullptr);
}

// Test promotion from STM to MTM
TEST_F(MemoryTierManagerTest, BlockPromotion) {
    auto& manager = MemoryTierManager::getInstance();
    
    // Allocate a block in STM
    MemoryBlock* block = manager.allocate(256, sep::memory::MemoryTierEnum::STM);
    ASSERT_NE(block, nullptr);
    
    // Initialize with test data
    uint8_t* data = static_cast<uint8_t*>(block->ptr);
    for (size_t i = 0; i < 256; i++) {
        data[i] = static_cast<uint8_t>(i);
    }
    
    // Update metrics to trigger promotion
    MemoryBlock* promoted = manager.updateBlockMetrics(block, 0.8f, 0.8f, 10, 1.0f);
    ASSERT_NE(promoted, nullptr);
    EXPECT_EQ(promoted->tier, sep::memory::MemoryTierEnum::MTM);
    
    // Verify data was copied correctly
    uint8_t* promoted_data = static_cast<uint8_t*>(promoted->ptr);
    for (size_t i = 0; i < 256; i++) {
        EXPECT_EQ(promoted_data[i], static_cast<uint8_t>(i)) 
            << "Data mismatch at index " << i;
    }
}

// Test demotion from MTM to STM
TEST_F(MemoryTierManagerTest, BlockDemotion) {
    auto& manager = MemoryTierManager::getInstance();
    
    // Allocate a block in MTM
    MemoryBlock* block = manager.allocate(256, sep::memory::MemoryTierEnum::MTM);
    ASSERT_NE(block, nullptr);
    
    // Initialize with test data
    uint8_t* data = static_cast<uint8_t*>(block->ptr);
    for (size_t i = 0; i < 256; i++) {
        data[i] = static_cast<uint8_t>(i);
    }
    
    // Update metrics to trigger demotion
    MemoryBlock* demoted = manager.updateBlockMetrics(block, 0.2f, 0.2f, 5, 0.5f);
    ASSERT_NE(demoted, nullptr);
    EXPECT_EQ(demoted->tier, sep::memory::MemoryTierEnum::STM);
    
    // Verify data was copied correctly
    uint8_t* demoted_data = static_cast<uint8_t*>(demoted->ptr);
    for (size_t i = 0; i < 256; i++) {
        EXPECT_EQ(demoted_data[i], static_cast<uint8_t>(i)) 
            << "Data mismatch at index " << i;
    }
}

// Test tier utilization metrics
TEST_F(MemoryTierManagerTest, TierUtilization) {
    auto& manager = MemoryTierManager::getInstance();
    
    // Initially tiers should be empty
    EXPECT_NEAR(manager.getTierUtilization(sep::memory::MemoryTierEnum::STM), 0.0f, 0.001f);
    EXPECT_NEAR(manager.getTierUtilization(sep::memory::MemoryTierEnum::MTM), 0.0f, 0.001f);
    EXPECT_NEAR(manager.getTierUtilization(sep::memory::MemoryTierEnum::LTM), 0.0f, 0.001f);
    
    // Allocate blocks to fill about 50% of STM
    std::vector<MemoryBlock*> blocks;
    size_t allocated = 0;
    size_t target = manager.getSTM().getSize() / 2;
    
    while (allocated < target) {
        MemoryBlock* block = manager.allocate(64, sep::memory::MemoryTierEnum::STM);
        if (!block) break;
        blocks.push_back(block);
        allocated += 64;
    }
    
    // Check utilization is around 50% for STM
    float util = manager.getTierUtilization(sep::memory::MemoryTierEnum::STM);
    EXPECT_GE(util, 0.4f);
    EXPECT_LE(util, 0.6f);
    
    // Clean up
    for (auto block : blocks) {
        manager.deallocate(block);
    }
}

// Test pattern registry and relationships
TEST_F(MemoryTierManagerTest, PatternRegistry) {
    auto& manager = MemoryTierManager::getInstance();
    
    // Create a simple pattern
    sep::pattern::PatternData pattern;
    pattern.id = 1;
    pattern.values = {0.5f, 0.6f, 0.7f};
    
    // Register it
    manager.registerPattern(pattern.id, pattern);
    
    // Retrieve it
    const auto* retrieved = manager.getPatternData(pattern.id);
    ASSERT_NE(retrieved, nullptr);
    EXPECT_EQ(retrieved->id, pattern.id);
    EXPECT_EQ(retrieved->values.size(), 3);
    
    // Add a relationship
    sep::pattern::PatternData pattern2;
    pattern2.id = 2;
    pattern2.values = {0.1f, 0.2f, 0.3f};
    manager.registerPattern(pattern2.id, pattern2);
    
    // Link them
    manager.updateRelationship(pattern.id, pattern2.id, 0.75f);
    
    // Remove first pattern
    manager.removePattern(pattern.id);
    
    // Should no longer be retrievable
    EXPECT_EQ(manager.getPatternData(pattern.id), nullptr);
}

// Entry point
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}