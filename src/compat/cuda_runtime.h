#ifndef SEP_COMPAT_CUDA_RUNTIME_H
#define SEP_COMPAT_CUDA_RUNTIME_H

// Include proper dependencies in correct order
#include <cstring>  // For memset
#include <cstdio>   // For fprintf
#include <cstdlib>  // For exit

#include "compat/cuda_fwd.h"    // Include forward declarations first
#include "compat/cuda_types.h"  // Include centralized CUDA type definitions

// Include math compatibility header to handle noexcept issues
#include "compat/cuda_math_compat.h"

// Define needed constants that were previously in cuda_constants.h
// Memory attachment flags
constexpr unsigned int SEP_cudaMemAttachGlobal = 0x01;
constexpr unsigned int SEP_cudaMemAttachHost = 0x02;
constexpr unsigned int SEP_cudaMemAttachSingle = 0x04;

// This header provides CUDA runtime function implementations that
// can be used regardless of whether CUDA is available

namespace sep {
namespace cuda {

    // Stream class forward declaration
    class Stream;

// Function declarations with implementations for both CUDA and non-CUDA environments
#if !SEP_ENGINE_HAS_CUDA
    // Non-CUDA stub implementations
    inline cudaError_t cudaStreamDestroy(cudaStream_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline const char* cudaGetErrorString(cudaError_t) { return "CUDA not available"; }
    inline cudaError_t cudaEventCreate(cudaEvent_t*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaEventDestroy(cudaEvent_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaEventRecord(cudaEvent_t, cudaStream_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaEventSynchronize(cudaEvent_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaEventElapsedTime(float*, cudaEvent_t, cudaEvent_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaMemGetInfo(size_t* free, size_t* total) {
        *free = 0;
        *total = 0;
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
    inline cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind)
    {
        if (dst && src && count > 0)
        {
            std::memcpy(dst, src, count);
        }
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
    inline cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind,
                                       cudaStream_t)
    {
        if (dst && src && count > 0)
        {
            std::memcpy(dst, src, count);
        }
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
    inline cudaError_t cudaSetDevice(int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaGetDevice(int*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaGetDeviceCount(int* count) { *count = 0; return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaGetDeviceProperties(cudaDeviceProp* prop, int)
    {
        std::memset(prop, 0, sizeof(cudaDeviceProp));
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
    inline cudaError_t cudaDeviceSynchronize() { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaDeviceReset() { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaSetDeviceFlags(unsigned int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaGetDeviceFlags(unsigned int*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaDeviceSetCacheConfig(int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaDeviceGetCacheConfig(int*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaGetLastError() { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaStreamCreate(cudaStream_t*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaStreamCreateWithFlags(cudaStream_t*, unsigned int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaStreamSynchronize(cudaStream_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaStreamWaitEvent(cudaStream_t, cudaEvent_t, unsigned int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaEventCreateWithFlags(cudaEvent_t*, unsigned int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaMalloc(void**, size_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaFree(void*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaMallocHost(void**, size_t) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaFreeHost(void*) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaMallocManaged(void**, size_t,
                                         unsigned int flags = SEP_cudaMemAttachGlobal)
    {
        (void)flags; // Unused parameter
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
    inline cudaError_t cudaHostAlloc(void**, size_t, unsigned int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaStreamAttachMemAsync(cudaStream_t, void*, size_t, unsigned int) { return (cudaError_t)SEP_CUDA_SUCCESS; }
    inline cudaError_t cudaMemset(void* ptr, int value, size_t count)
    {
        if (ptr && count > 0)
        {
            std::memset(ptr, value, count);
        }
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
    inline cudaError_t cudaMemsetAsync(void* ptr, int value, size_t count, cudaStream_t)
    {
        if (ptr && count > 0)
        {
            std::memset(ptr, value, count);
        }
        return (cudaError_t)SEP_CUDA_SUCCESS;
    }
#endif

    // SEP-prefixed functions (wrappers around the namespace functions)
    // These provide a consistent API regardless of CUDA availability
#if SEP_ENGINE_HAS_CUDA
    // When CUDA is available, use the global namespace functions directly
    inline cudaError_t SEP_cudaStreamDestroy(cudaStream_t stream) { return ::cudaStreamDestroy(stream); }
    inline const char* SEP_cudaGetErrorString(cudaError_t error) { return ::cudaGetErrorString(error); }
    inline cudaError_t SEP_cudaEventCreate(cudaEvent_t* event) { return ::cudaEventCreate(event); }
    inline cudaError_t SEP_cudaEventDestroy(cudaEvent_t event) { return ::cudaEventDestroy(event); }
    inline cudaError_t SEP_cudaEventRecord(cudaEvent_t event, cudaStream_t stream) { return ::cudaEventRecord(event, stream); }
    inline cudaError_t SEP_cudaEventSynchronize(cudaEvent_t event) { return ::cudaEventSynchronize(event); }
    inline cudaError_t SEP_cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end) { return ::cudaEventElapsedTime(ms, start, end); }
    inline cudaError_t SEP_cudaMemGetInfo(size_t* free, size_t* total) { return ::cudaMemGetInfo(free, total); }
    inline cudaError_t SEP_cudaMemcpy(void* dst, const void* src, size_t size, cudaMemcpyKind kind) { return ::cudaMemcpy(dst, src, size, kind); }
    inline cudaError_t SEP_cudaMemcpyAsync(void* dst, const void* src, size_t size, cudaMemcpyKind kind, cudaStream_t stream) { return ::cudaMemcpyAsync(dst, src, size, kind, stream); }
    inline cudaError_t SEP_cudaSetDevice(int device) { return ::cudaSetDevice(device); }
    inline cudaError_t SEP_cudaGetDevice(int* device) { return ::cudaGetDevice(device); }
    inline cudaError_t SEP_cudaGetDeviceCount(int* count) { return ::cudaGetDeviceCount(count); }
    inline cudaError_t SEP_cudaGetDeviceProperties(cudaDeviceProp* prop, int device) { return ::cudaGetDeviceProperties((::cudaDeviceProp*)prop, device); }
    inline cudaError_t SEP_cudaDeviceSynchronize() { return ::cudaDeviceSynchronize(); }
    inline cudaError_t SEP_cudaDeviceReset() { return ::cudaDeviceReset(); }
    inline cudaError_t SEP_cudaSetDeviceFlags(unsigned int flags) { return ::cudaSetDeviceFlags(flags); }
    inline cudaError_t SEP_cudaGetDeviceFlags(unsigned int* flags) { return ::cudaGetDeviceFlags(flags); }
    inline cudaError_t SEP_cudaDeviceSetCacheConfig(int cacheConfig) { return ::cudaDeviceSetCacheConfig((::cudaFuncCache)cacheConfig); }
    inline cudaError_t SEP_cudaDeviceGetCacheConfig(int* cacheConfig) { return ::cudaDeviceGetCacheConfig((::cudaFuncCache*)cacheConfig); }
    inline cudaError_t SEP_cudaGetLastError() { return ::cudaGetLastError(); }
    inline cudaError_t SEP_cudaStreamCreate(cudaStream_t* stream) { return ::cudaStreamCreate(stream); }
    inline cudaError_t SEP_cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags) { return ::cudaStreamCreateWithFlags(stream, flags); }
    inline cudaError_t SEP_cudaStreamSynchronize(cudaStream_t stream) { return ::cudaStreamSynchronize(stream); }
    inline cudaError_t SEP_cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags) { return ::cudaStreamWaitEvent(stream, event, flags); }
    inline cudaError_t SEP_cudaEventCreateWithFlags(cudaEvent_t* event, unsigned int flags) { return ::cudaEventCreateWithFlags(event, flags); }
    inline cudaError_t SEP_cudaMalloc(void** ptr, size_t size) { return ::cudaMalloc(ptr, size); }
    inline cudaError_t SEP_cudaFree(void* ptr) { return ::cudaFree(ptr); }
    inline cudaError_t SEP_cudaMallocHost(void** ptr, size_t size) { return ::cudaMallocHost(ptr, size); }
    inline cudaError_t SEP_cudaFreeHost(void* ptr) { return ::cudaFreeHost(ptr); }
    inline cudaError_t SEP_cudaMallocManaged(void** ptr, size_t size, unsigned int flags = SEP_cudaMemAttachGlobal) { return ::cudaMallocManaged(ptr, size, flags); }
    inline cudaError_t SEP_cudaHostAlloc(void** ptr, size_t size, unsigned int flags) { return ::cudaHostAlloc(ptr, size, flags); }
    inline cudaError_t SEP_cudaStreamAttachMemAsync(cudaStream_t stream, void* ptr, size_t size, unsigned int flags) { return ::cudaStreamAttachMemAsync(stream, ptr, size, flags); }
    inline cudaError_t SEP_cudaMemset(void* devPtr, int value, size_t count) { return ::cudaMemset(devPtr, value, count); }
    inline cudaError_t SEP_cudaMemsetAsync(void* devPtr, int value, size_t count, cudaStream_t stream) { return ::cudaMemsetAsync(devPtr, value, count, stream); }
#else
    // When CUDA is not available, use the stub implementations
    inline cudaError_t SEP_cudaStreamDestroy(cudaStream_t stream) { return cudaStreamDestroy(stream); }
    inline const char* SEP_cudaGetErrorString(cudaError_t error) { return cudaGetErrorString(error); }
    inline cudaError_t SEP_cudaEventCreate(cudaEvent_t* event) { return cudaEventCreate(event); }
    inline cudaError_t SEP_cudaEventDestroy(cudaEvent_t event) { return cudaEventDestroy(event); }
    inline cudaError_t SEP_cudaEventRecord(cudaEvent_t event, cudaStream_t stream) { return cudaEventRecord(event, stream); }
    inline cudaError_t SEP_cudaEventSynchronize(cudaEvent_t event) { return cudaEventSynchronize(event); }
    inline cudaError_t SEP_cudaEventElapsedTime(float* ms, cudaEvent_t start, cudaEvent_t end) { return cudaEventElapsedTime(ms, start, end); }
    inline cudaError_t SEP_cudaMemGetInfo(size_t* free, size_t* total) { return cudaMemGetInfo(free, total); }
    inline cudaError_t SEP_cudaMemcpy(void* dst, const void* src, size_t size, cudaMemcpyKind kind) { return cudaMemcpy(dst, src, size, kind); }
    inline cudaError_t SEP_cudaMemcpyAsync(void* dst, const void* src, size_t size, cudaMemcpyKind kind, cudaStream_t stream) { return cudaMemcpyAsync(dst, src, size, kind, stream); }
    inline cudaError_t SEP_cudaSetDevice(int device) { return cudaSetDevice(device); }
    inline cudaError_t SEP_cudaGetDevice(int* device) { return cudaGetDevice(device); }
    inline cudaError_t SEP_cudaGetDeviceCount(int* count) { return cudaGetDeviceCount(count); }
    inline cudaError_t SEP_cudaGetDeviceProperties(cudaDeviceProp* prop, int device) { return cudaGetDeviceProperties(prop, device); }
    inline cudaError_t SEP_cudaDeviceSynchronize() { return cudaDeviceSynchronize(); }
    inline cudaError_t SEP_cudaDeviceReset() { return cudaDeviceReset(); }
    inline cudaError_t SEP_cudaSetDeviceFlags(unsigned int flags) { return cudaSetDeviceFlags(flags); }
    inline cudaError_t SEP_cudaGetDeviceFlags(unsigned int* flags) { return cudaGetDeviceFlags(flags); }
    inline cudaError_t SEP_cudaDeviceSetCacheConfig(int cacheConfig) { return cudaDeviceSetCacheConfig(cacheConfig); }
    inline cudaError_t SEP_cudaDeviceGetCacheConfig(int* cacheConfig) { return cudaDeviceGetCacheConfig(cacheConfig); }
    inline cudaError_t SEP_cudaGetLastError() { return cudaGetLastError(); }
    inline cudaError_t SEP_cudaStreamCreate(cudaStream_t* stream) { return cudaStreamCreate(stream); }
    inline cudaError_t SEP_cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags) { return cudaStreamCreateWithFlags(stream, flags); }
    inline cudaError_t SEP_cudaStreamSynchronize(cudaStream_t stream) { return cudaStreamSynchronize(stream); }
    inline cudaError_t SEP_cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags) { return cudaStreamWaitEvent(stream, event, flags); }
    inline cudaError_t SEP_cudaEventCreateWithFlags(cudaEvent_t* event, unsigned int flags) { return cudaEventCreateWithFlags(event, flags); }
    inline cudaError_t SEP_cudaMalloc(void** ptr, size_t size) { return cudaMalloc(ptr, size); }
    inline cudaError_t SEP_cudaFree(void* ptr) { return cudaFree(ptr); }
    inline cudaError_t SEP_cudaMallocHost(void** ptr, size_t size) { return cudaMallocHost(ptr, size); }
    inline cudaError_t SEP_cudaFreeHost(void* ptr) { return cudaFreeHost(ptr); }
    inline cudaError_t SEP_cudaMallocManaged(void** ptr, size_t size, unsigned int flags = SEP_cudaMemAttachGlobal) { return cudaMallocManaged(ptr, size, flags); }
    inline cudaError_t SEP_cudaHostAlloc(void** ptr, size_t size, unsigned int flags) { return cudaHostAlloc(ptr, size, flags); }
    inline cudaError_t SEP_cudaStreamAttachMemAsync(cudaStream_t stream, void* ptr, size_t size, unsigned int flags) { return cudaStreamAttachMemAsync(stream, ptr, size, flags); }
    inline cudaError_t SEP_cudaMemset(void* devPtr, int value, size_t count) { return cudaMemset(devPtr, value, count); }
    inline cudaError_t SEP_cudaMemsetAsync(void* devPtr, int value, size_t count, cudaStream_t stream) { return cudaMemsetAsync(devPtr, value, count, stream); }
#endif

} // namespace cuda
} // namespace sep

// Export the SEP-prefixed CUDA functions to global scope
using sep::cuda::SEP_cudaStreamDestroy;
using sep::cuda::SEP_cudaGetErrorString;
using sep::cuda::SEP_cudaEventCreate;
using sep::cuda::SEP_cudaEventDestroy;
using sep::cuda::SEP_cudaEventRecord;
using sep::cuda::SEP_cudaEventSynchronize;
using sep::cuda::SEP_cudaEventElapsedTime;
using sep::cuda::SEP_cudaMemGetInfo;
using sep::cuda::SEP_cudaMemcpy;
using sep::cuda::SEP_cudaMemcpyAsync;
using sep::cuda::SEP_cudaSetDevice;
using sep::cuda::SEP_cudaGetDevice;
using sep::cuda::SEP_cudaGetDeviceCount;
using sep::cuda::SEP_cudaGetDeviceProperties;
using sep::cuda::SEP_cudaDeviceSynchronize;
using sep::cuda::SEP_cudaDeviceReset;
using sep::cuda::SEP_cudaSetDeviceFlags;
using sep::cuda::SEP_cudaGetDeviceFlags;
using sep::cuda::SEP_cudaDeviceSetCacheConfig;
using sep::cuda::SEP_cudaDeviceGetCacheConfig;
using sep::cuda::SEP_cudaGetLastError;
using sep::cuda::SEP_cudaStreamCreate;
using sep::cuda::SEP_cudaStreamCreateWithFlags;
using sep::cuda::SEP_cudaStreamSynchronize;
using sep::cuda::SEP_cudaStreamWaitEvent;
using sep::cuda::SEP_cudaEventCreateWithFlags;
using sep::cuda::SEP_cudaMalloc;
using sep::cuda::SEP_cudaFree;
using sep::cuda::SEP_cudaMallocHost;
using sep::cuda::SEP_cudaFreeHost;
using sep::cuda::SEP_cudaMallocManaged;
using sep::cuda::SEP_cudaHostAlloc;
using sep::cuda::SEP_cudaStreamAttachMemAsync;
using sep::cuda::SEP_cudaMemset;
using sep::cuda::SEP_cudaMemsetAsync;

#endif // SEP_COMPAT_CUDA_RUNTIME_H
