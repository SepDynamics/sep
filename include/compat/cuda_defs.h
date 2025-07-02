#pragma once

// CUDA function attributes
#ifdef __CUDACC__
#define SEP_HOST __host__
#define SEP_DEVICE __device__
#define SEP_HD __host__ __device__
#else
#define SEP_HOST
#define SEP_DEVICE
#define SEP_HD
#endif

// Always include the compatibility runtime header
#include "compat/cuda_runtime.h"

#include <cstddef>
#include <string>
#include "core/common.h"
#include "compat/macros.h"


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
using SepCudaStatus = Status;
#define SEP_CUDA_SUCCESS (Status::Success)

}  // namespace cuda
}  // namespace sep

