#pragma once

#include <cstdio>

#include <cuda_runtime.h>

// Comprehensive CUDA helper utilities - consolidated from multiple files
namespace sep {
namespace cuda {

#ifndef CUDA_CHECK
#define CUDA_CHECK(call)                                                  \
  do {                                                                    \
    cudaError_t error = (call);                                           \
    if (error != cudaSuccess) {                                           \
      (void)std::fprintf(stderr, "CUDA error in %s: %s\n", #call,          \
                         cudaGetErrorString(error));                      \
    }                                                                     \
  } while (0)
#endif

}  // namespace cuda
}  // namespace sep
