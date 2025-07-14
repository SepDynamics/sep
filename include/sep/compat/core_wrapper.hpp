#pragma once

#include "compat/cuda_defs.h"
#include "core/error_handler.h"

namespace sep::cuda {

class CudaCore {
public:
    // Initialize CUDA with the specified device
    static ::sep::Error initialize(int device = 0);
};

} // namespace sep::cuda