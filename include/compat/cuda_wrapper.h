/**
 * @file cuda_wrapper.h
 * @brief Singleton CUDA wrapper for the SEP context engine
 *
 * This header exposes a single class, CudaWrapper, that should be used by the
 * rest of the application when interacting with the CUDA runtime.  It provides
 * thin wrappers around the CUDA API and centralises device and stream
 * management.  A small C interface is kept for existing code but now forwards to
 * the singleton instance.
 */

#pragma once

#include <stddef.h>
#include <cstdio>
#include <cstdlib>
#include "compat/cuda_common.h"



namespace sep {
namespace cuda {

class CudaWrapper {
 public:
  static CudaWrapper& getInstance();

  Status setDevice(int device);
  Status getDeviceCount(int* count);

#ifdef __CUDACC__
  Status getDeviceProperties(cudaDeviceProp* prop, int device);
#else
  Status getDeviceProperties(struct cudaDeviceProp* prop, int device);
#endif

  Status synchronizeDevice();

  Status createStream(cudaStream_t* stream,
                     unsigned int flags = 0);
  Status destroyStream(cudaStream_t stream);
  Status synchronizeStream(cudaStream_t stream);

  Status malloc(void** devPtr, size_t size);
  Status free(void* devPtr);

  Status memcpy(void* dst, const void* src, size_t count,
                int kind);
  Status memcpyAsync(void* dst, const void* src, size_t count, int kind,
                     cudaStream_t stream);

  // Event helpers used by RAII utilities
  Status createEvent(cudaEvent_t* event);
  Status destroyEvent(cudaEvent_t event);
  Status recordEvent(cudaEvent_t event, cudaStream_t stream);
  Status queryEvent(cudaEvent_t event);
  Status synchronizeEvent(cudaEvent_t event);

 private:
  CudaWrapper() = default;
  ~CudaWrapper() = default;
  CudaWrapper(const CudaWrapper&) = delete;
  CudaWrapper& operator=(const CudaWrapper&) = delete;
};

} // namespace cuda
} // namespace sep

// -----------------------------------------------------------------------------
// Helper API
// -----------------------------------------------------------------------------
// C compatible API retained for legacy callers
extern "C" {
sep::cuda::Status checkCudaErrorWrapper(cudaError_t error, const char* operation);
sep::cuda::Status synchronizeDeviceWrapper();
sep::cuda::Status createStreamWrapper(cudaStream_t* stream);
sep::cuda::Status destroyStreamWrapper(cudaStream_t stream);
sep::cuda::Status synchronizeStreamWrapper(cudaStream_t stream);
sep::cuda::Status allocateMemoryWrapper(void** devPtr, size_t size);
sep::cuda::Status freeMemoryWrapper(void* devPtr);
sep::cuda::Status copyToDeviceWrapper(void* dst, const void* src, size_t count);
sep::cuda::Status copyToHostWrapper(void* dst, const void* src, size_t count);
sep::cuda::Status copyOnDeviceWrapper(void* dst, const void* src, size_t count);
sep::cuda::Status copyToDeviceAsyncWrapper(void* dst, const void* src, size_t count,
                                         cudaStream_t stream);
sep::cuda::Status copyToHostAsyncWrapper(void* dst, const void* src, size_t count,
                                       cudaStream_t stream);
sep::cuda::Status copyOnDeviceAsyncWrapper(void* dst, const void* src, size_t count,
                                         cudaStream_t stream);
}

