/*
 * Copyright (c) 2025 SEP Engine Contributors
 *
 * Implementation of HIP functions using our wrapper functions
 */

#include "hip_runtime_wrapper.h"
#include "hip_wrapper.h"

// Implement HIP functions using our wrapper functions
extern "C" {

// Module functions
hipError_t hipModuleLoadData(void** module, const void* image) {
    return hip_wrapper::ModuleLoadData(module, image);
}

hipError_t hipModuleUnload(void* module) {
    return hip_wrapper::ModuleUnload(module);
}

hipError_t hipModuleGetFunction(void** function, void* module, const char* name) {
    return hip_wrapper::ModuleGetFunction(function, module, name);
}

hipError_t hipModuleGetGlobal(void** dptr, size_t* bytes, void* module, const char* name) {
    return hip_wrapper::ModuleGetGlobal(dptr, bytes, module, name);
}

hipError_t hipModuleLaunchKernel(void* function, unsigned int gridDimX, unsigned int gridDimY,
                                unsigned int gridDimZ, unsigned int blockDimX,
                                unsigned int blockDimY, unsigned int blockDimZ,
                                unsigned int sharedMemBytes, hipStream_t stream,
                                void** kernelParams, void** extra) {
    return hip_wrapper::ModuleLaunchKernel(function, gridDimX, gridDimY, gridDimZ, blockDimX,
                                          blockDimY, blockDimZ, sharedMemBytes, stream,
                                          kernelParams, extra);
}

hipError_t hipModuleOccupancyMaxPotentialBlockSize(int* gridSize, int* blockSize,
                                                 void* function, size_t dynamicSMemSize,
                                                 int blockSizeLimit) {
    return hip_wrapper::ModuleOccupancyMaxPotentialBlockSize(gridSize, blockSize, function,
                                                           dynamicSMemSize, blockSizeLimit);
}

// Device functions
hipError_t hipDeviceCanAccessPeer(int* canAccessPeer, int device, int peerDevice) {
    return hip_wrapper::DeviceCanAccessPeer(canAccessPeer, device, peerDevice);
}

hipError_t hipDeviceGetAttribute(int* value, int attr, int device) {
    return hip_wrapper::DeviceGetAttribute(value, attr, device);
}

hipError_t hipDeviceGetName(char* name, int len, int device) {
    return hip_wrapper::DeviceGetName(name, len, device);
}

hipError_t hipDeviceGetP2PAttribute(int* value, int attr, int srcDevice, int dstDevice) {
    return hip_wrapper::DeviceGetP2PAttribute(value, attr, srcDevice, dstDevice);
}

hipError_t hipDeviceGet(int* device) {
    return hip_wrapper::DeviceGet(device);
}

hipError_t hipDeviceGetCount(int* count) {
    return hip_wrapper::DeviceGetCount(count);
}

hipError_t hipDeviceSynchronize() {
    return hip_wrapper::DeviceSynchronize();
}

// Context functions
hipError_t hipCtxCreate(void** ctx, unsigned int flags, int device) {
    return hip_wrapper::CtxCreate(ctx, flags, device);
}

hipError_t hipCtxDestroy(void* ctx) {
    return hip_wrapper::CtxDestroy(ctx);
}

hipError_t hipCtxPushCurrent(void* ctx) {
    return hip_wrapper::CtxPushCurrent(ctx);
}

hipError_t hipCtxPopCurrent(void** ctx) {
    return hip_wrapper::CtxPopCurrent(ctx);
}

hipError_t hipCtxEnablePeerAccess(void* peerCtx, unsigned int flags) {
    return hip_wrapper::CtxEnablePeerAccess(peerCtx, flags);
}

// Memory functions
hipError_t hipHostMalloc(void** ptr, size_t size, unsigned int flags) {
    return hip_wrapper::HostMalloc(ptr, size, flags);
}

hipError_t hipHostFree(void* ptr) {
    return hip_wrapper::HostFree(ptr);
}

hipError_t hipHostGetDevicePointer(void** devPtr, void* hostPtr, unsigned int flags) {
    return hip_wrapper::HostGetDevicePointer(devPtr, hostPtr, flags);
}

hipError_t hipMalloc(void** ptr, size_t size) {
    return hip_wrapper::Malloc(ptr, size);
}

hipError_t hipFree(void* ptr) {
    return hip_wrapper::Free(ptr);
}

hipError_t hipMemGetInfo(size_t* free, size_t* total) {
    return hip_wrapper::MemGetInfo(free, total);
}

hipError_t hipMemcpyHtoD(void* dst, const void* src, size_t size) {
    return hip_wrapper::MemcpyHtoD(dst, src, size);
}

hipError_t hipMemcpyDtoH(void* dst, const void* src, size_t size) {
    return hip_wrapper::MemcpyDtoH(dst, src, size);
}

hipError_t hipMemcpyHtoDAsync(void* dst, const void* src, size_t size, hipStream_t stream) {
    return hip_wrapper::MemcpyHtoDAsync(dst, src, size, stream);
}

hipError_t hipMemcpyDtoHAsync(void* dst, const void* src, size_t size, hipStream_t stream) {
    return hip_wrapper::MemcpyDtoHAsync(dst, src, size, stream);
}

hipError_t hipMemsetD8(void* dst, unsigned char value, size_t count) {
    return hip_wrapper::MemsetD8(dst, value, count);
}

hipError_t hipMemsetD8Async(void* dst, unsigned char value, size_t count, hipStream_t stream) {
    return hip_wrapper::MemsetD8Async(dst, value, count, stream);
}

// Array functions
hipError_t hipArray3DCreate(void** array, const void* desc, const void* extent) {
    return hip_wrapper::Array3DCreate(array, desc, extent);
}

hipError_t hipArrayDestroy(void* array) {
    return hip_wrapper::ArrayDestroy(array);
}

// Texture functions
hipError_t hipTexObjectCreate(unsigned long long* texObject, const void* resDesc, 
                             const void* texDesc, const void* resViewDesc) {
    return hip_wrapper::TexObjectCreate(texObject, resDesc, texDesc, resViewDesc);
}

hipError_t hipTexObjectDestroy(unsigned long long texObject) {
    return hip_wrapper::TexObjectDestroy(texObject);
}

// Graphics functions
hipError_t hipGraphicsGLRegisterBuffer(void** resource, unsigned int buffer, unsigned int flags) {
    return hip_wrapper::GraphicsGLRegisterBuffer(resource, buffer, flags);
}

hipError_t hipGraphicsUnregisterResource(void* resource) {
    return hip_wrapper::GraphicsUnregisterResource(resource);
}

hipError_t hipGraphicsMapResources(int count, void** resources, hipStream_t stream) {
    return hip_wrapper::GraphicsMapResources(count, resources, stream);
}

hipError_t hipGraphicsUnmapResources(int count, void** resources, hipStream_t stream) {
    return hip_wrapper::GraphicsUnmapResources(count, resources, stream);
}

hipError_t hipGraphicsResourceGetMappedPointer(void** devPtr, size_t* size, void* resource) {
    return hip_wrapper::GraphicsResourceGetMappedPointer(devPtr, size, resource);
}

// Stream functions
hipError_t hipStreamCreateWithFlags(hipStream_t* stream, unsigned int flags) {
    return (hipError_t)sep::cuda::cudaStreamCreateWithFlags((void**)stream, flags);
}

hipError_t hipStreamDestroy(hipStream_t stream) {
    return (hipError_t)sep::cuda::cudaStreamDestroy(stream);
}

hipError_t hipStreamSynchronize(hipStream_t stream) {
    return (hipError_t)sep::cuda::cudaStreamSynchronize(stream);
}

hipError_t hipStreamWaitEvent(hipStream_t stream, hipEvent_t event, unsigned int flags) {
    return (hipError_t)sep::cuda::cudaStreamWaitEvent(stream, event, flags);
}

// Event functions
hipError_t hipEventCreate(hipEvent_t* event) {
    return (hipError_t)sep::cuda::cudaEventCreate((void**)event);
}

hipError_t hipEventDestroy(hipEvent_t event) {
    return (hipError_t)sep::cuda::cudaEventDestroy(event);
}

hipError_t hipEventSynchronize(hipEvent_t event) {
    return (hipError_t)sep::cuda::cudaEventSynchronize(event);
}

hipError_t hipEventRecord(hipEvent_t event, hipStream_t stream) {
    return (hipError_t)sep::cuda::cudaEventRecord(event, stream);
}

// Driver functions
hipError_t hipDrvMemcpy2DUnaligned(void* dst, size_t dpitch, const void* src, size_t spitch,
                                  size_t width, size_t height, unsigned int kind) {
    return hip_wrapper::DrvMemcpy2DUnaligned(dst, dpitch, src, spitch, width, height, kind);
}

hipError_t hipDrvMemcpy3D(const void* pCopy) {
    return hip_wrapper::DrvMemcpy3D(pCopy);
}

// Runtime functions
hipError_t hipRuntimeGetVersion(int* runtimeVersion) {
    return hip_wrapper::RuntimeGetVersion(runtimeVersion);
}

// Initialization functions
hipError_t hipInit() {
    return hip_wrapper::Init();
}

// Error string functions
const char* hipGetErrorString(hipError_t error) {
    // Cast to hip_wrapper::hipError_t to avoid type mismatch
    return hip_wrapper::ErrorString(static_cast<hip_wrapper::hipError_t>(error));
}

// HIP-specific functions
const char* hipewErrorString(int error) {
    // Mark parameter as unused to avoid warning
    (void)error;
    return "HIP error (stub)";
}

const char* hipewCompilerPath() {
    return hip_wrapper::CompilerPath();
}

int hipewCompilerVersion() {
    return hip_wrapper::CompilerVersion();
}

hipError_t hipewInit() {
    return hip_wrapper::Success;
}

// CUDA-specific functions
const char* cuewErrorString(int error) {
    // Mark parameter as unused to avoid warning
    (void)error;
    return "CUDA error (stub)";
}

int cuewCompilerVersion() {
    return 120; // Version 12.0
}

const char* cuewCompilerPath() {
    return "/usr/local/cuda/bin/nvcc";
}

hipError_t cuewInit() {
    return hip_wrapper::Success;
}

hipError_t hipFuncSetCacheConfig(const void* func, int config) {
    (void)func;
    (void)config;
    return hip_wrapper::Success;
}

} // extern "C"