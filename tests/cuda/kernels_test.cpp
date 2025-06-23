#include <gtest/gtest.h>
#include "compat/kernels.h"
#include "compat/kernels.cuh"
#include "compat/types.h"
#include "compat/constants.h"
#include "compat/raii.h"
#include "test_utils.hpp"
#include <vector>
#include <memory>

using namespace sep::cuda;

class CudaKernelsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize CUDA only if a device is available
        int count = 0;
        cudaError_t err = cudaGetDeviceCount(&count);
        if (err != cudaSuccess || count == 0) {
            GTEST_SKIP() << "CUDA device not available";
        }
        ASSERT_EQ(cudaSetDevice(0), cudaSuccess);
    }

    void TearDown() override {
        // Clean up any remaining CUDA resources
        cudaDeviceSynchronize();
        cudaDeviceReset();
    }

    // Helper to copy to device
    template<typename T>
    void copyToDevice(T* dest, const std::vector<T>& src) {
        EXPECT_EQ(cudaMemcpy(dest, src.data(), src.size() * sizeof(T),
                            cudaMemcpyHostToDevice), cudaSuccess);
    }

    // Helper to copy from device
    template<typename T>
    void copyFromDevice(std::vector<T>& dest, T* src) {
        EXPECT_EQ(cudaMemcpy(dest.data(), src, dest.size() * sizeof(T),
                            cudaMemcpyDeviceToHost), cudaSuccess);
    }
};

// QBSA Kernel Tests
TEST_F(CudaKernelsTest, QBSAKernelBasic) {
    const uint32_t num_probes = 128;

    // Prepare test data
    std::vector<uint32_t> probe_indices(num_probes);
    std::vector<uint32_t> expectations(num_probes);
    for (uint32_t i = 0; i < num_probes; i++) {
        probe_indices[i] = i;
        expectations[i] = i % 2;  // Alternating 0s and 1s
    }

    // Allocate device memory via RAII wrappers
    DeviceBufferRAII<uint32_t> d_probe_indices(num_probes);
    DeviceBufferRAII<uint32_t> d_expectations(num_probes);
    DeviceBufferRAII<uint32_t> d_bitfield(constants::BITFIELD_WORDS);
    DeviceBufferRAII<uint32_t> d_corrections(constants::MAX_BLOCK_SIZE);
    DeviceBufferRAII<uint32_t> d_correction_count(1);

    // Initialize device memory
    copyToDevice(d_probe_indices.get(), probe_indices);
    copyToDevice(d_expectations.get(), expectations);
    EXPECT_EQ(cudaMemset(d_bitfield.get(), 0,
                         constants::BITFIELD_WORDS * sizeof(uint32_t)),
              cudaSuccess);
    EXPECT_EQ(cudaMemset(d_correction_count.get(), 0, sizeof(uint32_t)),
              cudaSuccess);

    // Launch kernel
    EXPECT_EQ(launchQBSAKernel(d_probe_indices.get(), d_expectations.get(),
                              num_probes, d_bitfield.get(), d_corrections.get(),
                              d_correction_count.get(), 0),
              cudaSuccess);

    // Verify results
    uint32_t correction_count;
    EXPECT_EQ(cudaMemcpy(&correction_count, d_correction_count.get(),
                         sizeof(uint32_t), cudaMemcpyDeviceToHost), cudaSuccess);
    EXPECT_GT(correction_count, 0);

    // Cleanup
}

// QSH Kernel Tests
TEST_F(CudaKernelsTest, QSHKernelBasic) {
    const uint32_t num_chunks = 64;

    // Prepare test data
    std::vector<uint64_t> chunks(num_chunks);
    for (uint32_t i = 0; i < num_chunks; i++) {
        // Create symmetrical pairs in each chunk
        chunks[i] = (1ULL << i) | (1ULL << (i + 32));
    }

    // Allocate device memory via RAII wrappers
    DeviceBufferRAII<uint64_t> d_chunks(num_chunks);
    DeviceBufferRAII<uint32_t> d_collapse_indices(num_chunks * constants::SYMMETRY_PAIRS);
    DeviceBufferRAII<uint32_t> d_collapse_counts(num_chunks);

    // Initialize device memory
    copyToDevice(d_chunks.get(), chunks);

    // Launch kernel
    EXPECT_EQ(launchQSHKernel(d_chunks.get(), num_chunks, d_collapse_indices.get(),
                             d_collapse_counts.get(), 0), cudaSuccess);

    // Verify results
    std::vector<uint32_t> collapse_counts(num_chunks);
    copyFromDevice(collapse_counts, d_collapse_counts.get());

    for (uint32_t count : collapse_counts) {
        EXPECT_GT(count, 0);
    }

    // Cleanup handled by RAII
}

// Similarity Kernel Tests
TEST_F(CudaKernelsTest, SimilarityKernelBasic) {
    const uint32_t embedding_size = 128;

    // Prepare test data
    std::vector<float> emb_a(embedding_size, 1.0f);
    std::vector<float> emb_b(embedding_size, 1.0f);

    // Allocate device memory via RAII wrappers
    DeviceBufferRAII<float> d_similarity(1);
    DeviceBufferRAII<float> d_emb_a(embedding_size);
    DeviceBufferRAII<float> d_emb_b(embedding_size);

    // Initialize device memory
    copyToDevice(d_emb_a.get(), emb_a);
    copyToDevice(d_emb_b.get(), emb_b);

    // Launch kernel
    EXPECT_EQ(launchSimilarityKernel(d_similarity.get(), d_emb_a.get(),
                                    d_emb_b.get(), embedding_size, 0),
              cudaSuccess);

    // Verify results
    float similarity;
    EXPECT_EQ(cudaMemcpy(&similarity, d_similarity.get(), sizeof(float),
                         cudaMemcpyDeviceToHost), cudaSuccess);
    EXPECT_FLOAT_EQ(similarity, static_cast<float>(embedding_size));

    // Cleanup handled by RAII
}

// Blend Kernel Tests
TEST_F(CudaKernelsTest, BlendKernelBasic) {
    const uint32_t num_contexts = 4;
    const uint32_t embedding_size = 128;

    // Prepare test data
    std::vector<float> embeddings(num_contexts * embedding_size, 1.0f);
    std::vector<float> weights(num_contexts, 0.25f);  // Equal weights
    std::vector<float> output(embedding_size);

    // Allocate device memory via RAII wrappers
    DeviceBufferRAII<float> d_output(embedding_size);
    DeviceBufferRAII<float> d_embeddings(num_contexts * embedding_size);
    DeviceBufferRAII<float> d_weights(num_contexts);

    // Initialize device memory
    copyToDevice(d_embeddings.get(), embeddings);
    copyToDevice(d_weights.get(), weights);

    // Launch kernel
    EXPECT_EQ(launchBlendKernel(d_output.get(), d_embeddings.get(), d_weights.get(),
                               num_contexts, embedding_size, 0), cudaSuccess);

    // Verify results
    copyFromDevice(output, d_output.get());
    for (float val : output) {
        EXPECT_FLOAT_EQ(val, 1.0f);  // Sum of 0.25 * 1.0 for each context
    }

    // Cleanup handled by RAII
}

// Stream Handling Tests
TEST_F(CudaKernelsTest, KernelStreamHandling) {
    cudaStream_t stream;
    ASSERT_EQ(cudaStreamCreate(&stream), cudaSuccess);

    const uint32_t num_probes = 128;

    // Prepare test data
    std::vector<uint32_t> probe_indices(num_probes);
    std::vector<uint32_t> expectations(num_probes);
    for (uint32_t i = 0; i < num_probes; i++) {
        probe_indices[i] = i;
        expectations[i] = i % 2;
    }

    // Allocate device memory via RAII wrappers
    DeviceBufferRAII<uint32_t> d_probe_indices(num_probes);
    DeviceBufferRAII<uint32_t> d_expectations(num_probes);
    DeviceBufferRAII<uint32_t> d_bitfield(constants::BITFIELD_WORDS);
    DeviceBufferRAII<uint32_t> d_corrections(constants::MAX_BLOCK_SIZE);
    DeviceBufferRAII<uint32_t> d_correction_count(1);

    // Initialize device memory
    copyToDevice(d_probe_indices.get(), probe_indices);
    copyToDevice(d_expectations.get(), expectations);
    EXPECT_EQ(cudaMemsetAsync(d_bitfield.get(), 0,
                              constants::BITFIELD_WORDS * sizeof(uint32_t), stream),
              cudaSuccess);
    EXPECT_EQ(cudaMemsetAsync(d_correction_count.get(), 0, sizeof(uint32_t), stream),
              cudaSuccess);

    // Launch kernel in stream
    EXPECT_EQ(launchQBSAKernel(d_probe_indices.get(), d_expectations.get(), num_probes,
                              d_bitfield.get(), d_corrections.get(),
                              d_correction_count.get(), stream),
              cudaSuccess);

    // Synchronize stream
    EXPECT_EQ(cudaStreamSynchronize(stream), cudaSuccess);

    // Cleanup handled by RAII
    cudaStreamDestroy(stream);
}

// Error Handling Tests
TEST_F(CudaKernelsTest, KernelErrorHandling) {
    // Test invalid parameters
    EXPECT_NE(launchQBSAKernel(nullptr, nullptr, 0, nullptr, nullptr, nullptr, 0),
              cudaSuccess);
    EXPECT_NE(launchQSHKernel(nullptr, 0, nullptr, nullptr, 0),
              cudaSuccess);
    EXPECT_NE(launchSimilarityKernel(nullptr, nullptr, nullptr, 0, 0),
              cudaSuccess);
    EXPECT_NE(launchBlendKernel(nullptr, nullptr, nullptr, 0, 0, 0),
              cudaSuccess);
}

// Memory Management Tests
TEST_F(CudaKernelsTest, MemoryManagement) {
    const size_t large_size = 1024 * 1024;  // 1M elements

    // Test large allocations via RAII wrapper
    DeviceBufferRAII<uint32_t> large_array(large_size);
    EXPECT_TRUE(large_array.valid());

    // Test initialization
    EXPECT_EQ(cudaMemset(large_array.get(), 0, large_size * sizeof(uint32_t)),
              cudaSuccess);

    // Cleanup handled by RAII
}
