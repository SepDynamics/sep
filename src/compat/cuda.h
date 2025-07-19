#ifndef SEP_COMPAT_CUDA_H
#define SEP_COMPAT_CUDA_H

#include "compat/cuda_unified.h"

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

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_H
