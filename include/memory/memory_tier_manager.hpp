#pragma once

/**
 * @brief Manager for STM/MTM/LTM memory tiers.
 *
 * Provides global access to tiered memory pools, handling allocation,
 * promotion and demotion of MemoryBlock instances.
 */

// Project includes
#include "core/common.h"
#include "core/types.h"
#include "core/dag_graph.h"
#include "memory/memory_tier.hpp"
#include "memory/types.h"
#include "compat/shim.h"
#include "quantum/types.h"
#include "quantum/data.hpp"
#include "memory/redis_manager.h"

// Standard library includes
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <system_error>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

#include <glm/vec3.hpp>

namespace sep {
namespace core {
class SystemHooks;
}

namespace persistence {
class IRedisManager;
}

namespace memory {


class MemoryTierManager {
public:
    struct Config {
        std::size_t stm_size;
        std::size_t mtm_size;
        std::size_t ltm_size;
        float promote_stm_to_mtm;
        float promote_mtm_to_ltm;
        float demote_threshold;
        float fragmentation_threshold;
        bool use_unified_memory;
        bool enable_compression;
        std::uint32_t stm_to_mtm_min_gen;
        std::uint32_t mtm_to_ltm_min_gen;

        Config()
            : stm_size(1 << 20), mtm_size(4 << 20), ltm_size(16 << 20), promote_stm_to_mtm(0.7f),
              promote_mtm_to_ltm(0.9f), demote_threshold(0.3f), fragmentation_threshold(0.3f), use_unified_memory(true),
              enable_compression(true), stm_to_mtm_min_gen(5), mtm_to_ltm_min_gen(100) {}
    };

    // Singleton access
    static MemoryTierManager& getInstance();

    MemoryTierManager();
    explicit MemoryTierManager(const Config& cfg);
    explicit MemoryTierManager(const sep::config::MemoryThresholdConfig& cfg);
    ~MemoryTierManager();

    void init(const Config& config);
    void shutdown();

    // Memory block allocation and management
    MemoryBlock* allocate(std::size_t size, TierType tier);
    void deallocate(MemoryBlock* block);
    MemoryBlock* findBlockByPtr(void* ptr);

    // Tier management
    MemoryTier* getTier(TierType tier);
    float getTierUtilization(TierType tier) const;
    float getTierFragmentation(TierType tier) const;
    float getTotalUtilization() const;
    float getTotalFragmentation() const;
    void defragmentTier(TierType tier);
    void optimizeBlocks();
    void optimizeTiers();

    // Aggregate metrics
    std::size_t getTotalAllocated() const;

    // Access tier objects
    MemoryTier& getSTM();
    MemoryTier& getMTM();
    MemoryTier& getLTM();

    // Block promotion/demotion
    SEPResult promoteBlock(MemoryBlock* block, MemoryBlock*& out_block);
    SEPResult demoteBlock(MemoryBlock* block, MemoryBlock*& out_block);
    MemoryTier* determineTier(float coherence, float stability, int generation_count);
    void updateBlockMetrics(MemoryBlock* block, float coherence, float stability, std::uint32_t generation, float context_score);
    void rebuildLookup();

    // Pattern management

    SEPResult launch_pattern_processing(sep::pattern::PatternData* patterns,
                                      sep::pattern::PatternData* results,
                                      const sep::pattern::PatternConfig& config,
                                      size_t pattern_count,
                                      const sep::pattern::PatternData* previous_patterns,
                                      void* stream);
                                      
    void updateRelationship(std::size_t id_a, std::size_t id_b, uint8_t type);
    void removePattern(std::size_t id);
    void pruneWeakRelationships();
    void calculateRelationshipCoherence();
    void loadLTMFromPersistence();
    void storeLTMToPersistence(const ::sep::quantum::Pattern& pattern, const persistence::PersistentPatternData& data);
    std::unique_ptr<::sep::quantum::Pattern> findPattern(std::size_t id);
    std::unique_ptr<::sep::quantum::Pattern> findPattern(std::size_t id) const;
    void registerPattern(std::size_t id, const ::sep::pattern::PatternData& pattern);
    const ::sep::pattern::PatternData* getPatternData(std::size_t id) const;
    void cleanupExpiredPatterns();
    void prunePatternsByPriority(TierType tier, size_t max_count);

    dag::DagGraph& getDagGraph() {
        return dag_graph_;
    }

private:
    static std::unique_ptr<MemoryTierManager> instance_;
    static std::once_flag once_flag_;
    
    Config config_;
    std::unique_ptr<MemoryTier> stm_;
    std::unique_ptr<MemoryTier> mtm_;
    std::unique_ptr<MemoryTier> ltm_;
    std::unordered_map<void*, MemoryBlock*> lookup_map_;

private:
    dag::DagGraph dag_graph_;
    std::unordered_map<std::size_t, uint64_t> pattern_dag_map_;
    std::unique_ptr<persistence::IRedisManager> redis_manager_;
    core::SystemHooks* hooks_{nullptr};

    std::unordered_map<std::size_t, std::unique_ptr<::sep::pattern::PatternData>> pattern_registry_;
    std::unordered_map<std::size_t, std::unordered_map<std::size_t, float>> pattern_relationships_;

    SEPResult promoteToTier(MemoryBlock* block, MemoryTier tier, MemoryBlock*& out_block);
    SEPResult compressBlock(MemoryBlock* block);

    // Pattern tier transition helpers
    bool checkTierPromotion(const ::sep::pattern::PatternData& pattern, MemoryTier target_tier) const;
  bool checkTierDemotion(const ::sep::pattern::PatternData& pattern) const;
};


void to_json(nlohmann::json& j, const MemoryTierManager::Config& c);

void from_json(const nlohmann::json& j, MemoryTierManager::Config& c);

}  // namespace memory
}  // namespace sep
