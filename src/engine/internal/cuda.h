#pragma once

#ifdef __CUDACC__
#include <cuda_runtime.h>
#include "engine/internal/cuda_helpers.h"
#include "engine/internal/cuda_sep.h"
#endif
#include "engine/internal/core.h"

namespace sep::cuda {

// CUDA utility functions and definitions
class CudaCore;

} // namespace sep::cuda
