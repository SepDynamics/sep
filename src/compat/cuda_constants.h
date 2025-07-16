#ifndef SEP_COMPAT_CUDA_CONSTANTS_H
#define SEP_COMPAT_CUDA_CONSTANTS_H

#include "cuda_base.h"

// This header provides all CUDA constants needed by the application
// without directly including CUDA headers that could cause circular dependencies

namespace sep {
namespace cuda {

// Error code constants as an enum to avoid conflicts with actual CUDA definitions
enum class CudaError : cudaError_t {
    Success = 0,
    MemoryAllocation = 2,
    InitializationError = 3,
    InvalidDevice = 10,
    InvalidValue = 11,
    DeviceUninitialized = 37,
    NotReady = 34,
    SetOnActiveProcess = 711,
    StreamCaptureUnsupported = 900,
    InvalidMemcpyDirection = 21,
    InvalidResourceHandle = 400,
    OutOfMemory = 2,
    InvalidDevicePointer = 17,
    InvalidConfiguration = 9,
    InvalidPitchValue = 12,
    InvalidSymbol = 13,
    DeviceAlreadyInUse = 54,
    DeviceNotLicensed = 803,
    NoDevice = 100
};

// For backward compatibility with existing code
// These constants will be phased out in favor of the enum
constexpr cudaError_t cudaSuccess = static_cast<cudaError_t>(CudaError::Success);
constexpr cudaError_t cudaErrorMemoryAllocation = static_cast<cudaError_t>(CudaError::MemoryAllocation);
constexpr cudaError_t cudaErrorInitializationError = static_cast<cudaError_t>(CudaError::InitializationError);
constexpr cudaError_t cudaErrorInvalidDevice = static_cast<cudaError_t>(CudaError::InvalidDevice);
constexpr cudaError_t cudaErrorInvalidValue = static_cast<cudaError_t>(CudaError::InvalidValue);
constexpr cudaError_t cudaErrorDeviceUninitialized = static_cast<cudaError_t>(CudaError::DeviceUninitialized);
constexpr cudaError_t cudaErrorNotReady = static_cast<cudaError_t>(CudaError::NotReady);
constexpr cudaError_t cudaErrorSetOnActiveProcess = static_cast<cudaError_t>(CudaError::SetOnActiveProcess);
constexpr cudaError_t cudaErrorStreamCaptureUnsupported = static_cast<cudaError_t>(CudaError::StreamCaptureUnsupported);
constexpr cudaError_t cudaErrorInvalidMemcpyDirection = static_cast<cudaError_t>(CudaError::InvalidMemcpyDirection);
constexpr cudaError_t cudaErrorInvalidResourceHandle = static_cast<cudaError_t>(CudaError::InvalidResourceHandle);
constexpr cudaError_t cudaErrorOutOfMemory = static_cast<cudaError_t>(CudaError::OutOfMemory);
constexpr cudaError_t cudaErrorInvalidDevicePointer = static_cast<cudaError_t>(CudaError::InvalidDevicePointer);
constexpr cudaError_t cudaErrorInvalidConfiguration = static_cast<cudaError_t>(CudaError::InvalidConfiguration);
constexpr cudaError_t cudaErrorInvalidPitchValue = static_cast<cudaError_t>(CudaError::InvalidPitchValue);
constexpr cudaError_t cudaErrorInvalidSymbol = static_cast<cudaError_t>(CudaError::InvalidSymbol);
constexpr cudaError_t cudaErrorDeviceAlreadyInUse = static_cast<cudaError_t>(CudaError::DeviceAlreadyInUse);
constexpr cudaError_t cudaErrorDeviceNotLicensed = static_cast<cudaError_t>(CudaError::DeviceNotLicensed);
constexpr cudaError_t cudaErrorNoDevice = static_cast<cudaError_t>(CudaError::NoDevice);

// Stream and memory flags
enum class CudaFlags : unsigned int {
    // Stream flags
    StreamDefault = 0x00,
    StreamNonBlocking = 0x01,
    
    // Memory attachment flags
    MemAttachGlobal = 0x01,
    MemAttachHost = 0x02,
    MemAttachSingle = 0x04,
    
    // Host allocation flags
    HostAllocDefault = 0x00,
    HostAllocPortable = 0x01,
    HostAllocMapped = 0x02,
    HostAllocWriteCombined = 0x04,
    
    // Event flags
    EventDefault = 0x00,
    EventBlockingSync = 0x01,
    EventDisableTiming = 0x02,
    EventInterprocess = 0x04
};

// Exported SEP-prefixed constants for backward compatibility
constexpr unsigned int SEP_cudaStreamDefault = static_cast<unsigned int>(CudaFlags::StreamDefault);
constexpr unsigned int SEP_cudaStreamNonBlocking = static_cast<unsigned int>(CudaFlags::StreamNonBlocking);
constexpr unsigned int SEP_cudaMemAttachGlobal = static_cast<unsigned int>(CudaFlags::MemAttachGlobal);
constexpr unsigned int SEP_cudaMemAttachHost = static_cast<unsigned int>(CudaFlags::MemAttachHost);
constexpr unsigned int SEP_cudaMemAttachSingle = static_cast<unsigned int>(CudaFlags::MemAttachSingle);
constexpr unsigned int SEP_cudaHostAllocDefault = static_cast<unsigned int>(CudaFlags::HostAllocDefault);
constexpr unsigned int SEP_cudaHostAllocPortable = static_cast<unsigned int>(CudaFlags::HostAllocPortable);
constexpr unsigned int SEP_cudaHostAllocMapped = static_cast<unsigned int>(CudaFlags::HostAllocMapped);
constexpr unsigned int SEP_cudaHostAllocWriteCombined = static_cast<unsigned int>(CudaFlags::HostAllocWriteCombined);
constexpr unsigned int SEP_cudaEventDefault = static_cast<unsigned int>(CudaFlags::EventDefault);
constexpr unsigned int SEP_cudaEventBlockingSync = static_cast<unsigned int>(CudaFlags::EventBlockingSync);
constexpr unsigned int SEP_cudaEventDisableTiming = static_cast<unsigned int>(CudaFlags::EventDisableTiming);
constexpr unsigned int SEP_cudaEventInterprocess = static_cast<unsigned int>(CudaFlags::EventInterprocess);

// Getter functions for error constants (preferred way to access in new code)
inline cudaError_t GetCudaSuccess() { return static_cast<cudaError_t>(CudaError::Success); }
inline cudaError_t GetCudaErrorNotReady() { return static_cast<cudaError_t>(CudaError::NotReady); }
inline cudaError_t GetCudaErrorInvalidValue() { return static_cast<cudaError_t>(CudaError::InvalidValue); }
inline cudaError_t GetCudaErrorInvalidDevice() { return static_cast<cudaError_t>(CudaError::InvalidDevice); }
inline cudaError_t GetCudaErrorInvalidMemcpyDirection() { return static_cast<cudaError_t>(CudaError::InvalidMemcpyDirection); }
inline cudaError_t GetCudaErrorInitializationError() { return static_cast<cudaError_t>(CudaError::InitializationError); }
inline cudaError_t GetCudaErrorNoDevice() { return static_cast<cudaError_t>(CudaError::NoDevice); }
inline cudaError_t GetCudaErrorOutOfMemory() { return static_cast<cudaError_t>(CudaError::OutOfMemory); }

// Stream flags as functions
inline unsigned int GetCudaStreamDefault() { return static_cast<unsigned int>(CudaFlags::StreamDefault); }
inline unsigned int GetCudaStreamNonBlocking() { return static_cast<unsigned int>(CudaFlags::StreamNonBlocking); }

// Memory flags as functions
inline unsigned int GetCudaMemAttachGlobal() { return static_cast<unsigned int>(CudaFlags::MemAttachGlobal); }
inline unsigned int GetCudaMemAttachHost() { return static_cast<unsigned int>(CudaFlags::MemAttachHost); }
inline unsigned int GetCudaMemAttachSingle() { return static_cast<unsigned int>(CudaFlags::MemAttachSingle); }

// Host allocation flags as functions
inline unsigned int GetCudaHostAllocDefault() { return static_cast<unsigned int>(CudaFlags::HostAllocDefault); }
inline unsigned int GetCudaHostAllocPortable() { return static_cast<unsigned int>(CudaFlags::HostAllocPortable); }
inline unsigned int GetCudaHostAllocMapped() { return static_cast<unsigned int>(CudaFlags::HostAllocMapped); }
inline unsigned int GetCudaHostAllocWriteCombined() { return static_cast<unsigned int>(CudaFlags::HostAllocWriteCombined); }

// Event flags as functions
inline unsigned int GetCudaEventDefault() { return static_cast<unsigned int>(CudaFlags::EventDefault); }
inline unsigned int GetCudaEventBlockingSync() { return static_cast<unsigned int>(CudaFlags::EventBlockingSync); }
inline unsigned int GetCudaEventDisableTiming() { return static_cast<unsigned int>(CudaFlags::EventDisableTiming); }
inline unsigned int GetCudaEventInterprocess() { return static_cast<unsigned int>(CudaFlags::EventInterprocess); }

// MemcpyKind values as functions - these use the enum from cuda_base.h
inline cudaMemcpyKind GetMemcpyHostToHost() { return cudaMemcpyHostToHost; }
inline cudaMemcpyKind GetMemcpyHostToDevice() { return cudaMemcpyHostToDevice; }
inline cudaMemcpyKind GetMemcpyDeviceToHost() { return cudaMemcpyDeviceToHost; }
inline cudaMemcpyKind GetMemcpyDeviceToDevice() { return cudaMemcpyDeviceToDevice; }
inline cudaMemcpyKind GetMemcpyDefault() { return cudaMemcpyDefault; }

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_CONSTANTS_H