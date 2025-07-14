#include <stdexcept>

#include "compat/core.h"
#include "compat/cuda_runtime.h"  // Must come first to define CUDA types
#include "compat/types.h"
#include "core/common.h"
#include "core/error_handler.h"

namespace sep::cuda {

    Error CudaCore::initialize(int device_id)
    {
        auto& instance = CudaCore::instance();
        if (instance.initialized_)
        {
            return Error(SEPResult::SUCCESS, "Already initialized", "CudaCore::initialize");
        }

        cudaError_t err = cudaSetDevice(device_id);
        if (err != cudaSuccess)
        {
            return Error(SEPResult::INVALID_ARGUMENT, "Failed to set device",
                         "CudaCore::initialize");
        }

        // Initialize CUDA context
        err = cudaFree(0);
        if (err != cudaSuccess)
        {
            return Error(SEPResult::NOT_INITIALIZED, "Failed to initialize CUDA context",
                         "CudaCore::initialize");
        }

        instance.current_device_ = device_id;
        instance.initialized_ = true;
        // Query device properties
        auto props_result = instance.queryDeviceProperties();
        if (!props_result.isSuccess())
        {
            return Error(SEPResult::UNKNOWN_ERROR, "Failed to query device properties",
                         "CudaCore::initialize");
        }
        return Error(SEPResult::SUCCESS, "Initialization successful", "CudaCore::initialize");
    }

    // Convert CUDA Status to SEP Result
    SEPResult toSEPResult(Status status)
    {
        switch (status)
        {
            case Status::Success:
                return SEPResult::SUCCESS;
            case Status::InvalidValue:
                return SEPResult::INVALID_ARGUMENT;
            case Status::NotInitialized:
                return SEPResult::NOT_INITIALIZED;
            case Status::DeviceNotFound:
                return SEPResult::FILE_NOT_FOUND;
            default:
                return SEPResult::UNKNOWN_ERROR;
        }
    }

    // Convert CUDA Error to SEP Error
    Error toSepError(const Error& cuda_error, const char* location)
    {
        static const std::map<Status, SEPResult> status_map = {
            {Status::Success, SEPResult::SUCCESS},
            {Status::InvalidValue, SEPResult::INVALID_ARGUMENT},
            {Status::NotInitialized, SEPResult::NOT_INITIALIZED},
            {Status::DeviceNotFound, SEPResult::DEVICE_NOT_FOUND}};
        return Error(status_map.at(cuda_error.status), cuda_error.message, location);
    }

    Error CudaCore::initialize(int device_id)
    {
        auto& instance = CudaCore::instance();
        if (instance.initialized_)
        {
            return Error(Status::Success);
        }

        cudaError_t err = cudaSetDevice(device_id);
        if (err != cudaSuccess)
        {
            return Error(Status::InvalidValue);
        }

        // Initialize CUDA context
        err = cudaFree(0);
        if (err != cudaSuccess)
        {
            return Error(Status::NotInitialized);
        }

        instance.current_device_ = device_id;
        instance.initialized_ = true;

        // Query device properties
        return instance.queryDeviceProperties();
    }

    Error CudaCore::queryDeviceProperties()
    {
        int count;
        cudaError_t err = cudaGetDeviceCount(&count);
        if (err != cudaSuccess)
        {
            return Error(Status::DeviceNotFound);
        }

        device_properties_.resize(count);
        for (int i = 0; i < count; i++)
        {
            err = cudaGetDeviceProperties(&device_properties_[i], i);
            if (err != cudaSuccess)
            {
                return Error(Status::InvalidValue);
            }
        }

        return Error(Status::Success);
    }

    Error CudaCore::initializeDevice(int device)
    {
        cudaError_t err = cudaSetDevice(device);
        if (err != cudaSuccess)
        {
            return Error(Status::InvalidValue);
        }

        current_device_ = device;
        return Error(Status::Success);
    }

}  // namespace sep::cuda
