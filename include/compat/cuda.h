#pragma once

#include "compat/macros.h"

// Forward declarations and includes
#if SEP_CUDA_AVAILABLE
#include "cuda_runtime.h"
#else
// Include the compatibility header that defines stub types
#include "compat/cuda_runtime.h"
#include "compat/types.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#endif  // SEP_CUDA_AVAILABLE
