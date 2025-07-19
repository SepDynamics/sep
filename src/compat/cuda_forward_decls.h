// src/compat/cuda_forward_decls.h
#ifndef SEP_CUDA_FORWARD_DECLS_H
#define SEP_CUDA_FORWARD_DECLS_H

// Forward declarations for CUDA types
// This file should only contain forward declarations to avoid circular dependencies.

#if SEP_USE_CUDA && defined(__CUDACC__)
  // Real CUDA forward declarations
  struct cudaStream;
  typedef struct cudaStream* cudaStream_t;

  struct cudaEvent;
  typedef struct cudaEvent* cudaEvent_t;

  // Add other necessary forward declarations here
#else
  // Stub forward declarations for non-CUDA builds
  typedef void* cudaStream_t;
  typedef void* cudaEvent_t;
#endif

#endif // SEP_CUDA_FORWARD_DECLS_H