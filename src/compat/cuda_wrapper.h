#pragma once

#include <cuda_runtime_api.h>

namespace sep::cuda {

inline cudaError_t SEP_cudaStreamCreateWithFlags(cudaStream_t* stream, unsigned int flags) {
    return ::cudaStreamCreateWithFlags(stream, flags);
}

inline cudaError_t SEP_cudaStreamDestroy(cudaStream_t stream) {
    return ::cudaStreamDestroy(stream);
}

inline cudaError_t SEP_cudaStreamSynchronize(cudaStream_t stream) {
    return ::cudaStreamSynchronize(stream);
}

inline cudaError_t SEP_cudaStreamWaitEvent(cudaStream_t stream, cudaEvent_t event, unsigned int flags) {
    return ::cudaStreamWaitEvent(stream, event, flags);
}

inline cudaError_t SEP_cudaStreamAttachMemAsync(cudaStream_t stream, void* devPtr, size_t length, unsigned int flags) {
    return ::cudaStreamAttachMemAsync(stream, devPtr, length, flags);
}

inline cudaError_t SEP_cudaEventCreate(cudaEvent_t* event) {
    return ::cudaEventCreate(event);
}

inline cudaError_t SEP_cudaEventDestroy(cudaEvent_t event) {
    return ::cudaEventDestroy(event);
}

inline cudaError_t SEP_cudaEventSynchronize(cudaEvent_t event) {
    return ::cudaEventSynchronize(event);
}

inline cudaError_t SEP_cudaEventRecord(cudaEvent_t event, cudaStream_t stream) {
    return ::cudaEventRecord(event, stream);
}

inline cudaError_t SEP_cudaMalloc(void** devPtr, size_t size) {
    return ::cudaMalloc(devPtr, size);
}

inline cudaError_t SEP_cudaFree(void* devPtr) {
    return ::cudaFree(devPtr);
}

inline cudaError_t SEP_cudaMallocManaged(void** devPtr, size_t size, unsigned int flags) {
    return ::cudaMallocManaged(devPtr, size, flags);
}

inline cudaError_t SEP_cudaMallocHost(void** ptr, size_t size) {
    return ::cudaMallocHost(ptr, size);
}

inline cudaError_t SEP_cudaFreeHost(void* ptr) {
    return ::cudaFreeHost(ptr);
}

inline cudaError_t SEP_cudaMemGetInfo(size_t* free, size_t* total) {
    return ::cudaMemGetInfo(free, total);
}

inline cudaError_t SEP_cudaMemset(void* devPtr, int value, size_t count) {
    return ::cudaMemset(devPtr, value, count);
}

inline cudaError_t SEP_cudaMemsetAsync(void* devPtr, int value, size_t count, cudaStream_t stream) {
    return ::cudaMemsetAsync(devPtr, value, count, stream);
}

inline cudaError_t SEP_cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind) {
    return ::cudaMemcpy(dst, src, count, kind);
}

inline cudaError_t SEP_cudaMemcpyAsync(void* dst, const void* src, size_t count, cudaMemcpyKind kind, cudaStream_t stream) {
    return ::cudaMemcpyAsync(dst, src, count, kind, stream);
}

inline cudaError_t SEP_cudaSetDevice(int device) {
    return ::cudaSetDevice(device);
}

inline cudaError_t SEP_cudaGetDevice(int* device) {
    return ::cudaGetDevice(device);
}

inline cudaError_t SEP_cudaGetDeviceCount(int* count) {
    return ::cudaGetDeviceCount(count);
}

inline cudaError_t SEP_cudaDeviceSynchronize() {
    return ::cudaDeviceSynchronize();
}

inline cudaError_t SEP_cudaDeviceReset() {
    return ::cudaDeviceReset();
}

inline cudaError_t SEP_cudaSetDeviceFlags(unsigned int flags) {
    return ::cudaSetDeviceFlags(flags);
}

inline cudaError_t SEP_cudaGetDeviceFlags(unsigned int* flags) {
    return ::cudaGetDeviceFlags(flags);
}

inline cudaError_t SEP_cudaGetLastError() {
    return ::cudaGetLastError();
}

inline const char* SEP_cudaGetErrorString(cudaError_t error) {
    return ::cudaGetErrorString(error);
}

} // namespace sep::cuda
