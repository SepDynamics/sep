#include "compat/cuda_common.h"

#include "compat/cuda_unified_fix.h"

#include "compat/cuda_helpers.h"

// GLM isolation layer

#include "compat/core.h"
#include "compat/event.h"
#include "compat/macros.h"
// Ensure CUDA_CHECK is defined before use
#include "compat/sep_glm_wrapper.h"

namespace sep::cuda {

Event::Event(EventFlags flags) {
    unsigned int cuda_flags = 0;
    if ((static_cast<unsigned int>(flags) & static_cast<unsigned int>(EventFlags::Default)))
        cuda_flags |= cudaEventDefault;
    if ((static_cast<unsigned int>(flags) & static_cast<unsigned int>(EventFlags::BlockingSync)))
        cuda_flags |= cudaEventBlockingSync;
    if ((static_cast<unsigned int>(flags) & static_cast<unsigned int>(EventFlags::DisableTiming)))
        cuda_flags |= cudaEventDisableTiming;
    if ((static_cast<unsigned int>(flags) & static_cast<unsigned int>(EventFlags::InterProcess)))
        cuda_flags |= cudaEventInterprocess;

    // Remove try-catch for CUDA compatibility
    cudaError_t result = cudaEventCreateWithFlags(&handle_, cuda_flags);
    if (result != cudaSuccess) {
        handle_ = nullptr;
    }
}

Event::~Event() {
    if (handle_) {
        // Remove try-catch for CUDA compatibility 
        cudaError_t result = cudaEventDestroy(handle_);
        (void)result; // Avoid unused variable warning
        handle_ = nullptr;
    }
}

void Event::record(Stream& stream) {
    if (handle_) {
        // Remove try-catch for CUDA compatibility 
        cudaError_t result = cudaEventRecord(handle_, reinterpret_cast<cudaStream_t>(stream.handle())); 
        (void)result; // Avoid unused variable warning
    }
}

void Event::synchronize() {
    if (handle_) {
        // Remove try-catch for CUDA compatibility 
        cudaError_t result = cudaEventSynchronize(handle_);
        (void)result; // Avoid unused variable warning
    }
}

float Event::elapsedTime(Event& start) {
    if (!handle_ || !start.handle_) {
        return 0.0f;
    }

    float elapsed = 0.0f;
    // Remove try-catch for CUDA compatibility 
    cudaError_t result = cudaEventElapsedTime(&elapsed, start.handle_, handle_);
    if (result != cudaSuccess) {
        return 0.0f;
    }

    return elapsed;
}

}  // namespace sep::cuda
