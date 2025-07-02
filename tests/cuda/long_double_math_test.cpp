#include "compat/cuda_unified_fix.h"
#include <gtest/gtest.h>
#include <cmath>
#include <cerrno>

TEST(LongDoubleMath, AcoslMatchesStd) {
    long double v = 0.5L;
    long double expected = ::acosl(v);
    long double result = acosl(v);
    EXPECT_NEAR(static_cast<double>(result), static_cast<double>(expected), 1e-12);
}

TEST(LongDoubleMath, Atan2lMatchesStd) {
    long double y = 0.7L;
    long double x = -0.4L;
    long double expected = ::atan2l(y, x);
    long double result = atan2l(y, x);
    EXPECT_NEAR(static_cast<double>(result), static_cast<double>(expected), 1e-12);
}

TEST(LongDoubleMath, AcoslDomainError) {
    errno = 0;
    long double r = acosl(1.5L);
    EXPECT_EQ(errno, EDOM);
    EXPECT_TRUE(std::isnan(static_cast<double>(r)));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
