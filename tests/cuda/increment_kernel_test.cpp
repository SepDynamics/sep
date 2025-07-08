#include <gtest/gtest.h>
#include "compat/raii.h"
#include "increment_kernel.hpp"

TEST(TestbedCudaKernel, IncrementKernel) {
    const unsigned int n = 32;
    sep::cuda::DeviceBufferRAII<int> d_data(n);
    ASSERT_TRUE(d_data.valid());

    std::vector<int> host(n, 1);
    EXPECT_EQ(cudaMemcpy(d_data.get(), host.data(), n * sizeof(int), sep::cuda::cudaMemcpyHostToDevice), cudaSuccess);

    EXPECT_EQ(launch_increment_kernel(d_data.get(), n, 0), cudaSuccess);
    EXPECT_EQ(cudaDeviceSynchronize(), cudaSuccess);

    host.assign(n, 0);
    EXPECT_EQ(cudaMemcpy(host.data(), d_data.get(), n * sizeof(int), sep::cuda::cudaMemcpyDeviceToHost), cudaSuccess);

    for (int v : host) {
        EXPECT_EQ(v, 2); // original 1 + 1
    }
}
