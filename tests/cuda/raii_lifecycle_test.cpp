#include <gtest/gtest.h>
#include "cuda/raii.h"

using namespace sep::cuda;

TEST(CudaRAIITest, StreamMoveAndValidity) {
    StreamRAII a;
    EXPECT_TRUE(a.valid());
    StreamRAII b(std::move(a));
    EXPECT_TRUE(b.valid());
    EXPECT_FALSE(a.valid());
}

TEST(CudaRAIITest, EventMoveAndValidity) {
    EventRAII e1;
    EXPECT_TRUE(e1.valid());
    EventRAII e2(std::move(e1));
    EXPECT_TRUE(e2.valid());
    EXPECT_FALSE(e1.valid());
}
