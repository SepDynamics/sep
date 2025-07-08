#include "memory/redis_manager.h"
#include "memory/types.h"
#include <gtest/gtest.h>
#include <algorithm>

using namespace sep::persistence;

class RedisManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        redis_manager = sep::persistence::createRedisManager("localhost", 6379);
    }

    void TearDown() override {
        redis_manager.reset();
    }

    // Helper to access private Impl methods for testing
#if 0
    class TestableRedisManager : public RedisManager {
    public:
        TestableRedisManager(const std::string& host, int port) : RedisManager(host, port) {}

        std::string getPatternKey(std::uint64_t id, const std::string& tier) const {
            return impl_->getPatternKey(id, tier);
        }

        std::string getTierPatternsKey(const std::string& tier) const {
            return impl_->getTierPatternsKey(tier);
        }

        std::string normalizeTier(const std::string& tier) const {
            return impl_->normalizeTier(tier);
        }
    };
#endif

    std::shared_ptr<sep::persistence::IRedisManager> redis_manager;
};

#if 0
TEST_F(RedisManagerTest, NormalizeTier) {
    // Test case normalization
    EXPECT_EQ(normalizeTier("stm"), "STM");
    EXPECT_EQ(normalizeTier("STM"), "STM");
    EXPECT_EQ(normalizeTier("StM"), "STM");

    // Test mtm/ltm normalization
    EXPECT_EQ(normalizeTier("mtm"), "MTM");
    EXPECT_EQ(normalizeTier("ltm"), "LTM");
}
#endif

#if 0
TEST_F(RedisManagerTest, KeyFormatConsistency) {
    // Test pattern key format
    std::string pattern_key = getPatternKey(123, "STM");
    EXPECT_EQ(pattern_key, "pattern:STM:123");

    // Test tier patterns key format
    std::string tier_key = getTierPatternsKey("STM");
    EXPECT_EQ(tier_key, "STM:patterns");
}
#endif

TEST_F(RedisManagerTest, InvalidTierHandling) {
    sep::persistence::PersistentPatternData data{};
    data.coherence = 0.8f;
    
    // Store with invalid tier should still work due to normalization
    redis_manager->storePattern(1, data, "invalid");
    
    // Load should return empty since tier is normalized
    auto loaded = redis_manager->loadPattern(1, "invalid");
    EXPECT_FALSE(loaded.has_value());
    
    // Pattern IDs should be empty for invalid tier
    auto ids = redis_manager->getPatternIds("invalid");
    EXPECT_TRUE(ids.empty());
}

TEST_F(RedisManagerTest, ConnectionFailureHandling) {
    // Create manager with invalid connection
    auto failed_manager = sep::persistence::createRedisManager("nonexistent", 1234);
    
    // Should return false for connection status
    EXPECT_FALSE(failed_manager->isConnected());
    
    sep::persistence::PersistentPatternData data{};
    
    // Operations should fail gracefully
    failed_manager->storePattern(1, data, "STM");
    auto loaded = failed_manager->loadPattern(1, "STM");
    EXPECT_FALSE(loaded.has_value());
    
    auto ids = failed_manager->getPatternIds("STM");
    EXPECT_TRUE(ids.empty());
}

TEST_F(RedisManagerTest, StoreAndLoadPattern) {
    sep::persistence::PersistentPatternData data{};
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
    sep::persistence::PersistentPatternData data{};
    
    // Store patterns in mixed case tiers
    redis_manager->storePattern(1, data, "STM");
    redis_manager->storePattern(2, data, "stm");
    
    // Get IDs should work with any case
    auto ids = redis_manager->getPatternIds("stm");
    EXPECT_EQ(ids.size(), 2);
    EXPECT_TRUE(std::find(ids.begin(), ids.end(), 1ULL) != ids.end());
    EXPECT_TRUE(std::find(ids.begin(), ids.end(), 2ULL) != ids.end());
}

TEST_F(RedisManagerTest, RemovePattern) {
    sep::persistence::PersistentPatternData data{};
    
    redis_manager->storePattern(1, data, "STM");
    redis_manager->removePattern(1, "stm"); // Different case
    
    auto loaded = redis_manager->loadPattern(1, "STM");
    EXPECT_FALSE(loaded.has_value());
}

TEST_F(RedisManagerTest, BulkOperations) {
    std::vector<std::pair<std::uint64_t, sep::persistence::PersistentPatternData>> patterns;
    sep::persistence::PersistentPatternData data{};
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