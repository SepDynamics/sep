#include <gtest/gtest.h>
#include <cerrno>
#include <cmath>
#include "compat/cuda_unified_fix.h"

TEST(LongDoubleMath, AcoslAccuracy) {
    long double values[] = {-1.0L, -0.5L, 0.0L, 0.5L, 1.0L};
    for (long double v : values) {
        errno = 0;
        long double res = acosl(v);
        long double ref = ::acosl(v);
        EXPECT_NEAR(static_cast<double>(res), static_cast<double>(ref), 1e-9);
        EXPECT_EQ(errno, 0);
    }
}

TEST(LongDoubleMath, AcoslDomain) {
    errno = 0;
    long double res = acosl(1.5L);
    EXPECT_TRUE(std::isnan(res));
    EXPECT_EQ(errno, EDOM);
}

TEST(LongDoubleMath, Atan2lAccuracy) {
    long double res = atan2l(0.75L, -0.25L);
    long double ref = ::atan2l(0.75L, -0.25L);
    EXPECT_NEAR(static_cast<double>(res), static_cast<double>(ref), 1e-9);
}
