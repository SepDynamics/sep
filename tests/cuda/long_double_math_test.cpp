#include <gtest/gtest.h>
#include <cmath>
#include <cuda_runtime.h>
#include "compat/cuda_unified_fix.h"
#include "compat/macros.h"

SEP_GLOBAL void device_acosl(long double* out, long double x) {
    out[0] = acosl(x);
}
SEP_GLOBAL void device_atan2l(long double* out, long double y, long double x) {
    out[0] = atan2l(y, x);
}

TEST(LongDoubleMathHost, AcoslAccuracy) {
    long double v = 0.5L;
    long double expected = ::acosl(v);
    EXPECT_NEAR(static_cast<double>(acosl(v)), static_cast<double>(expected), 1e-9);
}

TEST(LongDoubleMathHost, Atan2lAccuracy) {
    long double y = 0.5L, x = -0.3L;
    long double expected = ::atan2l(y, x);
    EXPECT_NEAR(static_cast<double>(atan2l(y, x)), static_cast<double>(expected), 1e-9);
}

#if SEP_ENGINE_HAS_CUDA
TEST(LongDoubleMathDevice, AcoslAccuracy) {
    long double* d_out;
    ASSERT_EQ(cudaMalloc(reinterpret_cast<void**>(&d_out), sizeof(long double)), cudaSuccess);
    device_acosl<<<1,1>>>(d_out, 0.5L);
    ASSERT_EQ(cudaDeviceSynchronize(), cudaSuccess);
    long double h_val;
    ASSERT_EQ(cudaMemcpy(&h_val, d_out, sizeof(long double), sep::cuda::cudaMemcpyDeviceToHost), cudaSuccess);
    cudaFree(d_out);
    long double expected = ::acosl(0.5L);
    EXPECT_NEAR(static_cast<double>(h_val), static_cast<double>(expected), 1e-9);
}

TEST(LongDoubleMathDevice, Atan2lAccuracy) {
    long double* d_out;
    ASSERT_EQ(cudaMalloc(reinterpret_cast<void**>(&d_out), sizeof(long double)), cudaSuccess);
    device_atan2l<<<1,1>>>(d_out, 0.5L, -0.3L);
    ASSERT_EQ(cudaDeviceSynchronize(), cudaSuccess);
    long double h_val;
    ASSERT_EQ(cudaMemcpy(&h_val, d_out, sizeof(long double), sep::cuda::cudaMemcpyDeviceToHost), cudaSuccess);
    cudaFree(d_out);
    long double expected = ::atan2l(0.5L, -0.3L);
    EXPECT_NEAR(static_cast<double>(h_val), static_cast<double>(expected), 1e-9);
}
#endif

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
