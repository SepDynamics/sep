#include "hip_runtime_wrapper.h"
#include "compat/cuda/cuda_wrapper_unified.h"

#include <string>

// Forward declarations for HIP functions
extern "C" {
    // Stub implementation for HIP functions - not needed for NVIDIA GPUs
    hipError_t hipGetDevicePropertiesR0600(sep::cuda::cudaDeviceProp* prop, int device)
    {
        // Return success but don't actually do anything
        // This is a stub since we're using NVIDIA GPUs, not AMD
        (void)prop;  // Suppress unused parameter warning
        (void)device;  // Suppress unused parameter warning
        return hipSuccess;  // Defined as cudaSuccess in hip_runtime_wrapper.h
    }
}

// Stub implementation for HIP functions that are not needed for NVIDIA GPUs
extern "C" {
    hipError_t hipHostMalloc(void** ptr, size_t size, unsigned int flags)
    {
        // Just use regular malloc as a fallback
        // This is a stub since we're using NVIDIA GPUs, not AMD
        (void)flags;  // Suppress unused parameter warning
        *ptr = malloc(size);
        return *ptr ? hipSuccess : static_cast<hipError_t>(1); // Return success if malloc succeeded
    }
}

// Implementation of CUDA functions in the sep::cuda namespace
namespace sep
{
    namespace cuda
    {

        // Stream functions
        cudaError_t cudaStreamCreateWithFlags(void** stream, unsigned int flags)
        {
            return cuda_wrapper::StreamCreateWithFlags(stream, flags);
        }

        cudaError_t cudaStreamDestroy(void* stream)
        {
            return cuda_wrapper::StreamDestroy(stream);
        }

        cudaError_t cudaStreamSynchronize(void* stream)
        {
            return cuda_wrapper::StreamSynchronize(stream);
        }

        cudaError_t cudaStreamWaitEvent(void* stream, void* event, unsigned int flags)
        {
            return cuda_wrapper::StreamWaitEvent(stream, event, flags);
        }

        cudaError_t cudaStreamAttachMemAsync(void* stream, void* devPtr, size_t length,
                                            unsigned int flags)
        {
            // HIP doesn't have a direct equivalent, but we can use hipStreamAttachMemAsync if
            // available or return success if not critical
            (void)stream;
            (void)devPtr;
            (void)length;
            (void)flags;
            return cuda_wrapper::Success;
        }

        // Event functions
        cudaError_t cudaEventCreate(void** event)
        {
            return cuda_wrapper::EventCreate(event);
        }

        cudaError_t cudaEventDestroy(void* event)
        {
            return cuda_wrapper::EventDestroy(event);
        }

        cudaError_t cudaEventSynchronize(void* event)
        {
            return cuda_wrapper::EventSynchronize(event);
        }

        cudaError_t cudaEventRecord(void* event, void* stream)
        {
            return cuda_wrapper::EventRecord(event, stream);
        }

        // Memory management functions
        cudaError_t cudaMalloc(void** devPtr, size_t size)
        {
            return cuda_wrapper::Malloc(devPtr, size);
        }

        cudaError_t cudaFree(void* devPtr)
        {
            return cuda_wrapper::Free(devPtr);
        }

        cudaError_t cudaMallocManaged(void** devPtr, size_t size, unsigned int flags)
        {
            // Use CUDA's managed memory directly since we're on NVIDIA hardware
            // This replaces the HIP-based implementation
            return cuda_wrapper::MallocManaged(devPtr, size, flags);
        }

        // Error handling
        const char* cudaGetErrorString(int error)
        {
            return cuda_wrapper::GetErrorString(error);
        }

    }  // namespace cuda
}  // namespace sep
