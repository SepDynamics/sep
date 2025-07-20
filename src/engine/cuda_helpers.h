#pragma once

#include <cstdio>

#ifdef __CUDACC__
#include <cuda_runtime.h>
#else
// Forward declarations when not compiling with CUDA
typedef int cudaError_t;
const int cudaSuccess = 0;
#endif

// Comprehensive CUDA helper utilities - consolidated from multiple files
namespace sep {
namespace cuda {

#ifndef CUDA_CHECK
#define CUDA_CHECK(call)                                                  \
  do {                                                                    \
    cudaError_t error = (call);                                           \
    if (error != cudaSuccess) {                                           \
      (void)std::fprintf(stderr, "CUDA error in %s: %s\n", #call,          \
                         cuda::SEP_cudaGetErrorString(error));                      \
    }                                                                     \
  } while (0)
#endif

}  // namespace cuda
}  // namespace sep
