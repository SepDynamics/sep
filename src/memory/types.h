#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include "memory/redis_manager.h"

namespace sep {

namespace memory {

    // Memory tier types that are used across multiple modules
    enum class MemoryTierEnum : int
    {
        // Logical memory tiers
        STM,
        SHORT_TERM = STM,
        SHORT_TERM_MEMORY = SHORT_TERM,
        MTM,
        LTM,
        // Physical memory locations
        HOST = 100,    // Host memory (CPU)
        DEVICE = 101,  // Device memory (GPU)
        UNIFIED = 102  // Unified memory (accessible by both CPU and GPU)
    };

// Convert string to MemoryTierEnum
    inline MemoryTierEnum stringToMemoryTier(const std::string& tier)
    {
        if (tier == "stm") return MemoryTierEnum::STM;
        if (tier == "mtm") return MemoryTierEnum::MTM;
        if (tier == "ltm") return MemoryTierEnum::LTM;
        if (tier == "host") return MemoryTierEnum::HOST;
        if (tier == "device") return MemoryTierEnum::DEVICE;
        if (tier == "unified") return MemoryTierEnum::UNIFIED;
        throw std::invalid_argument(std::string("Invalid memory tier string: ") + tier.c_str());
    }

enum class CompressionMethod : std::uint8_t {
    None,
    ZSTD,
    // Add other methods as needed
};

struct MemoryThresholdConfig {
    std::size_t stm_size;
    std::size_t mtm_size;
    std::size_t ltm_size;
    float promote_stm_to_mtm;
    float promote_mtm_to_ltm;
    float demote_threshold;
    float fragmentation_threshold;
    bool use_unified_memory;
    bool enable_compression;
    int stm_to_mtm_min_gen;
    int mtm_to_ltm_min_gen;
    std::uint32_t pattern_expiration_age{1000};
};

} // namespace memory
} // namespace sep
