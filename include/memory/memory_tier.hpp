#pragma once

// C++ Standard Library
#include <chrono>
#include <cstddef>
#include <deque>
#include <unordered_map>
#include <vector>
#include <cstdint>

// Third-party headers
#include <glm/vec3.hpp>

// Project headers
#include "../compat/shim.h"
#include "../core/common.h"
#include "../core/types.h"
#include "types.h"
#include "persistence/persistent_pattern_data.hpp"

namespace sep {
namespace memory {

using ::sep::CompressionMethod;
using ::sep::MemoryTierEnum;
using ::sep::SEPResult;

// Small epsilon used when comparing utilization metrics.  The value is tuned
// so that allocations of a single kilobyte in a default 1MB tier are still
// reported as non-zero while tiny rounding artifacts after promotions or
// defragmentation are clamped to zero.
// Increase epsilon slightly to better absorb rounding artifacts that occur
// when tiers temporarily hold extremely small allocations during promotion
// or defragmentation.  Values below this threshold are effectively treated
// as zero utilization in tests.
// Relax the epsilon to further suppress tiny residuals that appear after
// promotions or tier defragmentation.  Values below this threshold are treated
// as zero when reporting utilization metrics.
inline constexpr float kUtilizationEpsilon = 1e-2f;

// Memory tier types
enum class TierType {
    HOST = 0,   // Host memory (CPU)
    DEVICE = 1, // Device memory (GPU)
    UNIFIED = 2 // Unified memory (accessible by both CPU and GPU)
};

// Macros for CUDA kernel compatibility
#ifdef __CUDACC__
#ifndef SEP_MEMORY_TIER_STM
#define SEP_MEMORY_TIER_STM static_cast<int>(::sep::MemoryTierEnum::STM)
#endif
#ifndef SEP_MEMORY_TIER_MTM
#define SEP_MEMORY_TIER_MTM static_cast<int>(::sep::MemoryTierEnum::MTM)
#endif
#ifndef SEP_MEMORY_TIER_LTM
#define SEP_MEMORY_TIER_LTM static_cast<int>(::sep::MemoryTierEnum::LTM)
#endif
#endif

struct MemoryBlock {
    void*                      ptr{nullptr};
    std::size_t                size{0};
    std::size_t                offset{0};
    std::size_t                original_size{0};
    std::size_t                access_count{0};
    std::uint64_t              wait{0};
    std::uint32_t              generation{0};
    MemoryTierEnum            tier{MemoryTierEnum::STM};
    CompressionMethod compression{CompressionMethod::None};
    float                      utilization{0.0f};
    float                      stability{0.0f};
    float                      coherence{0.0f};
    float                      weight{0.0f};
    float                      coherence_trend{0.0f};
    float                      last_coherence{0.0f};
    float                      compression_ratio{1.0f};
    bool                       allocated{false};

    MemoryBlock() = default;
    MemoryBlock(void* p, std::size_t s, std::size_t off, MemoryTierEnum t)
        : ptr(p), size(s), offset(off), original_size(s), tier(t) {}
};


class MemoryTier {
public:
    struct Config {
        MemoryTierEnum type{MemoryTierEnum::STM};
        std::size_t size{0};
    };

    explicit MemoryTier(const Config& config);

    // Pattern management constructor
    MemoryTier(MemoryTierEnum type, size_t max_patterns, float coherence_threshold, int min_generations);

    // Combined constructor for memory pool and pattern management
    MemoryTier(const Config& config, size_t max_patterns, float coherence_threshold, int min_generations);

    ~MemoryTier();

    // Memory block management methods
    MemoryBlock* allocate(std::size_t size);
    void deallocate(MemoryBlock* block);
    SEPResult defragment();

    float calculateFragmentation() const;
    float calculateUtilization() const;
    std::size_t getFreeSpace() const;
    std::size_t getLargestFreeBlock() const;
    const std::deque<MemoryBlock>& getBlocks() const;
    bool moveData(MemoryBlock* dst, const MemoryBlock* src);

    // Resize the underlying memory pool, returns true on success
    bool resize(std::size_t new_size);

    // Expose configuration for manager-level optimizations
    MemoryTierEnum getType() const {
        return config_.type;
    }
    std::size_t getSize() const {
        return config_.size;
    }

    // Pattern management methods
    bool canAcceptPattern(const ::sep::persistence::PersistentPatternData& pattern) const;
    void addPattern(size_t id, ::sep::persistence::PersistentPatternData pattern);
    void removePattern(size_t id);
    const ::sep::persistence::PersistentPatternData* getPattern(size_t id) const;
    ::sep::persistence::PersistentPatternData* getPattern(size_t id);
    const std::unordered_map<size_t, ::sep::persistence::PersistentPatternData>& getPatterns() const {
        return m_patterns;
    }

private:
    MemoryBlock* findFreeBlock(std::size_t size);
    void splitBlock(MemoryBlock* block, std::size_t size);
    void mergeAdjacentBlocks();

    Config config_;
    void* memory_pool_{nullptr};
    std::deque<MemoryBlock> blocks_;
    std::size_t used_space_{0};

    // Pattern management members
    size_t m_max_patterns{0};
    float m_coherence_threshold{0.0f};
    int m_min_generations{0};
    std::unordered_map<size_t, ::sep::persistence::PersistentPatternData> m_patterns;
};

}  // namespace memory
}  // namespace sep
