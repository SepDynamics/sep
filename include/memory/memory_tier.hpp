#pragma once

#include <chrono>
#include <cstddef>
#include <deque>
#include <glm/vec3.hpp>
#include <unordered_map>
#include <vector>

#include "blender/compression.h"
#include "core/common.h"
#include "cuda/math_common.h"
#include "memory/memory_tier.h"
#include "memory/types.h"
#include "quantum/types.h"

namespace sep::memory {
namespace shim = ::sep::shim;
using ::sep::SEPResult;

class MemoryTier {
public:
    struct Config {
        TierType type{TierType::HOST};
        std::size_t size{0};
    };

    explicit MemoryTier(const Config& config);

    // Pattern management constructor
    MemoryTier(TierType type, size_t max_patterns, float coherence_threshold, int min_generations);

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
    TierType getType() const {
        return config_.type;
    }
    std::size_t getSize() const {
        return config_.size;
    }

    // Pattern management methods
    bool canAcceptPattern(const sep::pattern::PatternData& pattern) const;
    void addPattern(size_t id, sep::pattern::PatternData pattern);
    void removePattern(size_t id);
    const sep::pattern::PatternData* getPattern(size_t id) const;
    sep::pattern::PatternData* getPattern(size_t id);
    const std::unordered_map<size_t, sep::pattern::PatternData>& getPatterns() const {
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
    std::unordered_map<size_t, sep::pattern::PatternData> m_patterns;
};

}  // namespace sep::memory
