/**
 * @file cuda_wrapper.cu
 * @brief Production-ready CUDA wrapper implementation for the SEP context engine
 *
 * This file provides a robust C-style interface to CUDA functions, with specific
 * support for the SEP context engine's three-tier memory system (STM, MTM, LTM).
 * It handles memory management, stream operations, and pattern processing operations.
 */

// Include CUDA compatibility header first

#include "compat/cuda_common.h"
#include "compat/cuda_unified_fix.h"

// Include memory tiers first to prevent redefinitions
#include "memory/memory_tier.h"

// Host-specific standard headers
#if !defined(__CUDACC__)
#include <cstdio>
#include <cstring>
#include <map>
#include <mutex>
#include <string>
#include <vector>
#endif

// CUDA compatibility headers

// SEP core headers
#include "compat/cuda_wrapper.h"

#include "memory/memory_tier_manager.hpp"
#include "compat/cuda_helpers.h"
#include "compat/sep_glm_wrapper.h"

// Forward declarations
namespace cuda {
class CudaWrapper;
}  // namespace cuda

// Disable exception specification checks handled above

// Define implementation flag
#define CUDA_IMPLEMENTATION

// Include CUDA headers after compatibility setup
#include <cuda.h>
// Temporarily disable problematic headers
// #include <cuda_bf16.h>
// #include <cuda_fp16.h>
#include <device_launch_parameters.h>

#if !defined(__CUDACC__) || defined(SEP_CUDA_AVAILABLE)

// Coherence thresholds for memory tier transitions
// Coherence thresholds defined in memory_tier_manager.hpp

// -----------------------------------------------------------------------------
// CudaWrapper implementation
// -----------------------------------------------------------------------------

/**
 * @brief Thread-safe CUDA wrapper singleton
 * @note Individual CUDA operations are thread-safe when using separate streams
 * @warning Shared resources require external synchronization
 */
sep::cuda::CudaWrapper& sep::cuda::CudaWrapper::getInstance() {
    static CudaWrapper instance;
    return instance;
}

// Helper function for error checking is provided by error_check.h

sep::cuda::Status sep::cuda::CudaWrapper::setDevice(int device) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaSetDevice(device);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)device;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::getDeviceCount(int* count) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaGetDeviceCount(count);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)count;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::getDeviceProperties(cudaDeviceProp* prop, int device) {
#if defined(SEP_CUDA_AVAILABLE)
// Define a compatibility macro for different CUDA versions
#if defined(CUDA_VERSION) && CUDA_VERSION >= 12000
    // CUDA 12.0 and newer use cudaGetDeviceProperties_v2
    cudaError_t result = cudaGetDeviceProperties_v2(prop, device);
#else
    // Older CUDA versions use cudaGetDeviceProperties
    cudaError_t result = cudaGetDeviceProperties(prop, device);
#endif
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)prop;
    (void)device;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::synchronizeDevice() {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaDeviceSynchronize();
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::createStream(cudaStream_t* stream, unsigned int flags) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaStreamCreateWithFlags(stream, flags);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)stream;
    (void)flags;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::destroyStream(cudaStream_t stream) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaStreamDestroy(stream);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)stream;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::synchronizeStream(cudaStream_t stream) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaStreamSynchronize(stream);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)stream;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::malloc(void** devPtr, size_t size) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaMalloc(devPtr, size);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)devPtr;
    (void)size;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::free(void* devPtr) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaFree(devPtr);
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)devPtr;
    return Status::Error;
#endif
}

sep::cuda::Status sep::cuda::CudaWrapper::memcpy(void* dst, const void* src, size_t count, int kind) {
#if defined(SEP_CUDA_AVAILABLE)
    cudaError_t result = cudaMemcpy(dst, src, count, static_cast<cudaMemcpyKind>(kind));
    return result == cudaSuccess ? Status::Success : Status::Error;
#else
    (void)dst;
    (void)src;
    (void)count;
    (void)kind;
    return Status::Error;
#endif
}

#define CUDA_WRAPPER_IMPL(func_call, fallback)                              \
    do {                                                                    \
        if constexpr (cuda_available) {                                     \
            cudaError_t result = func_call;                                 \
            return result == cudaSuccess ? Status::Success : Status::Error; \
        } else {                                                            \
            fallback;                                                       \
            return Status::Error;                                           \
        }                                                                   \
    } while (0)

sep::cuda::Status sep::cuda::CudaWrapper::memcpyAsync(void* dst, const void* src, size_t count, int kind,
                                                      cudaStream_t stream) {
    CUDA_WRAPPER_IMPL(cudaMemcpyAsync(dst, src, count, static_cast<cudaMemcpyKind>(kind), stream), (void)dst; (void)src;
                      (void)count; (void)kind; (void)stream);
}

sep::cuda::Status sep::cuda::CudaWrapper::createEvent(cudaEvent_t* event) {
    CUDA_WRAPPER_IMPL(cudaEventCreate(event), *event = nullptr);
}

sep::cuda::Status sep::cuda::CudaWrapper::destroyEvent(cudaEvent_t event) {
    CUDA_WRAPPER_IMPL(cudaEventDestroy(event), (void)event);
}

sep::cuda::Status sep::cuda::CudaWrapper::recordEvent(cudaEvent_t event, cudaStream_t stream) {
    CUDA_WRAPPER_IMPL(cudaEventRecord(event, stream), (void)event; (void)stream);
}

sep::cuda::Status sep::cuda::CudaWrapper::queryEvent(cudaEvent_t event) {
    CUDA_WRAPPER_IMPL(cudaEventQuery(event), (void)event);
}

sep::cuda::Status sep::cuda::CudaWrapper::synchronizeEvent(cudaEvent_t event) {
    CUDA_WRAPPER_IMPL(cudaEventSynchronize(event), (void)event);
}

// -----------------------------------------------------------------------------
// C wrapper functions implementation
// -----------------------------------------------------------------------------

extern "C" {

sep::cuda::Status checkCudaErrorWrapper(cudaError_t error, const char* operation) {
#if defined(SEP_CUDA_AVAILABLE) && !defined(__CUDACC__)
    // Only use exceptions in host code, not in device code
    try {
        sep::cuda::checkCudaErrors(error, operation);
        return sep::cuda::Status::Success;
    } catch (const sep::CudaException& e) {
        fprintf(stderr, "Wrapper Error %s: %s\n", operation, e.what());
        return sep::cuda::Status::Error;
    } catch (...) {
        fprintf(stderr, "Unknown error in wrapper %s\n", operation);
        return sep::cuda::Status::Error;
    }
#else
    // No exceptions in device code or without CUDA runtime
    if (error != cudaSuccess) {
        fprintf(stderr, "CUDA Error %s: %d\n", operation, error);
        return sep::cuda::Status::Error;
    }
    return sep::cuda::Status::Success;
#endif
}

sep::cuda::Status synchronizeDeviceWrapper() {
    return sep::cuda::CudaWrapper::getInstance().synchronizeDevice();
}

sep::cuda::Status createStreamWrapper(cudaStream_t* stream) {
#if defined(SEP_CUDA_AVAILABLE)
    return sep::cuda::CudaWrapper::getInstance().createStream(stream);
#else
    *stream = reinterpret_cast<cudaStream_t>(1);
    return Status::Success;
#endif
}

sep::cuda::Status destroyStreamWrapper(cudaStream_t stream) {
    return sep::cuda::CudaWrapper::getInstance().destroyStream(stream);
}

sep::cuda::Status synchronizeStreamWrapper(cudaStream_t stream) {
    return sep::cuda::CudaWrapper::getInstance().synchronizeStream(stream);
}

sep::cuda::Status allocateMemoryWrapper(void** devPtr, size_t size) {
    return sep::cuda::CudaWrapper::getInstance().malloc(devPtr, size);
}

sep::cuda::Status freeMemoryWrapper(void* devPtr) {
    return sep::cuda::CudaWrapper::getInstance().free(devPtr);
}

sep::cuda::Status copyToDeviceWrapper(void* dst, const void* src, size_t count) {
    return sep::cuda::CudaWrapper::getInstance().memcpy(dst, src, count, cudaMemcpyHostToDevice);
}

sep::cuda::Status copyToHostWrapper(void* dst, const void* src, size_t count) {
    return sep::cuda::CudaWrapper::getInstance().memcpy(dst, src, count, cudaMemcpyDeviceToHost);
}

sep::cuda::Status copyOnDeviceWrapper(void* dst, const void* src, size_t count) {
    return sep::cuda::CudaWrapper::getInstance().memcpy(dst, src, count, cudaMemcpyDeviceToDevice);
}

sep::cuda::Status copyToDeviceAsyncWrapper(void* dst, const void* src, size_t count, cudaStream_t stream) {
    return sep::cuda::CudaWrapper::getInstance().memcpyAsync(dst, src, count, cudaMemcpyHostToDevice, stream);
}

sep::cuda::Status copyToHostAsyncWrapper(void* dst, const void* src, size_t count, cudaStream_t stream) {
    return sep::cuda::CudaWrapper::getInstance().memcpyAsync(dst, src, count, cudaMemcpyDeviceToHost, stream);
}

sep::cuda::Status copyOnDeviceAsyncWrapper(void* dst, const void* src, size_t count, cudaStream_t stream) {
    return sep::cuda::CudaWrapper::getInstance().memcpyAsync(dst, src, count, cudaMemcpyDeviceToDevice, stream);
}

}  // extern "C"

#endif  // !defined(__CUDACC__) || defined(SEP_CUDA_AVAILABLE)
