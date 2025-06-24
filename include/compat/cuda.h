#pragma once

// Forward declarations and includes
#include "macros.h"

#if SEP_CUDA_AVAILABLE
// Use angle brackets to specifically target system CUDA headers
#include <cuda_runtime.h>
#else
// Include the compatibility header that defines stub types

#include "types.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#endif  // SEP_CUDA_AVAILABLE
