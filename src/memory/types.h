#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <vector>
#include "../core/types.h"

namespace sep {
namespace memory {

    // Memory tier types that are used across multiple modules
    enum class MemoryTierEnum : int
    {
        // Logical memory tiers
        STM,
        MTM,
        LTM,
        // Physical memory locations
        HOST = 100,    // Host memory (CPU)
        DEVICE = 101,  // Device memory (GPU)
        UNIFIED = 102  // Unified memory (accessible by both CPU and GPU)
    };

    // Forward declarations
    namespace persistence
    {
        class IRedisManager;
}

// Convert MemoryTierEnum to string representation
inline std::string memoryTierToString(MemoryTierEnum tier) {
    switch (tier) {
        case MemoryTierEnum::STM: return "stm";
        case MemoryTierEnum::MTM: return "mtm";
        case MemoryTierEnum::LTM: return "ltm";
        case MemoryTierEnum::HOST: return "host";
        case MemoryTierEnum::DEVICE: return "device";
        case MemoryTierEnum::UNIFIED: return "unified";
        default: return "unknown";
    }
}

// Convert string to MemoryTierEnum
inline MemoryTierEnum stringToMemoryTier(const std::string& tier) {
    if (tier == "stm") return MemoryTierEnum::STM;
    if (tier == "mtm") return MemoryTierEnum::MTM;
    if (tier == "ltm") return MemoryTierEnum::LTM;
    if (tier == "host") return MemoryTierEnum::HOST;
    if (tier == "device") return MemoryTierEnum::DEVICE;
    if (tier == "unified") return MemoryTierEnum::UNIFIED;
    throw std::invalid_argument("Invalid memory tier string: " + tier);
}

} // namespace memory
} // namespace sep
