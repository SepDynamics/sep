// src/compat/cuda_constants.h
#ifndef SEP_CUDA_CONSTANTS_H
#define SEP_CUDA_CONSTANTS_H

#if SEP_USE_CUDA && defined(__CUDACC__)
  // Real CUDA constants
  #include <cuda_runtime.h>
#else
  // Define stub constants
  #define cudaSuccess 0
  // Add other necessary constants here
#endif

#endif // SEP_CUDA_CONSTANTS_H