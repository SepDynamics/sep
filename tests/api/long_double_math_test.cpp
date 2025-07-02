#include "compat/cuda_unified_fix.h"
#include <gtest/gtest.h>
#include <cmath>
#include <cerrno>

TEST(LongDoubleMath, AcoslAccuracy) {
    long double values[] = {-1.0L, -0.5L, 0.0L, 0.5L, 1.0L};
    for (long double v : values) {
        errno = 0;
        long double expected = std::acosl(v);
        long double result = acosl(v);
        if (v < -1.0L || v > 1.0L) {
            EXPECT_TRUE(std::isnan(result));
            EXPECT_EQ(errno, EDOM);
        } else {
            EXPECT_NEAR(static_cast<double>(result), static_cast<double>(expected), 1e-12);
        }
    }
}

TEST(LongDoubleMath, Atan2lAccuracy) {
    long double vals[] = {-1.0L, -0.5L, 0.0L, 0.5L, 1.0L};
    for (long double y : vals) {
        for (long double x : vals) {
            long double expected = std::atan2l(y, x);
            long double result = atan2l(y, x);
            EXPECT_NEAR(static_cast<double>(result), static_cast<double>(expected), 1e-12);
        }
    }
}
