#include <gtest/gtest.h>

#include "compat/constants.h"
#include "compat/memory.h"

using namespace sep::cuda;

// Test suite for CUDA utility functions
TEST(CudaUtilsTest, ValidateKernelDimensionsValid) {
    // Minimal valid dimensions
    EXPECT_TRUE(validateKernelDimensions(1, 1, 0));
    // Maximal valid dimensions based on constants
    EXPECT_TRUE(validateKernelDimensions(
        constants::MAX_BLOCK_SIZE,
        constants::DEFAULT_BLOCK_SIZE,
        0));
}

TEST(CudaUtilsTest, ValidateKernelDimensionsInvalidGrid) {
    // Grid size exceeds maximum
    uint32_t badGrid = constants::MAX_BLOCK_SIZE + 1;
    EXPECT_FALSE(validateKernelDimensions(badGrid, 1, 0));
}

TEST(CudaUtilsTest, ValidateKernelDimensionsInvalidBlock) {
    // Block size exceeds maximum
    uint32_t badBlock = constants::MAX_BLOCK_SIZE + 1;
    EXPECT_FALSE(validateKernelDimensions(1, badBlock, 0));
}

TEST(CudaUtilsTest, CheckDeviceMemoryZero) {
    // Requesting zero bytes should always succeed
    EXPECT_TRUE(checkDeviceMemory(0));
}

TEST(CudaUtilsTest, CheckDeviceMemoryTooLarge) {
    // Requesting a very large number of bytes should fail
    size_t huge = static_cast<size_t>(-1);
    EXPECT_FALSE(checkDeviceMemory(huge));
}

TEST(CudaUtilsTest, CheckMemoryZero) {
    EXPECT_TRUE(checkMemory(0));
}

TEST(CudaUtilsTest, CheckMemoryTooLarge) {
    size_t huge = static_cast<size_t>(-1);
    EXPECT_FALSE(checkMemory(huge));
}

// Note: Removed logCudaError stub since it's defined in the actual utils.cu file
