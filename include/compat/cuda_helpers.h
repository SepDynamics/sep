#pragma once

#include "compat/macros.h"
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#include "compat/cuda_helpers.h"
#else
#include "compat/cuda_runtime.h" // Provide cudaSuccess and related stubs
#include "compat/cuda_impl.h"
// Provide minimal definitions when CUDA is unavailable
#ifndef cudaSuccess
constexpr cudaError_t cudaSuccess = 0;
#endif
#ifndef cudaGetErrorString
inline const char* cudaGetErrorString(cudaError_t) { return "CUDA not available"; }
#endif
#endif

#include <cstdio>

#include "compat/cuda_common.h"
#include "compat/macros.h"

// Comprehensive CUDA helper utilities - consolidated from multiple files
namespace sep {
namespace cuda {

SEP_HOST inline void logCudaError(const char* operation, cudaError_t error) {
    if (error != cudaSuccess) {
        (void)std::fprintf(stderr, "CUDA error in %s: %s\n", operation, cudaGetErrorString(error));
    }
}


#ifndef CUDA_CHECK
#define CUDA_CHECK(call)                             \
    do {                                             \
        cudaError_t error = call;                    \
        if (error != cudaSuccess) {                  \
            ::sep::cuda::logCudaError(#call, error); \
        }                                            \
    } while (0)
#endif

// Additional CUDA utility functions that may be moved from other files

}  // namespace cuda
}  // namespace sep
