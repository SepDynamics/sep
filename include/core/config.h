#pragma once

// Core configuration macros and constants
#ifndef SEP_CUDA_AVAILABLE
#define SEP_CUDA_AVAILABLE 0
#endif

namespace sep {
namespace config {

// Version information
constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 1;
constexpr int VERSION_PATCH = 0;

// System configuration
constexpr bool ENABLE_DEBUG_LOGGING = false;
constexpr bool ENABLE_METRICS = true;
constexpr bool ENABLE_TRACING = false;

// Memory configuration
constexpr size_t DEFAULT_MEMORY_POOL_SIZE = 1024 * 1024 * 1024;  // 1GB
constexpr size_t MIN_ALLOCATION_SIZE = 256;                       // 256B
constexpr size_t MAX_ALLOCATION_SIZE = 1024 * 1024 * 1024;       // 1GB

}  // namespace config
}  // namespace sep