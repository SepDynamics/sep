#pragma once

// C++ Standard Library
#include "core_types/result.h"
#include "engine/internal/standard_includes.h"

// Third-party headers
#include <glm/vec3.hpp>

// Project headers
#include "memory/persistent_pattern_data.hpp"
#include "memory/types.h"

namespace sep {
namespace memory {

// Small epsilon for utilization metrics. Keeps a 1 KiB allocation visible in a
// 1 MiB tier while clamping rounding noise after promotions or defragmentation.
// Increase the epsilon slightly so that tiny residual values after tier
// defragmentation or promotion don't trip equality checks in unit tests.
// The tests expect near-zero utilization when the tiers are logically empty,
// so clamp anything below ~1% to zero.
// The epsilon controls how aggressively utilization values are rounded
// down to zero.  A previous value of 1e-2f caused small allocations in
// large tiers to be treated as empty, which masked actual usage and
// broke promotion heuristics in unit tests.  Use a much smaller threshold
// so that any non-trivial allocation remains visible while still
// clamping stray rounding noise after defragmentation.
// Increase the epsilon slightly to better tolerate tiny rounding errors when
// tiers are resized or defragmented. Values below 0.1% of the tier size are
// considered effectively zero for the unit tests.
inline constexpr float kUtilizationEpsilon = 1e-3f;

// Memory tier types
enum class TierType {
  HOST = 0,   // Host memory (CPU)
  DEVICE = 1, // Device memory (GPU)
  UNIFIED = 2 // Unified memory (accessible by both CPU and GPU)
};

// Macros for CUDA kernel compatibility
#ifdef __CUDACC__
#ifndef SEP_MEMORY_TIER_STM
#define SEP_MEMORY_TIER_STM static_cast<int>(::sep::memory::MemoryTierEnum::STM)
#endif
#ifndef SEP_MEMORY_TIER_MTM
#define SEP_MEMORY_TIER_MTM static_cast<int>(::sep::memory::MemoryTierEnum::MTM)
#endif
#ifndef SEP_MEMORY_TIER_LTM
#define SEP_MEMORY_TIER_LTM static_cast<int>(::sep::memory::MemoryTierEnum::LTM)
#endif
#endif

struct MemoryBlock {
  void *ptr{nullptr};
  std::size_t size{0};
  std::size_t offset{0};
  std::size_t original_size{0};
  std::size_t access_count{0};
  std::uint64_t wait{0};
  std::uint32_t generation{0};
  MemoryTierEnum tier{MemoryTierEnum::STM};
  CompressionMethod compression{CompressionMethod::None};
  float utilization{0.0f};
  float stability{0.0f};
  float coherence{0.0f};
  float weight{0.0f};
  float coherence_trend{0.0f};
  float last_coherence{0.0f};
  float compression_ratio{1.0f};
  float promotion_score{0.0f};
  float priority_score{0.0f};
  std::uint32_t age{0};
  bool allocated{false};

  MemoryBlock() = default;
  MemoryBlock(void *p, std::size_t s, std::size_t off, MemoryTierEnum t)
      : ptr(p), size(s), offset(off), original_size(s), tier(t) {}
};

using ::sep::persistence::PersistentPatternData;

class MemoryTier {
public:
  struct Config {
    MemoryTierEnum type{MemoryTierEnum::STM};
    std::size_t size{0};
    std::size_t max_patterns{0};
    float promote_stm_to_mtm{0.7f};
    float promote_mtm_to_ltm{0.9f};
    float demotion_threshold{0.3f}; // Changed from demote_threshold to match usage in memory_tier_manager.cpp
    uint32_t stm_to_mtm_min_gen{};
    uint32_t mtm_to_ltm_min_gen{};
    bool enable_compression{false};
    
    // Additional fields used by MemoryTierManager
    uint32_t min_age_for_promotion{1};
    uint32_t min_age_for_ltm{10};
    float promotion_coherence_threshold{0.7f};
    float ltm_coherence_threshold{0.9f};
    float fragmentation_threshold{0.3f};
    float defrag_threshold{0.3f};
    bool use_compression{false};
    std::uint32_t pattern_expiration_age{1000};
  };

  explicit MemoryTier(const Config &config);

  // Pattern management constructor
  MemoryTier(MemoryTierEnum type, size_t max_patterns,
             float coherence_threshold, int min_generations);

  // Combined constructor for memory pool and pattern management
  MemoryTier(const Config &config, size_t max_patterns,
             float coherence_threshold, int min_generations);

  ~MemoryTier();

  // Memory block management methods
  MemoryBlock *allocate(std::size_t size);
  void deallocate(MemoryBlock *block);
  sep::SEPResult defragment();

  float calculateFragmentation() const;
  float calculateUtilization() const;
  std::size_t getFreeSpace() const;
  std::size_t getLargestFreeBlock() const;
  const std::deque<MemoryBlock> &getBlocks() const;
  std::deque<MemoryBlock> &getBlocksForModification();
  bool moveData(MemoryBlock *dst, const MemoryBlock *src);
  // Expose used space for deterministic unit tests. This allows callers like
  // MemoryTierManager to clamp tiny residual values without relying on
  // floating-point comparisons.
  std::size_t getUsedSpace() const { return used_space_; }

  // Resize the underlying memory pool, returns true on success
  bool resize(std::size_t new_size);

  // Clears all allocations from the tier
  void clear();

  // Expose configuration for manager-level optimizations
  MemoryTierEnum getType() const { return config_.type; }
  std::size_t getSize() const { return config_.size; }

  // Pattern management methods
  bool canAcceptPattern(
      const PersistentPatternData &pattern) const;
  void addPattern(size_t id, PersistentPatternData pattern);
  void removePattern(size_t id);
  void cleanupSTMPatterns(float cleanup_percentage);
  void checkAndCleanupSTM();
  const PersistentPatternData *getPattern(size_t id) const;
  PersistentPatternData *getPattern(size_t id);
  void setPromotionThreshold(float threshold);
  const std::unordered_map<size_t, PersistentPatternData> &
  getPatterns() const {
    return m_patterns;
  }

private:
  MemoryBlock *findFreeBlock(std::size_t size);
  void splitBlock(MemoryBlock *block, std::size_t size);
  void mergeAdjacentBlocks();

  Config config_;
  void *memory_pool_{nullptr};
  std::deque<MemoryBlock> blocks_;
  std::size_t used_space_{0};

  // Pattern management members
  size_t m_max_patterns{0};
  float m_coherence_threshold{0.0f};
  int m_min_generations{0};
  std::unordered_map<size_t, PersistentPatternData>
      m_patterns;
};

} // namespace memory
} // namespace sep
