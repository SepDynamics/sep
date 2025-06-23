#include "compat/compat.h"
#include "compat/constants.h"

#include <gtest/gtest.h>
#include <type_traits>

// Ensure CUDA_KERNEL macro is available
#ifndef CUDA_KERNEL
#error "CUDA_KERNEL macro must be defined"
#endif

namespace sep::cuda {

// Test suite for CUDA compatibility header
TEST(CudaCompatTest, KernelConfigConstants) {
    EXPECT_EQ(constants::WARP_SIZE, 32);
    EXPECT_EQ(constants::MAX_BLOCK_SIZE, 1024);
    EXPECT_EQ(constants::DEFAULT_BLOCK_SIZE, 256);
}

TEST(CudaCompatTest, TypeDefinitions) {
    // cudaSuccess should be zero
    EXPECT_EQ(static_cast<int>(cudaSuccess), 0);

    // cudaError_t should be an enum or integral type
    static_assert(std::is_enum<cudaError_t>::value ||
                    std::is_integral<cudaError_t>::value,
                "cudaError_t must be enum or integral type");

    // cudaStream_t and cudaEvent_t should be pointer types
    static_assert(std::is_pointer<cudaStream_t>::value,
                "cudaStream_t should be a pointer type");
    static_assert(std::is_pointer<cudaEvent_t>::value,
                "cudaEvent_t should be a pointer type");
}

TEST(CudaCompatTest, CheckCudaErrorBehavior) {
    // Success case: should return true
    EXPECT_TRUE(checkCudaError(cudaSuccess, "successOp"));

    // Failure case: use a non-zero error code
    cudaError_t fakeError = static_cast<cudaError_t>(1);
    EXPECT_FALSE(checkCudaError(fakeError, "failOp"));
}

} // namespace sep::cuda
