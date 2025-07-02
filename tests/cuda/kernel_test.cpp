#include <gtest/gtest.h>
#include "compat/raii.h"
#include "compat/kernels.h"
#include "compat/shim.h"


using namespace sep;

TEST(CUDATest, QSHKernelBasic) {
    cuda::StreamRAII stream;
    ASSERT_TRUE(stream.valid());

    std::vector<uint64_t> chunks(32, 0xAAAAAAAAAAAAAAAAULL);
    std::vector<uint32_t> collapse_indices(chunks.size());
    std::vector<uint32_t> collapse_counts(chunks.size());

    cuda::DeviceBufferRAII<uint64_t> d_chunks(chunks.size());
    cuda::DeviceBufferRAII<uint32_t> d_idx(chunks.size());
    cuda::DeviceBufferRAII<uint32_t> d_counts(chunks.size());

    ASSERT_TRUE(d_chunks.valid());
    cudaMemcpy(d_chunks.get(), chunks.data(), chunks.size()*sizeof(uint64_t), cudaMemcpyHostToDevice);

    EXPECT_EQ(cuda::launchQSHKernel(d_chunks.get(), chunks.size(), d_idx.get(), d_counts.get(), stream.get()), cudaSuccess);
}
