#include <gtest/gtest.h>
#include "utils/cuda_helpers.h"

TEST(CudaHelpers, LogCudaErrorNoThrow) {
    // Should not throw or abort when cudaSuccess is passed
    sep::cuda::logCudaError("noop", cudaSuccess);
    SUCCEED();
}
