#pragma once

#include <cuda_runtime.h>
#include <cstddef>  // For size_t
#include <cstdio>   // For fprintf
#include <cstdlib>  // For malloc/free

#include <cstring>  // For strcpy, memcpy, memset

#include "cuda_wrappers.h"  // For proper CUDA type definitions

#ifndef SEP_HD
#define SEP_HD __host__ __device__
#endif

// Structure to hold memory copy parameters to avoid similar adjacent parameters
struct CudaMemcpyParams {
    void* destination;
    const void* source;
    size_t sizeInBytes;
    sep::cuda::cudaMemcpyKind direction;
    sep::cuda::cudaStream_t stream;
};

// Helper function for memory copies to avoid parameter similarity issues
inline sep::cuda::cudaError_t performCudaMemcpyAsync(const CudaMemcpyParams& params) {
    // Use the wrapper function to work both with CUDA and stub builds
    return cudaMemcpyAsync(params.destination, params.source,
                        params.sizeInBytes, params.direction,
                        params.stream);
}

namespace sep {
namespace cuda {

// Asynchronous memory copy using the helper function
inline cudaError_t cudaMemcpyAsyncImpl(void* dst, const void* src, size_t count, sep::cuda::cudaMemcpyKind kind, sep::cuda::cudaStream_t stream) {
    return performCudaMemcpyAsync({dst, src, count, kind, stream});
}

} // namespace cuda
} // namespace sep
