#ifndef SEP_COMPAT_CUDA_CONSTANTS_H
#define SEP_COMPAT_CUDA_CONSTANTS_H

#include "compat/cuda_types.h"

namespace sep {
namespace cuda {

#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, import the actual CUDA constants
// Note: We don't place these in the global namespace to avoid conflicts
const cudaError_t cudaSuccess = ::cudaSuccess;
const cudaError_t cudaErrorNotReady = ::cudaErrorNotReady;
const cudaError_t cudaErrorInvalidValue = ::cudaErrorInvalidValue;
const cudaError_t cudaErrorInvalidDevice = ::cudaErrorInvalidDevice;
const cudaError_t cudaErrorInvalidMemcpyDirection = ::cudaErrorInvalidMemcpyDirection;
const cudaError_t cudaErrorInitializationError = ::cudaErrorInitializationError;
const cudaError_t cudaErrorNoDevice = ::cudaErrorNoDevice;
const cudaError_t cudaErrorOutOfMemory = ::cudaErrorOutOfMemory;

// Stream flags
const unsigned int cudaStreamDefault = 0x00;
const unsigned int cudaStreamNonBlocking = 0x01;

// MemcpyKind values
const cudaMemcpyKind memcpyHostToHost = ::cudaMemcpyHostToHost;
const cudaMemcpyKind memcpyHostToDevice = ::cudaMemcpyHostToDevice;
const cudaMemcpyKind memcpyDeviceToHost = ::cudaMemcpyDeviceToHost;
const cudaMemcpyKind memcpyDeviceToDevice = ::cudaMemcpyDeviceToDevice;
const cudaMemcpyKind memcpyDefault = ::cudaMemcpyDefault;

#else
// When CUDA is not available, define stub constants
const cudaError_t cudaSuccess = 0;
const cudaError_t cudaErrorNotReady = 34;
const cudaError_t cudaErrorInvalidValue = 11;
const cudaError_t cudaErrorInvalidDevice = 10;
const cudaError_t cudaErrorInvalidMemcpyDirection = 21;
const cudaError_t cudaErrorInitializationError = 3;
const cudaError_t cudaErrorNoDevice = 100;
const cudaError_t cudaErrorOutOfMemory = 2;

// Stream flags
const unsigned int cudaStreamDefault = 0x00;
const unsigned int cudaStreamNonBlocking = 0x01;

// MemcpyKind values
const cudaMemcpyKind memcpyHostToHost = cudaMemcpyHostToHost;
const cudaMemcpyKind memcpyHostToDevice = cudaMemcpyHostToDevice;
const cudaMemcpyKind memcpyDeviceToHost = cudaMemcpyDeviceToHost;
const cudaMemcpyKind memcpyDeviceToDevice = cudaMemcpyDeviceToDevice;
const cudaMemcpyKind memcpyDefault = cudaMemcpyDefault;

#endif

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_CONSTANTS_H