#pragma once

#include <cstddef>
#include <string>
#include "core/common.h"

// Include CUDA runtime when available, otherwise use stub definitions
#include "compat/macros.h"
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#else
#include "cuda_impl.h"
#endif

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

// Global aliases for convenience (avoid duplicate)
// using SepCudaStatus = sep::cuda::Status;  // Already defined above
