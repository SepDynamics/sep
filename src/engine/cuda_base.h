#ifndef SEP_COMPAT_CUDA_BASE_H
#define SEP_COMPAT_CUDA_BASE_H

#include <stddef.h>  // For size_t

#ifndef SEP_ENGINE_HAS_CUDA
#define SEP_ENGINE_HAS_CUDA 0
#endif

namespace sep {
namespace cuda {

#if SEP_ENGINE_HAS_CUDA
#include <cuda_runtime_api.h>
#else
// Stub types when CUDA is not available
using cudaError_t = int;
using cudaStream_t = void*;
using cudaEvent_t = void*;

enum cudaMemcpyKind {
    cudaMemcpyHostToHost = 0,
    cudaMemcpyHostToDevice = 1,
    cudaMemcpyDeviceToHost = 2,
    cudaMemcpyDeviceToDevice = 3,
    cudaMemcpyDefault = 4
};
#endif

// Device properties structure
struct cudaDeviceProp {
    char name[256];
    int major;
    int minor;
    size_t totalGlobalMem;
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_COMPAT_CUDA_BASE_H