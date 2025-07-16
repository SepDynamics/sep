/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of CUDA functions in the sep::cuda namespace
 */

#include <cuda.h>
#include <cuda_runtime.h>

#include "compat/cuda_unified.h"
#include "core/common.h"
#include "core/error_handler.h"

using sep::cuda::Error;
using sep::shim::string;

namespace sep {
namespace cuda {

    Error CudaCore::initialize(int device)
    {
        // Basic CUDA device initialization
        cudaError_t cuda_err = cudaSetDevice(device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(cudaGetErrorString(cuda_err)),
                         string("CudaCore::initialize"), SEPResult::CUDA_ERROR);
        }
        return Error(Status::Success, string("CUDA initialized successfully"),
                     string("CudaCore::initialize"), SEPResult::SUCCESS);
    }

// Memory management functions - these are implemented in the global namespace
// but call the sep::cuda namespace versions
} // namespace cuda
} // namespace sep

// Global namespace implementations - using the new namespace-based functions
cudaError_t cudaMemset(void* devPtr, int value, size_t count) {
    return sep::cuda::cudaMemset(devPtr, value, count);
}

cudaError_t cudaMemsetAsync(void* devPtr, int value, size_t count, void* stream) {
    return sep::cuda::cudaMemsetAsync(devPtr, value, count, 
                               static_cast<sep::cuda::cudaStream_t>(stream));
}

cudaError_t cudaMemGetInfo(size_t* free, size_t* total) {
    return sep::cuda::cudaMemGetInfo(free, total);
}

cudaError_t cudaMallocHost(void** ptr, size_t size) {
    return sep::cuda::cudaMallocHost(ptr, size);
}

cudaError_t cudaFreeHost(void* ptr) { 
    return sep::cuda::cudaFreeHost(ptr); 
}

cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind) {
    return sep::cuda::cudaMemcpy(dst, src, count, 
                           static_cast<sep::cuda::cudaMemcpyKind>(kind));
}

cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, void* stream) {
    return sep::cuda::cudaMemcpyAsync(dst, src, count, 
                                static_cast<sep::cuda::cudaMemcpyKind>(kind),
                                static_cast<sep::cuda::cudaStream_t>(stream));
}

// Device management functions
cudaError_t cudaSetDevice(int device) { 
    return sep::cuda::cudaSetDevice(device); 
}

cudaError_t cudaGetDevice(int* device) { 
    return sep::cuda::cudaGetDevice(device); 
}

cudaError_t cudaGetDeviceCount(int* count) { 
    return sep::cuda::cudaGetDeviceCount(count); 
}

cudaError_t cudaDeviceSynchronize(void) { 
    return sep::cuda::cudaDeviceSynchronize(); 
}

cudaError_t cudaDeviceReset(void) { 
    return sep::cuda::cudaDeviceReset(); 
}

cudaError_t cudaSetDeviceFlags(unsigned int flags)
{
    return sep::cuda::cudaSetDeviceFlags(flags);
}

cudaError_t cudaGetDeviceFlags(unsigned int* flags)
{
    return sep::cuda::cudaGetDeviceFlags(flags);
}

cudaError_t cudaGetLastError(void) { 
    return sep::cuda::cudaGetLastError(); 
}

const char* cudaGetErrorString(cudaError_t error)
{
    return sep::cuda::cudaGetErrorString(error);
}