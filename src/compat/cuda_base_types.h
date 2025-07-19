// src/compat/cuda_base_types.h
#ifndef SEP_CUDA_BASE_TYPES_H
#define SEP_CUDA_BASE_TYPES_H

#if SEP_USE_CUDA && defined(__CUDACC__)
  // Real CUDA types available
#else
  // Define stub types
  typedef int cudaError_t;
  typedef void* cudaStream_t;
  typedef void* cudaEvent_t;
  // ... other basic types
#endif

#endif