#ifndef SEP_COMPAT_CUDA_H
#define SEP_COMPAT_CUDA_H

// Standard includes
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

// CUDA includes in proper order to avoid redefinitions
#include "compat/cuda_types.h"         // Basic type definitions first
#include "compat/cuda_unified_fix.h"   // Compatibility fixes
#include "compat/cuda_runtime.h"       // Runtime functions
#include "compat/macros.h"             // General macros

// This header is the main entry point for CUDA functionality in the SEP Engine.
// It provides a simplified interface for all CUDA operations.

namespace sep {
namespace cuda {

// Helper function to check if CUDA is available at runtime
inline bool isCudaAvailable() {
#if SEP_ENGINE_HAS_CUDA
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);
    return (error == cudaSuccess && deviceCount > 0);
#else
    return false;
#endif
}

// Helper function to get error message
inline const char* getErrorString(cudaError_t error) {
    return cudaGetErrorString(error);
}

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_H
