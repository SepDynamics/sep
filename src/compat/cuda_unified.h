#ifndef SEP_COMPAT_CUDA_UNIFIED_H
#define SEP_COMPAT_CUDA_UNIFIED_H

/**
 * @file cuda_unified.h
 * @brief Unified CUDA compatibility layer
 *
 * This is the main header that should be included by any component
 * that needs CUDA functionality. It provides a consistent API
 * that works whether CUDA is available or not.
 */

// 1. Base types first (no dependencies)
#include "compat/cuda_base_types.h"

// 2. Forward declarations
#include "compat/cuda_forward_decls.h"

// 3. Constants
#include "compat/cuda_constants.h"

// 4. Function declarations
#include "compat/cuda_functions.h"

// 5. Implementations last
#if SEP_USE_CUDA
  #include "compat/cuda_runtime_impl.h"
  #include "compat/cuda_math_impl.h"
#endif

#endif // SEP_COMPAT_CUDA_UNIFIED_H