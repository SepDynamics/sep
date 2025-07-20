#pragma once

#ifdef __CUDACC__
#include <cuda_runtime.h>
#include "engine/cuda_helpers.h"
#include "engine/cuda_sep.h"
#endif

namespace sep::cuda {

// CUDA utility functions and definitions
class CudaCore;

} // namespace sep::cuda
