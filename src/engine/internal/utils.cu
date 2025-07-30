#include "engine/internal/standard_includes.h"

// GLM isolation layer
#ifndef SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS
#define SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS 1
#endif
#include <glm/glm.hpp>

#include "constants.h"

namespace sep::cuda {
#if !defined(__CUDACC__)

bool checkDeviceMemory(std::size_t required_size) {
    std::size_t free_memory = 0, total_memory = 0;
    cudaError_t error = cudaMemGetInfo(&free_memory, &total_memory);
    if (error != cudaSuccess) {
        return false;
    }
    return free_memory >= required_size;
}

bool checkMemory(std::size_t required_size) {
#if defined(SEP_USE_CUDA)
    return checkDeviceMemory(required_size);
#else
#ifdef __linux__
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        std::size_t free_mem = static_cast<std::size_t>(info.freeram) * info.mem_unit;
        return free_mem >= required_size;
    }
    return required_size == 0;
#else
    void* ptr = std::malloc(required_size);
    if (!ptr)
        return false;
    std::free(ptr);
    return true;
#endif
#endif
}

bool validateKernelDimensions(std::size_t total_threads, std::size_t block_size, std::size_t shared_mem) {
    // Check block size
    if (block_size > cuda::constants::DEFAULT_BLOCK_SIZE) {
        return false;
    }

    // Check total threads
    if (total_threads > cuda::constants::MAX_BATCH_SIZE * cuda::constants::CHUNK_SIZE) {
        return false;
    }

    // Check shared memory
    if (shared_mem > cuda::constants::MAX_SHARED_MEMORY) {
        return false;
    }

    return true;
}
#endif

}  // namespace sep::cuda
