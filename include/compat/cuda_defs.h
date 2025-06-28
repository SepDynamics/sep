#pragma once

#include <cstddef>
#include <string>
#include "core/common.h"
#include "compat/macros.h"

// CUDA types and constants must be in global scope to match CUDA runtime
#if !SEP_CUDA_AVAILABLE

// Basic CUDA types
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;

#endif

// SEP CUDA namespace for additional functionality
namespace sep {
namespace cuda {

// Status codes for CUDA operations
enum class Status { Success = 0, Error = 1, InvalidValue = 2, OutOfMemory = 3, NotInitialized = 4, DeviceNotFound = 5 };

// Error wrapper class
class Error {
public:
    Error() : status(Status::Success), code(::sep::SEPResult::SUCCESS) {}
    explicit Error(Status s) : status(s), code(::sep::SEPResult::SUCCESS) {}
    Error(Status s, const std::string& msg, const std::string& loc, ::sep::SEPResult c)
        : status(s), message(msg), location(loc), code(c) {}
    
    Status status;
    std::string message;
    std::string location;
    ::sep::SEPResult code;
};

// Legacy type aliases for backward compatibility
using SepCudaStatus = sep::cuda::Status;
#define SEP_CUDA_SUCCESS (sep::cuda::Status::Success)

}  // namespace cuda
}  // namespace sep

#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#include "compat/cuda_helpers.h"
#endif
