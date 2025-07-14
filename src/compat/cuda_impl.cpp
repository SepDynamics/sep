/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of CUDA functions in the sep::cuda namespace
 */

#include <cuda.h>
#include <cuda_runtime.h>

#include "compat/core.h"
#include "compat/cuda_defs.h"
#include "compat/shim.h"
#include "compat/types.h"
#include "core/common.h"
#include "core/error_handler.h"
#include "cuda_wrapper.h"

using sep::cuda::Error;
using sep::shim::string;

namespace sep {
namespace cuda {

    Error CudaCore::initialize(int device)
    {
        // Basic CUDA device initialization
        cudaError_t cuda_err = cuda_wrapper::SetDevice(device);
        if (cuda_err != cuda_wrapper::Success)
        {
            return Error(Status::Error, string(cuda_wrapper::GetErrorString(cuda_err)),
                         string("CudaCore::initialize"), SEPResult::CUDA_ERROR);
        }
        return Error(Status::Success, string("CUDA initialized successfully"),
                     string("CudaCore::initialize"), SEPResult::SUCCESS);
    }

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

// Device management functions
cudaError_t cudaSetDevice(int device)
{
    return static_cast<cudaError_t>(cuda_wrapper::SetDevice(device));
}

cudaError_t cudaGetDevice(int* device)
{
    return static_cast<cudaError_t>(cuda_wrapper::GetDevice(device));
}

cudaError_t cudaGetDeviceCount(int* count)
{
    return static_cast<cudaError_t>(cuda_wrapper::GetDeviceCount(count));
}

cudaError_t cudaDeviceSynchronize(void)
{
    return static_cast<cudaError_t>(cuda_wrapper::DeviceSynchronize());
}

cudaError_t cudaDeviceReset(void) { return static_cast<cudaError_t>(cuda_wrapper::DeviceReset()); }

cudaError_t cudaSetDeviceFlags(unsigned int flags)
{
    return static_cast<cudaError_t>(cuda_wrapper::SetDeviceFlags(flags));
}

cudaError_t cudaGetDeviceFlags(unsigned int* flags)
{
    return static_cast<cudaError_t>(cuda_wrapper::GetDeviceFlags(flags));
}

cudaError_t cudaGetLastError(void)
{
    return static_cast<cudaError_t>(cuda_wrapper::GetLastError());
}

const char* cudaGetErrorString(cudaError_t error)
{
    return cuda_wrapper::GetErrorString(static_cast<int>(error));
}

} // namespace cuda
} // namespace sep