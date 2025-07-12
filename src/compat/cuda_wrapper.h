/*
 * Copyright (c) 2025 SEP Engine Contributors
 * 
 * CUDA wrapper functions to avoid name collisions
 */

#ifndef CUDA_WRAPPER_H
#define CUDA_WRAPPER_H

#include <cuda_runtime.h>

// Wrapper functions for CUDA functions with different names
namespace cuda_wrapper {

// Stream functions
inline cudaError_t StreamCreateWithFlags(void** stream, unsigned int flags) {
    return cudaStreamCreateWithFlags(stream, flags);
}

inline cudaError_t StreamDestroy(void* stream) {
    return cudaStreamDestroy(stream);
}

inline cudaError_t StreamSynchronize(void* stream) {
    return cudaStreamSynchronize(stream);
}

inline cudaError_t StreamWaitEvent(void* stream, void* event, unsigned int flags) {
    return cudaStreamWaitEvent(stream, event, flags);
}

inline cudaError_t StreamAttachMemAsync(void* stream, void* devPtr, size_t length, unsigned int flags) {
    return cudaStreamAttachMemAsync(stream, devPtr, length, flags);
}

// Event functions
inline cudaError_t EventCreate(void** event) {
    return cudaEventCreate(event);
}

inline cudaError_t EventDestroy(void* event) {
    return cudaEventDestroy(event);
}

inline cudaError_t EventSynchronize(void* event) {
    return cudaEventSynchronize(event);
}

inline cudaError_t EventRecord(void* event, void* stream) {
    return cudaEventRecord(event, stream);
}

// Memory management functions
inline cudaError_t Malloc(void** devPtr, size_t size) {
    return cudaMalloc(devPtr, size);
}

inline cudaError_t Free(void* devPtr) {
    return cudaFree(devPtr);
}

inline cudaError_t MallocManaged(void** devPtr, size_t size, unsigned int flags) {
    return cudaMallocManaged(devPtr, size, flags);
}

// Added missing memory functions
inline cudaError_t MallocHost(void** ptr, size_t size) {
    return cudaMallocHost(ptr, size);
}

inline cudaError_t FreeHost(void* ptr) {
    return cudaFreeHost(ptr);
}

inline cudaError_t MemGetInfo(size_t* free, size_t* total) {
    return cudaMemGetInfo(free, total);
}

inline cudaError_t Memset(void* devPtr, int value, size_t count) {
    return cudaMemset(devPtr, value, count);
}

inline cudaError_t MemsetAsync(void* devPtr, int value, size_t count, void* stream) {
    return cudaMemsetAsync(devPtr, value, count, (cudaStream_t)stream);
}

inline cudaError_t Memcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind) {
    return cudaMemcpy(dst, src, count, kind);
}

inline cudaError_t MemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, void* stream) {
    return cudaMemcpyAsync(dst, src, count, kind, (cudaStream_t)stream);
}

// Error handling
inline const char* GetErrorString(int error) {
    return cudaGetErrorString(error);
}

// Constants
static const cudaError_t Success = cudaSuccess;
static const cudaError_t ErrorNotReady = cudaErrorNotReady;
static const cudaError_t ErrorInvalidValue = cudaErrorInvalidValue;

} // namespace cuda_wrapper

#endif // CUDA_WRAPPER_H
