#include "memory/redis_manager.h"
#include "memory/types.h"
#include <gtest/gtest.h>

using namespace sep::persistence;
using namespace sep::memory;

class RedisManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        redis_manager = createRedisManager("localhost", 6379);
    }

    void TearDown() override {
        redis_manager.reset();
    }

    std::shared_ptr<IRedisManager> redis_manager;
};

TEST_F(RedisManagerTest, StoreAndLoadPattern) {
    PersistentPatternData data{};
    data.coherence = 0.8f;
    data.stability = 0.9f;
    data.generation_count = 42;

    // Test with different tier string formats that should normalize to the same value
    const std::string tier1 = "stm";
    const std::string tier2 = "STM";
    
    // Store with lowercase
    redis_manager->storePattern(1, data, tier1);
    
    // Load with uppercase
    auto loaded = redis_manager->loadPattern(1, tier2);
    
    ASSERT_TRUE(loaded.has_value());
    EXPECT_FLOAT_EQ(loaded->coherence, data.coherence);
    EXPECT_FLOAT_EQ(loaded->stability, data.stability);
    EXPECT_EQ(loaded->generation_count, data.generation_count);
}

TEST_F(RedisManagerTest, GetPatternIds) {
    PersistentPatternData data{};
    
    // Store patterns in mixed case tiers
    redis_manager->storePattern(1, data, "STM");
    redis_manager->storePattern(2, data, "stm");
    
    // Get IDs should work with any case
    auto ids = redis_manager->getPatternIds("stm");
    EXPECT_EQ(ids.size(), 2);
    EXPECT_TRUE(std::find(ids.begin(), ids.end(), 1) != ids.end());
    EXPECT_TRUE(std::find(ids.begin(), ids.end(), 2) != ids.end());
}

TEST_F(RedisManagerTest, RemovePattern) {
    PersistentPatternData data{};
    
    redis_manager->storePattern(1, data, "STM");
    redis_manager->removePattern(1, "stm"); // Different case
    
    auto loaded = redis_manager->loadPattern(1, "STM");
    EXPECT_FALSE(loaded.has_value());
}

TEST_F(RedisManagerTest, BulkOperations) {
    std::vector<std::pair<std::uint64_t, PersistentPatternData>> patterns;
    PersistentPatternData data{};
    data.coherence = 0.8f;
    patterns.push_back({1, data});
    patterns.push_back({2, data});
    
    // Test bulk operations with mixed case
    redis_manager->bulkStore(patterns, "STM");
    auto loaded = redis_manager->bulkLoad({1, 2}, "stm");
    
    EXPECT_EQ(loaded.size(), 2);
    for (const auto& pattern : loaded) {
        EXPECT_FLOAT_EQ(pattern.coherence, data.coherence);
    }
}