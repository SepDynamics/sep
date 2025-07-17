#ifndef SEP_COMPAT_CUDA_UNIFIED_H
#define SEP_COMPAT_CUDA_UNIFIED_H

/**
 * @file cuda_unified.h
 * @brief Unified CUDA compatibility layer
 *
 * This is the main header that should be included by any component
 * that needs CUDA functionality. It provides a consistent API
 * that works whether CUDA is available or not.
 *
 * The layer is organized as follows:
 * 1. Type definitions (cuda_types.h)
 * 2. Compatibility fixes (cuda_unified_fix.h)
 * 3. Function declarations (cuda_functions.h)
 * 4. Runtime implementations (cuda_runtime.h)
 */

#include <stdio.h>  // For fprintf and stderr

// Include in correct order to avoid redefinitions
#include "compat/cuda_types.h"        // Basic type definitions first
#include "compat/cuda_unified_fix.h"  // Compatibility fixes second
#include "compat/cuda_functions.h"    // Function declarations third
#include "compat/cuda_runtime.h"      // Runtime implementations last

namespace sep {
namespace cuda {

// Utility function for error checking and logging
inline void checkCudaError(cudaError_t result, const char* operation) {
    if (result != cudaSuccess) {
        // Use platform-independent error handling
        fprintf(stderr, "CUDA Error in %s: %s\n", operation, cudaGetErrorString(result));
    }
}

// Convenience macro for error checking
#ifndef CUDA_CHECK
#define CUDA_CHECK(call)                                    \
    do {                                                    \
        cudaError_t result = call;                          \
        ::sep::cuda::checkCudaError(result, #call);         \
        if (result != ::sep::cuda::cudaSuccess) return result; \
    } while(0)
#endif

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_UNIFIED_H