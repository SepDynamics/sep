#ifndef SEP_COMPAT_CUDA_WRAPPER_H
#define SEP_COMPAT_CUDA_WRAPPER_H

/**
 * @file cuda_wrapper.h
 * @brief DEPRECATED: Use cuda_unified.h instead
 *
 * This file is kept for backward compatibility but all
 * functions are now provided by cuda_unified.h.
 */

// Include the unified header that provides all required definitions
#include "compat/cuda_unified.h"

// No additional definitions needed - all SEP_cuda* functions are now defined
// in cuda_runtime.h which is included by cuda_unified.h

#endif // SEP_COMPAT_CUDA_WRAPPER_H
