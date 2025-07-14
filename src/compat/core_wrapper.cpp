#include "sep/compat/core_wrapper.hpp"
#include <cuda.h>
#include <cuda_runtime.h>
#include "compat/cuda_defs.h"
#include "compat/types.h"
#include "core/error_handler.h"
#include "core/common.h"

namespace sep::cuda {

::sep::Error CudaCore::initialize(int device) {
    // Basic CUDA device initialization
    cudaError_t cuda_err = cudaSetDevice(device);
    if (cuda_err != cudaSuccess) {
        return ::sep::Error(
            SEPResult::CUDA_ERROR,
            cudaGetErrorString(cuda_err),
            "CudaCore::initialize"
        );
    }

    // Get device properties to verify compatibility
    cudaDeviceProp props;
    cuda_err = cudaGetDeviceProperties(&props, device);
    if (cuda_err != cudaSuccess) {
        return ::sep::Error(
            SEPResult::CUDA_ERROR,
            cudaGetErrorString(cuda_err),
            "CudaCore::initialize"
        );
    }

    return ::sep::Error(
        SEPResult::SUCCESS,
        "CUDA initialized successfully",
        "CudaCore::initialize"
    );
}

} // namespace sep::cuda