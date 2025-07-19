#ifndef SEP_COMPAT_CUDA_TYPES_H
#define SEP_COMPAT_CUDA_TYPES_H

#include "compat/cuda_base.h"

namespace sep {
namespace cuda {

// Error code definitions (consistent regardless of CUDA availability)
enum ErrorCode
{
    Success = 0,
    InvalidValue = 1,
    OutOfMemory = 2,
    NotInitialized = 3,
    Deinitialized = 4,
    DeviceNotLicensed = 10,
    MemoryAllocation = 2, // Corresponds to cudaErrorMemoryAllocation
    InvalidDevice = 101
};

}  // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_TYPES_H