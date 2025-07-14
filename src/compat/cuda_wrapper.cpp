#include <cuda.h>
#include <cuda_runtime.h>

#include "compat/core.h"
#include "compat/cuda_defs.h"
#include "compat/shim.h"
#include "compat/types.h"
#include "core/common.h"
#include "core/error_handler.h"

using sep::cuda::Error;
using sep::shim::string;

namespace sep::cuda {

    Error CudaCore::initialize(int device)
    {
        // Basic CUDA device initialization
        cudaError_t cuda_err = cudaSetDevice(device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(cudaGetErrorString(cuda_err)),
                          string("CudaCore::initialize"), SEPResult::CUDA_ERROR);
        }

        // Get device properties to verify compatibility
        cudaDeviceProp props;
        cuda_err = cudaGetDeviceProperties(&props, device);
        if (cuda_err != cudaSuccess)
        {
            return Error(Status::Error, string(cudaGetErrorString(cuda_err)),
                          string("CudaCore::initialize"), SEPResult::CUDA_ERROR);
        }

        return Error(Status::Success, string("CUDA initialized successfully"),
                      string("CudaCore::initialize"), SEPResult::SUCCESS);
    }

} // namespace sep::cuda