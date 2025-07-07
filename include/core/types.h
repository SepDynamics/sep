#ifndef SEP_CONFIG_TYPES_H
#define SEP_CONFIG_TYPES_H

// Standard C headers
#include <cstddef>
#include <cstdint>

// Standard C++ headers
#include <map>
#include <string>
#include <vector>

namespace sep {

// Memory tier types that are used across multiple modules
enum class MemoryTierEnum {
    // Logical memory tiers
    STM, MTM, LTM,
    // Physical memory locations 
    HOST = 100,   // Host memory (CPU)
    DEVICE = 101, // Device memory (GPU) 
    UNIFIED = 102 // Unified memory (accessible by both CPU and GPU)
};

namespace config {

// Default configuration values
constexpr const char* DEFAULT_LOG_LEVEL = "info";
constexpr const char* DEFAULT_LOG_FILE  = "sep.log";
constexpr const char* DEFAULT_LOG_DIR   = "logs";

struct MemoryThresholdConfig {
    float promote_stm_to_mtm{0.7f};
    float promote_mtm_to_ltm{0.9f};
    float demote_threshold{0.3f};
    float fragmentation_threshold{0.3f};
    std::size_t stm_size{1 << 20};
    std::size_t mtm_size{4 << 20};
    std::size_t ltm_size{16 << 20};
    uint32_t stm_to_mtm_min_gen{5};
    uint32_t mtm_to_ltm_min_gen{100};
    bool use_unified_memory{true};
    bool enable_compression{true};
};

} // namespace config

enum class PatternStateEnum {
    UNINITIALIZED = 0,
    INITIALIZING,
    ACTIVE,
    STOPPED,
    ERROR
};

enum class StreamFlags {
    Default = 0,
    NonBlocking = 1
};

// Compression methods used across modules
enum class CompressionMethod {
    None,
    LZ4,
    ZSTD,
    Custom
};

} // namespace sep

#endif // SEP_CONFIG_TYPES_H
