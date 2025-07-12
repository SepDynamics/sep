/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of CUDA functions in the sep::cuda namespace
 */

#include "cuda_wrapper.h"

// Implementation of CUDA functions in the sep::cuda namespace
namespace sep {
namespace cuda {

// Memory management functions
cudaError_t cudaMemset(void* devPtr, int value, size_t count) {
    return static_cast<cudaError_t>(cuda_wrapper::Memset(devPtr, value, count));
}

cudaError_t cudaMemsetAsync(void* devPtr, int value, size_t count, void* stream) {
    return static_cast<cudaError_t>(cuda_wrapper::MemsetAsync(devPtr, value, count, stream));
}

cudaError_t cudaMemGetInfo(size_t* free, size_t* total) {
    return static_cast<cudaError_t>(cuda_wrapper::MemGetInfo(free, total));
}

cudaError_t cudaMallocHost(void** ptr, size_t size) {
    return static_cast<cudaError_t>(cuda_wrapper::MallocHost(ptr, size));
}

cudaError_t cudaFreeHost(void* ptr) {
    return static_cast<cudaError_t>(cuda_wrapper::FreeHost(ptr));
}

cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind) {
    return static_cast<cudaError_t>(cuda_wrapper::Memcpy(dst, src, count,
        static_cast<cudaMemcpyKind>(kind)));
}

cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, void* stream) {
    return static_cast<cudaError_t>(cuda_wrapper::MemcpyAsync(dst, src, count,
        static_cast<cudaMemcpyKind>(kind), stream));
}

} // namespace cuda
} // namespace sep