#include <gtest/gtest.h>
#include "cuda/raii.h"

using namespace sep::cuda;

TEST(CudaRAIITest, StreamMoveSemantics) {
    StreamRAII s1;
    ASSERT_TRUE(s1.valid());
    cudaStream_t handle = s1.get();
    StreamRAII s2(std::move(s1));
    EXPECT_FALSE(s1.valid());
    EXPECT_EQ(s2.get(), handle);
}

TEST(CudaRAIITest, EventMoveSemantics) {
    EventRAII e1;
    ASSERT_TRUE(e1.valid());
    cudaEvent_t handle = e1.get();
    EventRAII e2(std::move(e1));
    EXPECT_FALSE(e1.valid());
    EXPECT_EQ(e2.get(), handle);
}

TEST(CudaRAIITest, DeviceBufferMoveSemantics) {
    DeviceBufferRAII<uint32_t> b1(4);
    ASSERT_TRUE(b1.valid());
    uint32_t* ptr = b1.get();
    DeviceBufferRAII<uint32_t> b2(std::move(b1));
    EXPECT_FALSE(b1.valid());
    EXPECT_EQ(b2.get(), ptr);
}
