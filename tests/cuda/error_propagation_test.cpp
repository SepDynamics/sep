#include "cuda/core.h"
#include <gtest/gtest.h>

using namespace sep;

TEST(CUDACoreError, InvalidDeviceSet)
{
    auto& core = cuda::CudaCore::instance();
    auto  err  = core.setDevice(-1);
    EXPECT_EQ(err.status, Status::Error);
    EXPECT_EQ(err.code, ErrorCode::InvalidArgument);
}

TEST(CUDACoreError, InvalidGetDeviceProperties)
{
    auto&          core = cuda::CudaCore::instance();
    cudaDeviceProp props{};
    auto           err = core.getDeviceProperties(props, -1);
    EXPECT_EQ(err.status, Status::Error);
    EXPECT_EQ(err.code, ErrorCode::InvalidArgument);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
