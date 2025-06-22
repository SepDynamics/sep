#include "compat/cuda_unified_fix.h"

#include "cuda/cuda_common.h"


#if !defined(__CUDACC__)
#    include <string>
#endif

#if !defined(__CUDACC__)
namespace sep::cuda {

// Simple error handling
enum class Status
{
    Success = 0,
    Error   = 1
};

class Error
{
public:
    Error() : status(Status::Success), message("") {}
    Error(Status s) : status(s), message("") {}
    Error(Status s, const std::string& msg) : status(s), message(msg) {}

    Status      status;
    std::string message;
};

KernelTrace::KernelTrace(const char* name, cudaStream_t stream) noexcept
    : span_(name), stream_(stream), start_(nullptr), stop_(nullptr)
{
    if (cudaEventCreate(&start_) != cudaSuccess || cudaEventCreate(&stop_) != cudaSuccess
        || cudaEventRecord(start_, stream_) != cudaSuccess)
    {
        // Handle initialization failure
        if (start_)
            cudaEventDestroy(start_);
        if (stop_)
            cudaEventDestroy(stop_);
        start_ = stop_ = nullptr;
    }
}

KernelTrace::~KernelTrace() noexcept
{
    if (!start_ || !stop_)
        return;

    try
    {
        float ms = 0.0f;
        if (cudaEventRecord(stop_, stream_) == cudaSuccess && cudaEventSynchronize(stop_) == cudaSuccess
            && cudaEventElapsedTime(&ms, start_, stop_) == cudaSuccess)
        {
            span_.setAttribute("kernel_time_ms", static_cast<std::int64_t>(ms));
        }
    }
    catch (...)
    {
        // Suppress exceptions in destructor
    }

    // Cleanup events
    if (start_)
        cudaEventDestroy(start_);
    if (stop_)
        cudaEventDestroy(stop_);
}

// CUDA kernel for calculating grid dimensions
SEP_DEVICE dim3 calculateGrid(int n, int blockSize)
{
    int numBlocks = (n + blockSize - 1) / blockSize;
    return dim3(numBlocks);
}

// CUDA kernel for calculating block dimensions
SEP_DEVICE dim3 calculateBlock(int blockSize)
{
    return dim3(blockSize);
}

// Helper function to check CUDA errors
Error checkCudaError(cudaError_t cuda_error, const char* operation)
{
    try
    {
        if (cuda_error != cudaSuccess)
        {
            throw sep::CudaException(__FILE__, __LINE__, cuda_error);
        }
    }
    catch (const sep::CudaException& e)
    {
        return Error(Status::Error, std::string(operation) + ": " + e.what());
    }
    return Error(Status::Success);
}

// Helper function to synchronize device
Error synchronizeDevice() noexcept
{
    try
    {
        cudaError_t sync_error = cudaDeviceSynchronize();
        if (sync_error != cudaSuccess)
        {
            return Error(Status::Error,
                         std::string("Device synchronization failed: ") + cudaGetErrorString(sync_error));
        }
    }
    catch (...)
    {
        return Error(Status::Error, "Unexpected error during device synchronization");
    }
    return Error(Status::Success);
}

}  // namespace sep::cuda
#endif
