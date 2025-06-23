#include <gtest/gtest.h>
#include "compat/raii.h"

extern "C" int mockCudaGetAllocationCount();
extern "C" void mockCudaReset();

using namespace sep::cuda;

TEST(CudaRAIITest, DeviceBufferFreesOnScopeExit) {
    mockCudaReset();
    EXPECT_EQ(mockCudaGetAllocationCount(), 0);
    {
        DeviceBufferRAII<uint32_t> buf(8);
        EXPECT_TRUE(buf.valid());
        EXPECT_GT(mockCudaGetAllocationCount(), 0);
    }
    EXPECT_EQ(mockCudaGetAllocationCount(), 0);
}

TEST(CudaRAIITest, StreamMoveTransfersOwnership) {
    StreamRAII stream1;
    EXPECT_TRUE(stream1.valid());
    cudaStream_t original = stream1.get();
    StreamRAII stream2(std::move(stream1));
    EXPECT_TRUE(stream2.valid());
    EXPECT_EQ(stream2.get(), original);
    EXPECT_FALSE(stream1.valid());
}

TEST(CudaRAIITest, EventMoveTransfersOwnership) {
    EventRAII ev1;
    EXPECT_TRUE(ev1.valid());
    cudaEvent_t original = ev1.get();
    EventRAII ev2(std::move(ev1));
    EXPECT_TRUE(ev2.valid());
    EXPECT_EQ(ev2.get(), original);
    EXPECT_FALSE(ev1.valid());
}
