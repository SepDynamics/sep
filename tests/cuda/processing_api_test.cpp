#include <gtest/gtest.h>
#include "compat/cuda_api.hpp"
#include "compat/constants.h"

using namespace sep;

class CudaApiProcessingTest : public ::testing::Test {
protected:
    void SetUp() override {
        int count = 0;
        if (sep::cuda::cudaGetDeviceCount(&count) != cudaSuccess || count == 0) {
            GTEST_SKIP() << "CUDA device not available";
        }
        EXPECT_EQ(sep_cuda_init(0), SEPResult::SUCCESS);
    }

    void TearDown() override { sep_cuda_cleanup(); }
};

TEST_F(CudaApiProcessingTest, ProcessBatchBasic) {
    const uint32_t num_probes = 4;
    uint32_t probe_indices[num_probes] = {0, 1, 32, 33};
    uint32_t expectations[num_probes]  = {1, 0, 1, 0};

    uint32_t bitfield[sep::cuda::constants::BITFIELD_WORDS] = {0};
    uint32_t corrections[sep::cuda::constants::MAX_BLOCK_SIZE] = {0};
    uint32_t correction_count = 0;

    ASSERT_EQ(sep_cuda_process_batch(probe_indices, expectations, num_probes,
                                     bitfield, corrections, &correction_count),
              SEPResult::SUCCESS);
    EXPECT_EQ(correction_count, 2u);
    EXPECT_EQ(bitfield[0], 1u);
    EXPECT_EQ(bitfield[1], 1u);
    EXPECT_EQ(corrections[0], 0u);
    EXPECT_EQ(corrections[1], 32u);
}

TEST_F(CudaApiProcessingTest, ProcessSymmetryBasic) {
    const uint32_t num_chunks = 2;
    uint64_t chunks[num_chunks] = {0ULL, 0xFFFFFFFFFFFFFFFFULL};

    constexpr uint32_t pairs = sep::cuda::constants::SYMMETRY_PAIRS;
    uint32_t indices[num_chunks * pairs] = {0};
    uint32_t counts[num_chunks] = {0};

    ASSERT_EQ(sep_cuda_process_symmetry(chunks, num_chunks, indices, counts),
              SEPResult::SUCCESS);

    for (uint32_t c = 0; c < num_chunks; ++c) {
        EXPECT_EQ(counts[c], pairs);
        for (uint32_t i = 0; i < pairs; ++i) {
            EXPECT_EQ(indices[c * pairs + i], i);
        }
    }
}

