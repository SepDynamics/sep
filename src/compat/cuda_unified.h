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

// 1. Core definitions and macros that have no dependencies
#include "compat/macros.h"
#include "compat/cuda_config.h"
#include "compat/cuda_defs.h"

// 2. Base type definitions (forward declarations and stubs)
#include "compat/cuda_base.h"
#include "compat/cuda_types.h"
#include "compat/cuda_constants.h"

// 3. Math compatibility layer
#include "compat/math_common.h"

// 4. Function declarations
#include "compat/cuda_functions.h"

// 5. Inline implementations and runtime wrappers
#include "compat/cuda_runtime.h"

// 6. Higher-level utilities and wrappers
#include "compat/cuda_helpers.h"
#include "compat/raii.h"
#include "compat/stream.h"
#include "compat/event.h"

#endif // SEP_COMPAT_CUDA_UNIFIED_H