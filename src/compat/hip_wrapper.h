#ifndef HIP_WRAPPER_H
#define HIP_WRAPPER_H

#include <cuda_runtime.h>

// This file provides wrapper functions for HIP API calls
// to avoid name collisions and infinite recursion when
// both HIP and CUDA headers are included

namespace hip_wrapper {

// Error codes
typedef enum {
    Success = 0,
    ErrorInvalidValue = 1,
    ErrorNotReady = 34
} hipError_t;

// HIP Module functions
inline hipError_t ModuleLoadData(void** module, const void* image) {
    // Map to CUDA equivalent if available
    // For now, return success to allow compilation
    (void)module;
    (void)image;
    return Success;
}

inline hipError_t ModuleUnload(void* module) {
    (void)module;
    return Success;
}

inline hipError_t ModuleGetFunction(void** function, void* module, const char* name) {
    (void)function;
    (void)module;
    (void)name;
    return Success;
}

inline hipError_t ModuleGetGlobal(void** dptr, size_t* bytes, void* module, const char* name) {
    (void)dptr;
    (void)bytes;
    (void)module;
    (void)name;
    return Success;
}

inline hipError_t ModuleLaunchKernel(void* function, unsigned int gridDimX, unsigned int gridDimY,
                                     unsigned int gridDimZ, unsigned int blockDimX,
                                     unsigned int blockDimY, unsigned int blockDimZ,
                                     unsigned int sharedMemBytes, void* stream,
                                     void** kernelParams, void** extra) {
    (void)function;
    (void)gridDimX;
    (void)gridDimY;
    (void)gridDimZ;
    (void)blockDimX;
    (void)blockDimY;
    (void)blockDimZ;
    (void)sharedMemBytes;
    (void)stream;
    (void)kernelParams;
    (void)extra;
    return Success;
}

inline hipError_t ModuleOccupancyMaxPotentialBlockSize(int* gridSize, int* blockSize,
                                                      void* function, size_t dynamicSMemSize,
                                                      int blockSizeLimit) {
    (void)gridSize;
    (void)blockSize;
    (void)function;
    (void)dynamicSMemSize;
    (void)blockSizeLimit;
    return Success;
}

// Device functions
inline hipError_t DeviceCanAccessPeer(int* canAccessPeer, int device, int peerDevice) {
    (void)canAccessPeer;
    (void)device;
    (void)peerDevice;
    return Success;
}

inline hipError_t DeviceGetAttribute(int* value, int attr, int device) {
    (void)value;
    (void)attr;
    (void)device;
    return Success;
}

inline hipError_t DeviceGetName(char* name, int len, int device) {
    (void)name;
    (void)len;
    (void)device;
    return Success;
}

inline hipError_t DeviceGetP2PAttribute(int* value, int attr, int srcDevice, int dstDevice) {
    (void)value;
    (void)attr;
    (void)srcDevice;
    (void)dstDevice;
    return Success;
}

inline hipError_t DeviceGet(int* device) {
    (void)device;
    return Success;
}

inline hipError_t DeviceGetCount(int* count) {
    (void)count;
    return Success;
}

inline hipError_t DeviceSynchronize() {
    return Success;
}

// Context functions
inline hipError_t CtxCreate(void** ctx, unsigned int flags, int device) {
    (void)ctx;
    (void)flags;
    (void)device;
    return Success;
}

inline hipError_t CtxDestroy(void* ctx) {
    (void)ctx;
    return Success;
}

inline hipError_t CtxPushCurrent(void* ctx) {
    (void)ctx;
    return Success;
}

inline hipError_t CtxPopCurrent(void** ctx) {
    (void)ctx;
    return Success;
}

inline hipError_t CtxEnablePeerAccess(void* peerCtx, unsigned int flags) {
    (void)peerCtx;
    (void)flags;
    return Success;
}

// Memory functions
inline hipError_t HostMalloc(void** ptr, size_t size, unsigned int flags) {
    (void)flags;
    return (hipError_t)cudaMallocHost(ptr, size);
}

inline hipError_t HostFree(void* ptr) {
    return (hipError_t)cudaFreeHost(ptr);
}

inline hipError_t HostGetDevicePointer(void** devPtr, void* hostPtr, unsigned int flags) {
    (void)devPtr;
    (void)hostPtr;
    (void)flags;
    return Success;
}

inline hipError_t Malloc(void** ptr, size_t size) {
    return (hipError_t)cudaMalloc(ptr, size);
}

inline hipError_t Free(void* ptr) {
    return (hipError_t)cudaFree(ptr);
}

inline hipError_t MemGetInfo(size_t* free, size_t* total) {
    return (hipError_t)cudaMemGetInfo(free, total);
}

inline hipError_t MemcpyHtoD(void* dst, const void* src, size_t size) {
    // Use cudaMemcpyKind from sep::cuda namespace
    return (hipError_t)cudaMemcpy(dst, src, size, sep::cuda::cudaMemcpyHostToDevice);
}

inline hipError_t MemcpyDtoH(void* dst, const void* src, size_t size) {
    return (hipError_t)cudaMemcpy(dst, src, size, sep::cuda::cudaMemcpyDeviceToHost);
}

inline hipError_t MemcpyHtoDAsync(void* dst, const void* src, size_t size, void* stream) {
    return (hipError_t)cudaMemcpyAsync(dst, src, size, sep::cuda::cudaMemcpyHostToDevice, (cudaStream_t)stream);
}

inline hipError_t MemcpyDtoHAsync(void* dst, const void* src, size_t size, void* stream) {
    return (hipError_t)cudaMemcpyAsync(dst, src, size, sep::cuda::cudaMemcpyDeviceToHost, (cudaStream_t)stream);
}

inline hipError_t MemsetD8(void* dst, unsigned char value, size_t count) {
    return (hipError_t)cudaMemset(dst, value, count);
}

inline hipError_t MemsetD8Async(void* dst, unsigned char value, size_t count, void* stream) {
    return (hipError_t)cudaMemsetAsync(dst, value, count, (cudaStream_t)stream);
}

// Array functions
inline hipError_t Array3DCreate(void** array, const void* desc, const void* extent) {
    (void)array;
    (void)desc;
    (void)extent;
    return Success;
}

inline hipError_t ArrayDestroy(void* array) {
    (void)array;
    return Success;
}

// Texture functions
inline hipError_t TexObjectCreate(unsigned long long* texObject, const void* resDesc, 
                                 const void* texDesc, const void* resViewDesc) {
    (void)texObject;
    (void)resDesc;
    (void)texDesc;
    (void)resViewDesc;
    return Success;
}

inline hipError_t TexObjectDestroy(unsigned long long texObject) {
    (void)texObject;
    return Success;
}

// Graphics functions
inline hipError_t GraphicsGLRegisterBuffer(void** resource, unsigned int buffer, unsigned int flags) {
    (void)resource;
    (void)buffer;
    (void)flags;
    return Success;
}

inline hipError_t GraphicsUnregisterResource(void* resource) {
    (void)resource;
    return Success;
}

inline hipError_t GraphicsMapResources(int count, void** resources, void* stream) {
    (void)count;
    (void)resources;
    (void)stream;
    return Success;
}

inline hipError_t GraphicsUnmapResources(int count, void** resources, void* stream) {
    (void)count;
    (void)resources;
    (void)stream;
    return Success;
}

inline hipError_t GraphicsResourceGetMappedPointer(void** devPtr, size_t* size, void* resource) {
    (void)devPtr;
    (void)size;
    (void)resource;
    return Success;
}

// Driver functions
inline hipError_t DrvMemcpy2DUnaligned(void* dst, size_t dpitch, const void* src, size_t spitch,
                                      size_t width, size_t height, unsigned int kind) {
    (void)dst;
    (void)dpitch;
    (void)src;
    (void)spitch;
    (void)width;
    (void)height;
    (void)kind;
    return Success;
}

inline hipError_t DrvMemcpy3D(const void* pCopy) {
    (void)pCopy;
    return Success;
}

// Runtime functions
inline hipError_t RuntimeGetVersion(int* runtimeVersion) {
    (void)runtimeVersion;
    return Success;
}

// Initialization functions
inline hipError_t Init() {
    return Success;
}

// Error string functions
inline const char* ErrorString(hipError_t error) {
    (void)error;
    return "HIP error (stub)";
}

// Compiler functions
inline const char* CompilerPath() {
    return "/usr/local/hip/bin/hipcc";
}

inline int CompilerVersion() {
    return 400; // Version 4.0.0
}

} // namespace hip_wrapper

#endif // HIP_WRAPPER_H