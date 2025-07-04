#pragma once

#include <stddef.h>  // For size_t
#include <stdio.h>   // For fprintf
#include <stdlib.h>  // For malloc/free
#include <string.h>  // For strcpy, memcpy, memset
#include "compat/cuda_runtime.h"  // Must come before cuda_defs.h
#include "compat/cuda_defs.h"

#ifndef SEP_HD
#define SEP_HD __host__ __device__
#endif

// Structure to hold memory copy parameters to avoid similar adjacent parameters
struct CudaMemcpyParams {
    void* destination;
    const void* source;
    size_t sizeInBytes;
    cudaMemcpyKind direction;
    cudaStream_t stream;
};

// Helper function for memory copies to avoid parameter similarity issues
inline cudaError_t performCudaMemcpyAsync(const CudaMemcpyParams& params) {
    // Use the namespaced wrapper to work both with CUDA and stub builds
    return sep::cuda::cudaMemcpyAsync(params.destination, params.source,
                                      params.sizeInBytes, params.direction,
                                      params.stream);
}

namespace sep {
namespace cuda {

// Asynchronous memory copy using the helper function
inline cudaError_t cudaMemcpyAsyncImpl(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
    return performCudaMemcpyAsync({dst, src, count, kind, stream});
}

} // namespace cuda
} // namespace sep
