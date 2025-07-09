#pragma once


// Thread block size for CUDA kernels
#ifndef PATTERN_BLOCK_SIZE
#define PATTERN_BLOCK_SIZE 256
#endif

#include <cstdint>

namespace sep {
namespace cuda {
namespace constants {
inline constexpr std::uint32_t get_default_block_size() {
    return PATTERN_BLOCK_SIZE;
}
inline constexpr std::uint32_t MAX_BLOCK_SIZE = 1024;
inline constexpr std::uint32_t BITFIELD_WORDS = 64;
inline constexpr std::uint32_t SYMMETRY_PAIRS = 32;
inline constexpr std::uint32_t DEFAULT_BLOCK_SIZE = PATTERN_BLOCK_SIZE;
inline constexpr std::uint32_t WARP_SIZE = 32;
} // namespace constants
} // namespace cuda
} // namespace sep

// CUDA memory management
#ifndef CUDA_MIN_MEMORY
#define CUDA_MIN_MEMORY (64UL * 1024UL * 1024UL)
#endif

#ifndef CUDA_MAX_MEMORY
#define CUDA_MAX_MEMORY (2UL * 1024UL * 1024UL * 1024UL)
#endif

// Pattern processing parameters (avoid conflicts with math_common.h)
#ifndef CUDA_DEFAULT_EVOLUTION_RATE
#define CUDA_DEFAULT_EVOLUTION_RATE 0.1f
#endif

#ifndef CUDA_DEFAULT_TIME_DELTA
#define CUDA_DEFAULT_TIME_DELTA 0.016f
#endif

#ifndef CUDA_MAX_PATTERNS_PER_BATCH
#define CUDA_MAX_PATTERNS_PER_BATCH (1024 * 1024)
#endif

// Aliases for backward compatibility
#ifndef DEFAULT_EVOLUTION_RATE
#define DEFAULT_EVOLUTION_RATE CUDA_DEFAULT_EVOLUTION_RATE
#endif

#ifndef DEFAULT_TIME_DELTA
#define DEFAULT_TIME_DELTA CUDA_DEFAULT_TIME_DELTA
#endif
