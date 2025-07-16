#ifndef SEP_COMPAT_CUDA_TYPES_H
#define SEP_COMPAT_CUDA_TYPES_H

#include "compat/cuda_config.h"

// Include the actual CUDA runtime when available
#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime.h>
#endif

namespace sep {
namespace cuda {

#if SEP_ENGINE_HAS_CUDA
// When CUDA is available, import the actual CUDA types
using ::cudaError_t;
using ::cudaStream_t;
using ::cudaEvent_t;
using ::cudaMemcpyKind;

#else
// When CUDA is not available, define compatible stub types
typedef int cudaError_t;
typedef void* cudaStream_t;
typedef void* cudaEvent_t;

// Stub enum to match CUDA's memory copy directions
enum cudaMemcpyKind
{
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};
#endif

} // namespace cuda
} // namespace sep

#endif // SEP_COMPAT_CUDA_TYPES_H