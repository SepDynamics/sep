#pragma once

#ifndef SEP_MEMORY_BLOCK_H
#define SEP_MEMORY_BLOCK_H

#include <cstddef>
#include <cstddef>
#include <cstdint>

namespace sep::blender {
enum class CompressionMethod { None, DeltaEncoding, LZ4, ZSTD };
}

class MemoryTier;

namespace sep {

namespace memory {


// Memory tier types
enum class TierType {
    HOST = 0,  // Host memory (CPU)
    DEVICE = 1, // Device memory (GPU)
    UNIFIED = 2 // Unified memory (accessible by both CPU and GPU)
};

// Macros for CUDA kernel compatibility
#ifdef __CUDACC__
#ifndef SEP_MEMORY_TIER_STM
#define SEP_MEMORY_TIER_STM static_cast<int>(sep::MemoryTierEnum::STM)
#endif

#ifndef SEP_MEMORY_TIER_MTM
#define SEP_MEMORY_TIER_MTM static_cast<int>(sep::MemoryTierEnum::MTM)
#endif

#ifndef SEP_MEMORY_TIER_LTM
#define SEP_MEMORY_TIER_LTM static_cast<int>(sep::MemoryTierEnum::LTM)
#endif
#endif

struct MemoryBlock {
    void*                      ptr{nullptr};
    std::size_t                size{0};
    std::size_t                offset{0};
    bool                       allocated{false};
    float                      utilization{0.0f};
    std::size_t                access_count{0};
    TierType                   tier{TierType::HOST};
    blender::CompressionMethod compression{blender::CompressionMethod::None};
    std::size_t                original_size{0};
    float                      stability{0.0f};
    float                      coherence{0.0f};
    std::uint32_t              generation{0};
    float                      weight{0.0f};
    std::uint64_t              wait{0};
    float                      coherence_trend{0.0f};
    float                      last_coherence{0.0f};
    float                      compression_ratio{1.0f};

    MemoryBlock() = default;
    MemoryBlock(void* p, std::size_t s, std::size_t off, TierType t)
        : ptr(p), size(s), offset(off), tier(t), original_size(s) {}
};
}  // namespace memory

// Memory tier enum - for pattern storage
enum class MemoryTierEnum {
    STM = 0,  // Short-term memory
    MTM = 1,  // Medium-term memory
    LTM = 2   // Long-term memory
};
} // namespace sep

#endif  // SEP_MEMORY_BLOCK_H
