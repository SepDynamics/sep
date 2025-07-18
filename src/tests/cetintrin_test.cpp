#include <gtest/gtest.h>

#include <immintrin.h>

TEST(CetIntrinTest, Incsspd) {
    unsigned int t = 0; // Initialize t to 0
    // This is a test case to ensure the intrinsic can be called without warnings.
    // The actual behavior of the intrinsic is not tested here.
    _incsspd(t);
    SUCCEED();
}