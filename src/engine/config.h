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

}  // namespace config
}  // namespace sep