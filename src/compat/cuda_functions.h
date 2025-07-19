// src/compat/cuda_functions.h
#ifndef SEP_CUDA_FUNCTIONS_H
#define SEP_CUDA_FUNCTIONS_H

#include "cuda_base_types.h" // For cudaError_t, cudaStream_t, etc.
#include "cuda_constants.h"  // For cudaSuccess
#include <cstddef>           // For size_t
#include <cstdint>           // For nullptr

#if SEP_USE_CUDA && defined(__CUDACC__)
  // Real CUDA function declarations
  #include <cuda_runtime.h>
#else
  // Define stub functions
  static inline cudaError_t cudaGetDeviceCount(int* count) { *count = 0; return cudaSuccess; }
  static inline cudaError_t cudaMalloc(void** devPtr, size_t size) { *devPtr = nullptr; return cudaSuccess; }
  static inline cudaError_t cudaFree(void* devPtr) { return cudaSuccess; }
  static inline cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, int kind) { return cudaSuccess; }
  static inline cudaError_t cudaDeviceSynchronize() { return cudaSuccess; }
  // Add other necessary function declarations here
#endif

#endif // SEP_CUDA_FUNCTIONS_H