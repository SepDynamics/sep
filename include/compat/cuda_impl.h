#pragma once

#include <stddef.h>  // For size_t
#include <stdio.h>   // For fprintf
#include <stdlib.h>  // For malloc/free
#include <string.h>  // For strcpy, memcpy, memset
#include "compat/cuda_defs.h"
#include "compat/cuda_runtime.h"

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
    return sep::cuda::cudaMemcpyAsync(params.destination, params.source, params.sizeInBytes, params.direction, params.stream);
}

// Asynchronous memory copy using the helper function
inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
    return performCudaMemcpyAsync({dst, src, count, kind, stream});
}

extern "C" {
// Memory management functions
cudaError_t cudaMemset(void* devPtr, int value, size_t count);
cudaError_t cudaMallocManaged(void** ptr, size_t size);

// Memory copy functions
cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind);
} // extern "C"
