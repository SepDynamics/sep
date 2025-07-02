#include "blender/gpu_context.h"

#include "compat/macros.h"
#include <gtest/gtest.h>
#if SEP_CUDA_AVAILABLE
#include <cuda_runtime.h>
#include "compat/cuda_impl.h"
#endif

#include <cmath>
#include <memory>
#include <thread>
#include <vector>

#if !SEP_CUDA_AVAILABLE
TEST(GPUContextTest, SkipWithoutCuda) {
    GTEST_SKIP() << "CUDA not available, skipping GPU context tests.";
}
#else

class GPUContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        context_ = std::make_unique<sep::GPUContext>();
        ASSERT_EQ(sep::SEPResult::SUCCESS, context_->init());
    }

    void TearDown() override {
        context_.reset();
    }

    // Helper to allocate and fill test data
    std::vector<float> createTestData(size_t size) {
        std::vector<float> data(size);
        for (size_t i = 0; i < size; ++i) {
            data[i] = static_cast<float>(std::sin(static_cast<double>(i)));
        }
        return data;
    }

    std::unique_ptr<sep::GPUContext> context_;
};

TEST_F(GPUContextTest, InitializationTest) {
    sep::GPUContext ctx;
    EXPECT_EQ(sep::SEPResult::SUCCESS, ctx.init());
    EXPECT_FALSE(ctx.hasError());
}

TEST_F(GPUContextTest, DeviceSelectionTest) {
    int device_count;
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->getDeviceCount(device_count));
    if (device_count > 1) {
        EXPECT_EQ(sep::SEPResult::SUCCESS, context_->selectDevice(1));
        EXPECT_FALSE(context_->hasError());
    }
    EXPECT_EQ(sep::SEPResult::INVALID_DEVICE, context_->selectDevice(device_count + 1));
    EXPECT_TRUE(context_->hasError());
}

TEST_F(GPUContextTest, DeviceCapabilitiesTest) {
    // Skip this test since cudaDeviceProp is not fully available
    // The GPUContext should provide capability checking methods instead
    EXPECT_TRUE(context_->supportsComputeCapability(6, 1));  // Minimum compute capability 6.1
    EXPECT_TRUE(context_->supportsUnifiedMemory());
    EXPECT_TRUE(context_->supportsConcurrentKernels());
    EXPECT_GE(context_->getTotalMemory(), 4ULL * 1024 * 1024 * 1024);  // 4GB minimum
}

TEST_F(GPUContextTest, MemoryAllocationTest) {
    void* ptr = nullptr;
    const size_t size = 1024 * 1024;  // 1MB

    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&ptr, size));
    EXPECT_NE(nullptr, ptr);
    EXPECT_GE(context_->getUsedMemory(), size);

    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(ptr));
    EXPECT_EQ(0u, context_->getUsedMemory());
}

TEST_F(GPUContextTest, MemoryTransferTest) {
    const size_t size = 1024;
    std::vector<float> host_data = createTestData(size);
    std::vector<float> result_data(size);

    void* device_ptr = nullptr;
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&device_ptr, size * sizeof(float)));

    // Host to Device
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->copyHostToDevice(device_ptr, host_data.data(), size * sizeof(float)));

    // Device to Host
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->copyDeviceToHost(result_data.data(), device_ptr, size * sizeof(float)));

    // Verify data
    for (size_t i = 0; i < size; ++i) {
        EXPECT_FLOAT_EQ(host_data[i], result_data[i]);
    }

    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(device_ptr));
}

TEST_F(GPUContextTest, StreamOperationsTest) {
    cudaStream_t stream;
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->createStream(stream));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->synchronizeStream(stream));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyStream(stream));
}

TEST_F(GPUContextTest, EventOperationsTest) {
    cudaEvent_t event;
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->createEvent(event));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyEvent(event));
}

TEST_F(GPUContextTest, EventRecordAndSyncTest) {
    cudaEvent_t event;
    cudaStream_t stream;
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->createStream(stream));
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->createEvent(event));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->recordEvent(event, stream));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->synchronizeEvent(event));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyEvent(event));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyStream(stream));
}

TEST_F(GPUContextTest, DeviceInfoTest) {
    cudaDeviceProp prop{};
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->getDeviceInfo(prop));
    EXPECT_GT(prop.totalGlobalMem, 0u);
}

TEST_F(GPUContextTest, ErrorHandlingTest) {
    // Invalid memory allocation
    void* ptr = nullptr;
    const size_t huge_size = 1ULL << 40;  // 1TB
    EXPECT_EQ(sep::SEPResult::MEMORY_ERROR, context_->allocateMemory(&ptr, huge_size));
    EXPECT_TRUE(context_->hasError());
    EXPECT_FALSE(context_->getLastError().empty());

    context_->clearError();
    EXPECT_FALSE(context_->hasError());
}

TEST_F(GPUContextTest, ConcurrentOperationsTest) {
    const int num_streams = 4;
    const size_t size_per_stream = 1024;
    std::vector<cudaStream_t> streams(num_streams);
    std::vector<void*> device_ptrs(num_streams);
    std::vector<std::vector<float>> host_data(num_streams);
    std::vector<std::vector<float>> result_data(num_streams);

    // Initialize streams and allocate memory
    for (int i = 0; i < num_streams; ++i) {
        ASSERT_EQ(sep::SEPResult::SUCCESS, context_->createStream(streams[i]));
        ASSERT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&device_ptrs[i], size_per_stream * sizeof(float))); 
        host_data[i] = createTestData(size_per_stream);
        result_data[i].resize(size_per_stream);
    }

    // Perform concurrent operations
    for (int i = 0; i < num_streams; ++i) {
        EXPECT_EQ(sep::SEPResult::SUCCESS,
                  context_->copyHostToDevice(device_ptrs[i], host_data[i].data(), size_per_stream * sizeof(float)));
    }

    // Synchronize and verify
    for (int i = 0; i < num_streams; ++i) {
        EXPECT_EQ(sep::SEPResult::SUCCESS, context_->synchronizeStream(streams[i]));
        EXPECT_EQ(sep::SEPResult::SUCCESS,
                  context_->copyDeviceToHost(result_data[i].data(), device_ptrs[i], size_per_stream * sizeof(float)));

        for (size_t j = 0; j < size_per_stream; ++j) {
            EXPECT_FLOAT_EQ(host_data[i][j], result_data[i][j]);
        }
    }

    // Cleanup
    for (int i = 0; i < num_streams; ++i) {
        EXPECT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(device_ptrs[i]));
        EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyStream(streams[i]));
    }
}

TEST_F(GPUContextTest, MemoryTrackingTest) {
    const size_t num_allocs = 10;
    const size_t alloc_size = 1024 * 1024;  // 1MB
    std::vector<void*> ptrs(num_allocs);

    size_t total_allocated = 0;
    for (size_t i = 0; i < num_allocs; ++i) {
        ASSERT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&ptrs[i], alloc_size));
        total_allocated += alloc_size;
        EXPECT_EQ(total_allocated, context_->getUsedMemory());
    }

    for (size_t i = 0; i < num_allocs; ++i) {
        EXPECT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(ptrs[i]));
        total_allocated -= alloc_size;
        EXPECT_EQ(total_allocated, context_->getUsedMemory());
    }
}

TEST_F(GPUContextTest, DeviceQueryTest) {
    EXPECT_TRUE(context_->supportsComputeCapability(6, 1));
    EXPECT_TRUE(context_->supportsUnifiedMemory());

    size_t total = context_->getTotalMemory();
    size_t free = context_->getFreeMemory();
    size_t used = context_->getUsedMemory();

    EXPECT_GT(total, 0u);
    EXPECT_GT(free, 0u);
    EXPECT_LE(used, total);
    EXPECT_GE(total, free + used);
}

TEST_F(GPUContextTest, BufferOperationsTest) {
    const size_t size = 1024;
    std::vector<uint8_t> data(size, 42);

    auto buf = context_->createBuffer(size, data.data());
    ASSERT_TRUE(buf);

    void* mapped = context_->mapBuffer(buf.get());
    ASSERT_NE(nullptr, mapped);

    std::vector<uint8_t> host(size);
    std::memcpy(host.data(), mapped, size);
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(data[i], host[i]);
    }

    context_->unmapBuffer(buf.get());
}

#endif  // SEP_CUDA_AVAILABLE
