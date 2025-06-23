#include <gtest/gtest.h>

#include "compat/macros.h"
#if !SEP_CUDA_AVAILABLE
TEST(CudaAvailabilityTest, SkipWhenNoCudaRuntime) {
    GTEST_SKIP() << "CUDA runtime not available";
}
#else
TEST(CudaAvailabilityTest, DevicePresent) {
    int count = 0;
    cudaError_t err = cudaGetDeviceCount(&count);
    if (err != cudaSuccess || count == 0) {
        GTEST_SKIP() << "CUDA device not available";
    }
    EXPECT_GT(count, 0);
}
#endif
