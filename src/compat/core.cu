#include "compat/core.h"
#include "compat/cuda_runtime.h"
#include "compat/types.h"
#include <stdexcept>

namespace sep::cuda {

sep::api::ErrorCode CudaCore::initialize(int device_id) {
    if (initialized_) {
        return ErrorCode::Success;
    }

    cudaError_t err = cudaSetDevice(device_id);
    if (err != cudaSuccess) {
        return ErrorCode::InvalidArgument;
    }

    // Initialize CUDA context
    err = cudaFree(0);
    if (err != cudaSuccess) {
        return ErrorCode::InvalidState;
    }

    current_device_ = device_id;
    initialized_ = true;

    // Query device properties
    return queryDeviceProperties();
}

sep::api::ErrorCode CudaCore::queryDeviceProperties() {
    int count;
    cudaError_t err = cudaGetDeviceCount(&count);
    if (err != cudaSuccess) {
        return ErrorCode::ResourceNotFound;
    }

    device_properties_.resize(count);
    for (int i = 0; i < count; i++) {
        err = cudaGetDeviceProperties(&device_properties_[i], i);
        if (err != cudaSuccess) {
            return ErrorCode::InvalidArgument;
        }
    }

    return Error::SUCCESS;
}

sep::api::ErrorCode CudaCore::initializeDevice(int device) {
    cudaError_t err = cudaSetDevice(device);
    if (err != cudaSuccess) {
        return CudaResult::ERROR_INVALID_DEVICE_POINTER;
    }

    current_device_ = device;
    return ErrorCode::Success;
}

} // namespace sep::cuda
