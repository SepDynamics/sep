#ifndef SEP_COMPAT_CUDA_SEP_H
#define SEP_COMPAT_CUDA_SEP_H

// Include proper dependencies in correct order
#include <cuda_runtime.h>
#include "shim.h"
#include <cuda_runtime.h>

// This header provides CUDA runtime function implementations that
// can be used regardless of whether CUDA is available

namespace sep {
namespace sep {

    // Stream class forward declaration
    class Stream;

// When CUDA is available, use the real CUDA functions directly
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
inline cudaError_t SEP_cudaGetDeviceProperties(cudaDeviceProp* prop, int device) { return cudaGetDeviceProperties((::cudaDeviceProp*)prop, device); }
inline cudaError_t SEP_cudaDeviceSynchronize() { return cudaDeviceSynchronize(); }
inline cudaError_t SEP_cudaDeviceReset() { return cudaDeviceReset(); }
inline cudaError_t SEP_cudaSetDeviceFlags(unsigned int flags) { return cudaSetDeviceFlags(flags); }
inline cudaError_t SEP_cudaGetDeviceFlags(unsigned int* flags) { return cudaGetDeviceFlags(flags); }
inline cudaError_t SEP_cudaDeviceSetCacheConfig(int cacheConfig) { return cudaDeviceSetCacheConfig((::cudaFuncCache)cacheConfig); }
inline cudaError_t SEP_cudaDeviceGetCacheConfig(int* cacheConfig) { return cudaDeviceGetCacheConfig((::cudaFuncCache*)cacheConfig); }
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
}  // namespace sep
#endif // SEP_COMPAT_CUDA_SEP_H
