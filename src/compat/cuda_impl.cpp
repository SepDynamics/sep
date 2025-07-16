/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of CUDA functions in the sep::cuda namespace
 */

#include <cuda.h>
#include <cuda_runtime.h>

#include "compat/core.h"
#include "compat/cuda_defs.h"
#include "compat/cuda_wrappers.h"
#include "compat/shim.h"
#include "compat/types.h"
#include "core/common.h"
#include "core/error_handler.h"

using sep::cuda::Error;
using sep::shim::string;

namespace sep {
namespace cuda {

    Error CudaCore::initialize(int device)
    {
        // Basic CUDA device initialization
        cudaError_t cuda_err = sep::cuda::SEP_cudaSetDevice(device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(sep::cuda::SEP_cudaGetErrorString(cuda_err)),
                         string("CudaCore::initialize"), SEPResult::CUDA_ERROR);
        }
        return Error(Status::Success, string("CUDA initialized successfully"),
                     string("CudaCore::initialize"), SEPResult::SUCCESS);
    }

// Memory management functions
cudaError_t cudaMemset(void* devPtr, int value, size_t count) {
    return sep::cuda::SEP_cudaMemset(devPtr, value, count);
}

cudaError_t cudaMemsetAsync(void* devPtr, int value, size_t count, void* stream) {
    return sep::cuda::SEP_cudaMemsetAsync(devPtr, value, count, stream);
}

cudaError_t cudaMemGetInfo(size_t* free, size_t* total) {
    return sep::cuda::SEP_cudaMemGetInfo(free, total);
}

cudaError_t cudaMallocHost(void** ptr, size_t size) {
    return sep::cuda::SEP_cudaMallocHost(ptr, size);
}

cudaError_t cudaFreeHost(void* ptr) { return sep::cuda::SEP_cudaFreeHost(ptr); }

cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind) {
    return sep::cuda::SEP_cudaMemcpy(dst, src, count, kind);
}

cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, void* stream) {
    return sep::cuda::SEP_cudaMemcpyAsync(dst, src, count, kind, stream);
}

// Device management functions
cudaError_t cudaSetDevice(int device) { return sep::cuda::SEP_cudaSetDevice(device); }

cudaError_t cudaGetDevice(int* device) { return sep::cuda::SEP_cudaGetDevice(device); }

cudaError_t cudaGetDeviceCount(int* count) { return sep::cuda::SEP_cudaGetDeviceCount(count); }

cudaError_t cudaDeviceSynchronize(void) { return sep::cuda::SEP_cudaDeviceSynchronize(); }

cudaError_t cudaDeviceReset(void) { return sep::cuda::SEP_cudaDeviceReset(); }

cudaError_t cudaSetDeviceFlags(unsigned int flags)
{
    return sep::cuda::SEP_cudaSetDeviceFlags(flags);
}

cudaError_t cudaGetDeviceFlags(unsigned int* flags)
{
    return sep::cuda::SEP_cudaGetDeviceFlags(flags);
}

cudaError_t cudaGetLastError(void) { return sep::cuda::SEP_cudaGetLastError(); }

const char* cudaGetErrorString(cudaError_t error)
{
    return sep::cuda::SEP_cudaGetErrorString(error);
}

} // namespace cuda
} // namespace sep