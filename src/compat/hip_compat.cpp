#include "hip_runtime_wrapper.h"
#include "cuda_wrapper.h"

#include <string>

// Forward declarations for HIP functions
extern "C" {
    hipError_t hipGetDevicePropertiesR0600(sep::cuda::cudaDeviceProp* prop, int device)
    {
        return hipGetDeviceProperties(prop, device);
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
            // HIP doesn't have direct managed memory equivalent
            // Use hipHostMalloc as a fallback with similar behavior
            (void)flags; // Suppress unused parameter warning
            return hipHostMalloc(devPtr, size, hipHostMallocMapped);
        }

        // Error handling
        const char* cudaGetErrorString(int error)
        {
            return cuda_wrapper::GetErrorString(error);
        }

    }  // namespace cuda
}  // namespace sep
