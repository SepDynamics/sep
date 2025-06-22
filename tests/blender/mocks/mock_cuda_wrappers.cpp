/**
 * @file mock_cuda_wrappers.cpp
 * @brief Mock implementation of CUDA wrapper functions for testing
 *
 * This file provides mock implementations of the CUDA wrapper functions
 * used by the SEP system. These mocks allow tests to run without requiring
 * actual CUDA hardware or drivers.
 */

#include "cuda/cuda_wrapper.h"
#include "cuda/macros.h"
#include "cuda/raii.h"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

// Mock memory management
namespace {
// Simple memory tracking for mocks
std::map<void*, size_t> g_allocatedMemory;

// Mock stream tracking
std::vector<cudaStream_t> g_mockStreams;

// Mock event tracking
std::vector<cudaEvent_t> g_mockEvents;

// Mock device properties
// Initialize with a simpler approach to avoid compilation errors with structure layout
cudaDeviceProp g_mockDeviceProps;

void initMockDeviceProps()
{
    // Initialize the mock device properties with safe values
    std::memset(&g_mockDeviceProps, 0, sizeof(cudaDeviceProp));

    // Set basic properties
    std::strcpy(g_mockDeviceProps.name, "Mock CUDA Device");
    g_mockDeviceProps.totalGlobalMem     = static_cast<size_t>(1024) * 1024 * 1024;  // 1GB
    g_mockDeviceProps.sharedMemPerBlock  = static_cast<size_t>(48) * 1024;
    g_mockDeviceProps.regsPerBlock       = 32;
    g_mockDeviceProps.warpSize           = 32;
    g_mockDeviceProps.maxThreadsPerBlock = 512;

    // Set array properties
    g_mockDeviceProps.maxThreadsDim[0] = 512;
    g_mockDeviceProps.maxThreadsDim[1] = 512;
    g_mockDeviceProps.maxThreadsDim[2] = 64;

    g_mockDeviceProps.maxGridSize[0] = 65535;
    g_mockDeviceProps.maxGridSize[1] = 65535;
    g_mockDeviceProps.maxGridSize[2] = 65535;

    g_mockDeviceProps.major                = 7;
    g_mockDeviceProps.minor                = 5;
    g_mockDeviceProps.multiProcessorCount  = 16;
    g_mockDeviceProps.concurrentKernels    = 1;
    g_mockDeviceProps.canMapHostMemory     = 1;
    g_mockDeviceProps.pageableMemoryAccess = 1;
};
}  // namespace

// Mock implementations of the C interface functions
extern "C" {

sep::cuda::Status checkCudaErrorWrapper(cudaError_t error, const char* operation)
{
    (void)error;
    (void)operation;
    return sep::cuda::Status::Success;
}

sep::cuda::Status synchronizeDeviceWrapper()
{
    return sep::cuda::Status::Success;
}

sep::cuda::Status createStreamWrapper(cudaStream_t* stream)
{
    // Create a fake stream pointer
    *stream = reinterpret_cast<cudaStream_t>(new int(g_mockStreams.size()));
    g_mockStreams.push_back(*stream);
    return sep::cuda::Status::Success;
}

sep::cuda::Status destroyStreamWrapper(cudaStream_t stream)
{
    // Find and remove the stream
    for (auto it = g_mockStreams.begin(); it != g_mockStreams.end(); ++it)
    {
        if (*it == stream)
        {
            g_mockStreams.erase(it);
            delete reinterpret_cast<int*>(stream);
            break;
        }
    }
    return sep::cuda::Status::Success;
}

sep::cuda::Status synchronizeStreamWrapper(cudaStream_t stream)
{
    (void)stream;
    return sep::cuda::Status::Success;
}

sep::cuda::Status allocateMemoryWrapper(void** devPtr, size_t size)
{
    // Allocate host memory as a stand-in for device memory
    *devPtr = malloc(size);
    if (*devPtr)
    {
        g_allocatedMemory[*devPtr] = size;
        return sep::cuda::Status::Success;
    }
    return sep::cuda::Status::OutOfMemory;
}

sep::cuda::Status freeMemoryWrapper(void* devPtr)
{
    if (g_allocatedMemory.find(devPtr) != g_allocatedMemory.end())
    {
        free(devPtr);
        g_allocatedMemory.erase(devPtr);
        return sep::cuda::Status::Success;
    }
    return sep::cuda::Status::InvalidValue;
}

sep::cuda::Status copyToDeviceWrapper(void* dst, const void* src, size_t count)
{
    if (g_allocatedMemory.find(dst) != g_allocatedMemory.end() && g_allocatedMemory[dst] >= count)
    {
        memcpy(dst, src, count);
        return sep::cuda::Status::Success;
    }
    return sep::cuda::Status::InvalidValue;
}

sep::cuda::Status copyToHostWrapper(void* dst, const void* src, size_t count)
{
    if (g_allocatedMemory.find(const_cast<void*>(src)) != g_allocatedMemory.end()
        && g_allocatedMemory[const_cast<void*>(src)] >= count)
    {
        memcpy(dst, src, count);
        return sep::cuda::Status::Success;
    }
    return sep::cuda::Status::InvalidValue;
}

sep::cuda::Status copyOnDeviceWrapper(void* dst, const void* src, size_t count)
{
    if (g_allocatedMemory.find(dst) != g_allocatedMemory.end()
        && g_allocatedMemory.find(const_cast<void*>(src)) != g_allocatedMemory.end() && g_allocatedMemory[dst] >= count
        && g_allocatedMemory[const_cast<void*>(src)] >= count)
    {
        memcpy(dst, src, count);
        return sep::cuda::Status::Success;
    }
    return sep::cuda::Status::InvalidValue;
}

sep::cuda::Status copyToDeviceAsyncWrapper(void* dst, const void* src, size_t count, cudaStream_t stream)
{
    (void)stream;
    return copyToDeviceWrapper(dst, src, count);
}

sep::cuda::Status copyToHostAsyncWrapper(void* dst, const void* src, size_t count, cudaStream_t stream)
{
    (void)stream;
    return copyToHostWrapper(dst, src, count);
}

sep::cuda::Status copyOnDeviceAsyncWrapper(void* dst, const void* src, size_t count, cudaStream_t stream)
{
    (void)stream;
    return copyOnDeviceWrapper(dst, src, count);
}

}  // extern "C"

// Mock implementation of the CudaWrapper singleton
namespace sep::cuda {

// Static instance accessor
CudaWrapper& CudaWrapper::getInstance()
{
    static CudaWrapper instance;
    return instance;
}

Status CudaWrapper::setDevice(int device)
{
    (void)device;
    return Status::Success;
}

Status CudaWrapper::getDeviceCount(int* count)
{
    *count = 1;  // Always report 1 device
    return Status::Success;
}

Status CudaWrapper::getDeviceProperties(cudaDeviceProp* prop, int device)
{
    (void)device;
    // Initialize mock device properties if not already done
    static bool initialized = false;
    if (!initialized)
    {
        initMockDeviceProps();
        initialized = true;
    }
    *prop = g_mockDeviceProps;
    return Status::Success;
}

Status CudaWrapper::synchronizeDevice()
{
    return Status::Success;
}

Status CudaWrapper::createStream(cudaStream_t* stream, unsigned int flags)
{
    (void)flags;
    // Create a fake stream pointer
    *stream = reinterpret_cast<cudaStream_t>(new int(g_mockStreams.size()));
    g_mockStreams.push_back(*stream);
    return Status::Success;
}

Status CudaWrapper::destroyStream(cudaStream_t stream)
{
    // Find and remove the stream
    for (auto it = g_mockStreams.begin(); it != g_mockStreams.end(); ++it)
    {
        if (*it == stream)
        {
            g_mockStreams.erase(it);
            delete reinterpret_cast<int*>(stream);
            break;
        }
    }
    return Status::Success;
}

Status CudaWrapper::synchronizeStream(cudaStream_t stream)
{
    (void)stream;
    return Status::Success;
}

Status CudaWrapper::malloc(void** devPtr, size_t size)
{
    // Allocate host memory as a stand-in for device memory
    *devPtr = ::malloc(size);
    if (*devPtr)
    {
        g_allocatedMemory[*devPtr] = size;
        return Status::Success;
    }
    return Status::OutOfMemory;
}

Status CudaWrapper::free(void* devPtr)
{
    if (g_allocatedMemory.find(devPtr) != g_allocatedMemory.end())
    {
        ::free(devPtr);
        g_allocatedMemory.erase(devPtr);
        return Status::Success;
    }
    return Status::InvalidValue;
}

Status CudaWrapper::memcpy(void* destination, const void* source, size_t count, int kind)
{
    (void)kind;
    if (count == 0)
    {
        return Status::Success;
    }

    // For simplicity, just do a regular memcpy regardless of kind
    ::memcpy(destination, source, count);
    return Status::Success;
}

Status CudaWrapper::memcpyAsync(void* dst, const void* src, size_t count, int kind, cudaStream_t stream)
{
    (void)stream;
    return memcpy(dst, src, count, kind);
}

Status CudaWrapper::createEvent(cudaEvent_t* event)
{
    // Create a fake event pointer
    *event = reinterpret_cast<cudaEvent_t>(new int(g_mockEvents.size()));
    g_mockEvents.push_back(*event);
    return Status::Success;
}

Status CudaWrapper::destroyEvent(cudaEvent_t event)
{
    // Find and remove the event
    for (auto it = g_mockEvents.begin(); it != g_mockEvents.end(); ++it)
    {
        if (*it == event)
        {
            g_mockEvents.erase(it);
            delete reinterpret_cast<int*>(event);
            break;
        }
    }
    return Status::Success;
}

Status CudaWrapper::recordEvent(cudaEvent_t event, cudaStream_t stream)
{
    (void)event;
    (void)stream;
    return Status::Success;
}

Status CudaWrapper::queryEvent(cudaEvent_t event)
{
    (void)event;
    return Status::Success;
}

Status CudaWrapper::synchronizeEvent(cudaEvent_t event)
{
    (void)event;
    return Status::Success;
}

}  // namespace sep::cuda

// Note: We're not implementing the memory management functions here
// to avoid conflicts with the real implementation in the memory manager.
// The real implementations will be used from libsep_memory.a