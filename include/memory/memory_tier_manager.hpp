#pragma once

/**
 * @brief Manager for STM/MTM/LTM memory tiers.
 *
 * Provides global access to tiered memory pools, handling allocation,
 * promotion and demotion of MemoryBlock instances.
 */

// Project includes
#include "compat/shim.h"
#include "core/common.h"
#include "core/dag_graph.h"
#include "core/types.h"
#include "memory/memory_tier.hpp"
#include "memory/types.h"
#include "persistence/persistent_pattern_data.hpp"
#include "quantum/data.hpp"
#include "quantum/types.h"
#ifndef SEP_NO_REDIS
#include "memory/redis_manager.h"
#endif // SEP_NO_REDIS

// Standard library includes
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <system_error>
#include <unordered_map>
#include <vector>

#include <glm/vec3.hpp>

namespace sep {
namespace core {
class SystemHooks;
}

namespace memory {

using ::sep::MemoryTierEnum;

class MemoryTierManager {
public:
  using Config = ::sep::config::MemoryThresholdConfig;

  // Singleton access
  static MemoryTierManager &getInstance();

  MemoryTierManager();
  explicit MemoryTierManager(const Config &cfg);
  ~MemoryTierManager();

  void init(const Config &config);
  void shutdown();

  // Memory block allocation and management
  MemoryBlock *allocate(std::size_t size, MemoryTierEnum tier);
  void deallocate(MemoryBlock *block);
  MemoryBlock *findBlockByPtr(void *ptr);

  // Tier management
  MemoryTier *getTier(MemoryTierEnum tier);
  float getTierUtilization(MemoryTierEnum tier) const;
  float getTierFragmentation(MemoryTierEnum tier) const;
  float getTotalUtilization() const;
  float getTotalFragmentation() const;
  void defragmentTier(MemoryTierEnum tier);
  void optimizeBlocks();
  void optimizeTiers();

  // Aggregate metrics
  std::size_t getTotalAllocated() const;

  // Access tier objects
  MemoryTier &getSTM();
  MemoryTier &getMTM();
  MemoryTier &getLTM();

  // Block promotion/demotion
  SEPResult promoteBlock(MemoryBlock *block, MemoryBlock *&out_block);
  SEPResult demoteBlock(MemoryBlock *block, MemoryBlock *&out_block);
  MemoryTier *determineTier(float coherence, float stability,
                            int generation_count);
  MemoryBlock *updateBlockMetrics(MemoryBlock *block, float coherence,
                                  float stability, std::uint32_t generation,
                                  float context_score);
  void rebuildLookup();

  // Pattern management

  SEPResult launch_pattern_processing(
      ::sep::pattern::PatternData *patterns,
      ::sep::pattern::PatternData *results,
      const ::sep::pattern::PatternConfig &config, size_t pattern_count,
      const ::sep::pattern::PatternData *previous_patterns, void *stream);

  void updateRelationship(std::size_t id_a, std::size_t id_b, float strength);
  void removePattern(std::size_t id);
  void pruneWeakRelationships();
  void calculateRelationshipCoherence();
  void loadLTMFromPersistence();
  void
  storeLTMToPersistence(const ::sep::quantum::Pattern &pattern,
                        const ::sep::persistence::PersistentPatternData &data);
  std::unique_ptr<::sep::quantum::Pattern> findPattern(std::size_t id);
  std::unique_ptr<::sep::quantum::Pattern> findPattern(std::size_t id) const;
  void registerPattern(std::size_t id,
                       const ::sep::pattern::PatternData &pattern);
  const ::sep::pattern::PatternData *getPatternData(std::size_t id) const;
  void cleanupExpiredPatterns();
  void prunePatternsByPriority(MemoryTierEnum tier, size_t max_count);

  // Test helpers
  void resetForTesting(const Config &cfg = Config());

  dag::DagGraph &getDagGraph() { return dag_graph_; }

private:
  static std::unique_ptr<MemoryTierManager> instance_;
  static std::once_flag once_flag_;

  Config config_;
  std::unique_ptr<MemoryTier> stm_;
  std::unique_ptr<MemoryTier> mtm_;
  std::unique_ptr<MemoryTier> ltm_;
  std::unordered_map<void *, MemoryBlock *> lookup_map_;
  // Legacy pointer lookup table used during tier transitions. When blocks
  // move between tiers the old pointer remains valid for a short period so
  // tests can resolve both the new and previous addresses. This map stores
  // those temporary associations until the next rebuild.
  std::unordered_map<void *, MemoryBlock *> legacy_lookup_map_;

private:
  dag::DagGraph dag_graph_;
  std::unordered_map<std::size_t, uint64_t> pattern_dag_map_;
  core::SystemHooks *hooks_{nullptr};

  std::unordered_map<std::size_t, std::unique_ptr<::sep::pattern::PatternData>>
      pattern_registry_;
  std::unordered_map<std::size_t, std::unordered_map<std::size_t, float>>
      pattern_relationships_;

  SEPResult promoteToTier(MemoryBlock *block, MemoryTierEnum tier,
                          MemoryBlock *&out_block);
  SEPResult compressBlock(MemoryBlock *block);

  // Pattern tier transition helpers
  bool checkTierPromotion(const ::sep::pattern::PatternData &pattern,
                          MemoryTier target_tier) const;
  bool checkTierDemotion(const ::sep::pattern::PatternData &pattern) const;
};

} // namespace memory

namespace config {
void to_json(nlohmann::json &j, const MemoryThresholdConfig &c);
void from_json(const nlohmann::json &j, MemoryThresholdConfig &c);
} // namespace config

} // namespace sep
