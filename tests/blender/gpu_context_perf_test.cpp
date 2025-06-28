#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

#include "blender/gpu_context.h"

class GPUContextPerfTest : public ::testing::Test {
 protected:
  void SetUp() override {
    context_ = std::make_unique<sep::GPUContext>();
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->init());
  }

  void TearDown() override { context_.reset(); }

  std::vector<float> createTestData(size_t size) {
    std::vector<float> data(size);
    for (size_t i = 0; i < size; ++i) {
      data[i] = static_cast<float>(std::sin(static_cast<double>(i)));
    }
    return data;
  }

  struct TimingStats {
    double min;
    double max;
    double avg;
    double stddev;

    TimingStats(const std::vector<double>& measurements) {
      if (measurements.empty()) {
        min = max = avg = stddev = 0.0;
        return;
      }

      min = *std::min_element(measurements.begin(), measurements.end());
      max = *std::max_element(measurements.begin(), measurements.end());
      avg = std::accumulate(measurements.begin(), measurements.end(), 0.0) / measurements.size();

      double variance = 0.0;
      for (double m : measurements) {
        variance += (m - avg) * (m - avg);
      }
      stddev = std::sqrt(variance / measurements.size());
    }

    void print(const std::string& name) const {
      std::cout << name << " timing statistics:\n"
                << "  Min: " << std::fixed << std::setprecision(3) << min << "ms\n"
                << "  Max: " << max << "ms\n"
                << "  Avg: " << avg << "ms\n"
                << "  StdDev: " << stddev << "ms\n";
    }
  };

  double measureOperation(std::function<void()> op) {
    auto start = std::chrono::high_resolution_clock::now();
    op();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
  }

  std::unique_ptr<sep::GPUContext> context_;
};

TEST_F(GPUContextPerfTest, MemoryAllocationPerformance) {
  const int iterations = 1000;
  const size_t alloc_size = 1024 * 1024;  // 1MB
  std::vector<void*> ptrs(iterations);
  std::vector<double> times;
  times.reserve(iterations);

  for (int i = 0; i < iterations; ++i) {
    double time = measureOperation(
        [&]() { ASSERT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&ptrs[i], alloc_size)); });
    times.push_back(time);
  }

  TimingStats stats(times);
  stats.print("Memory allocation");
  EXPECT_LT(stats.avg, 0.5) << "Average allocation time exceeds target";
  EXPECT_LT(stats.max, 2.0) << "Maximum allocation time exceeds target";

  // Cleanup
  for (auto ptr : ptrs) {
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(ptr));
  }
}

TEST_F(GPUContextPerfTest, DataTransferPerformance) {
  const size_t sizes[] = {
      1024 * 1024,       // 1MB
      10 * 1024 * 1024,  // 10MB
      100 * 1024 * 1024  // 100MB
  };

  for (size_t size : sizes) {
    std::vector<float> host_data = createTestData(size / sizeof(float));
    std::vector<float> result_data(size / sizeof(float));
    void* device_ptr = nullptr;

    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&device_ptr, size));

    const int iterations = 100;
    std::vector<double> h2d_times;
    std::vector<double> d2h_times;
    h2d_times.reserve(iterations);
    d2h_times.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
      // Measure H2D transfer
      h2d_times.push_back(measureOperation([&]() {
        ASSERT_EQ(sep::SEPResult::SUCCESS,
                  context_->copyHostToDevice(device_ptr, host_data.data(), size));
      }));

      // Measure D2H transfer
      d2h_times.push_back(measureOperation([&]() {
        ASSERT_EQ(sep::SEPResult::SUCCESS,
                  context_->copyDeviceToHost(result_data.data(), device_ptr, size));
      }));
    }

    TimingStats h2d_stats(h2d_times);
    TimingStats d2h_stats(d2h_times);

    std::cout << "\nTransfer size: " << (size / (1024 * 1024)) << "MB\n";
    h2d_stats.print("Host to Device");
    d2h_stats.print("Device to Host");

    double bandwidth_h2d = (size / (1024.0 * 1024.0)) / (h2d_stats.avg / 1000.0);  // GB/s
    double bandwidth_d2h = (size / (1024.0 * 1024.0)) / (d2h_stats.avg / 1000.0);  // GB/s

    std::cout << "Bandwidth:\n"
              << "  H2D: " << std::fixed << std::setprecision(2) << bandwidth_h2d << " GB/s\n"
              << "  D2H: " << bandwidth_d2h << " GB/s\n";

    EXPECT_GT(bandwidth_h2d, 10.0) << "H2D bandwidth below 10 GB/s";
    EXPECT_GT(bandwidth_d2h, 10.0) << "D2H bandwidth below 10 GB/s";

    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(device_ptr));
  }
}

TEST_F(GPUContextPerfTest, ConcurrentStreamPerformance) {
  const int num_streams = 8;
  const size_t size_per_stream = 10 * 1024 * 1024;  // 10MB
  std::vector<cudaStream_t> streams(num_streams);
  std::vector<void*> device_ptrs(num_streams);
  std::vector<std::vector<float>> host_data(num_streams);
  std::vector<std::vector<float>> result_data(num_streams);

  // Initialize
  for (int i = 0; i < num_streams; ++i) {
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->createStream(streams[i]));
    ASSERT_EQ(sep::SEPResult::SUCCESS, context_->allocateMemory(&device_ptrs[i], size_per_stream));
    host_data[i] = createTestData(size_per_stream / sizeof(float));
    result_data[i].resize(size_per_stream / sizeof(float));
  }

  const int iterations = 100;
  std::vector<double> concurrent_times;
  std::vector<double> sequential_times;
  concurrent_times.reserve(iterations);
  sequential_times.reserve(iterations);

  for (int iter = 0; iter < iterations; ++iter) {
    // Measure concurrent transfers
    concurrent_times.push_back(measureOperation([&]() {
      for (int i = 0; i < num_streams; ++i) {
        ASSERT_EQ(sep::SEPResult::SUCCESS,
                  context_->copyHostToDevice(device_ptrs[i], host_data[i].data(), size_per_stream));
      }
      for (int i = 0; i < num_streams; ++i) {
        ASSERT_EQ(sep::SEPResult::SUCCESS, context_->synchronizeStream(streams[i]));
      }
    }));

    // Measure sequential transfers
    double sequential_time = 0.0;
    for (int i = 0; i < num_streams; ++i) {
      sequential_time += measureOperation([&]() {
        ASSERT_EQ(sep::SEPResult::SUCCESS,
                  context_->copyHostToDevice(device_ptrs[i], host_data[i].data(), size_per_stream));
        ASSERT_EQ(sep::SEPResult::SUCCESS, context_->synchronizeStream(streams[i]));
      });
    }
    sequential_times.push_back(sequential_time);
  }

  TimingStats concurrent_stats(concurrent_times);
  TimingStats sequential_stats(sequential_times);

  std::cout << "\nStream performance:\n";
  concurrent_stats.print("Concurrent transfers");
  sequential_stats.print("Sequential transfers");

  double avg_speedup = sequential_stats.avg / concurrent_stats.avg;
  std::cout << "Average speedup: " << std::fixed << std::setprecision(2) << avg_speedup << "x\n";

  EXPECT_GT(avg_speedup, 2.0) << "Insufficient concurrent performance gain";

  // Cleanup
  for (int i = 0; i < num_streams; ++i) {
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->freeMemory(device_ptrs[i]));
    EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyStream(streams[i]));
  }
}

TEST_F(GPUContextPerfTest, StreamLatencyTest) {
  const int iterations = 1000;
  cudaStream_t stream;
  ASSERT_EQ(sep::SEPResult::SUCCESS, context_->createStream(stream));

  std::vector<double> latencies;
  latencies.reserve(iterations);

  for (int i = 0; i < iterations; ++i) {
    latencies.push_back(measureOperation(
        [&]() { ASSERT_EQ(sep::SEPResult::SUCCESS, context_->synchronizeStream(stream)); }));
  }

  TimingStats stats(latencies);
  stats.print("Stream synchronization");

  EXPECT_LT(stats.avg, 0.1) << "Average stream sync latency too high";
  EXPECT_LT(stats.max, 1.0) << "Maximum stream sync latency too high";

  EXPECT_EQ(sep::SEPResult::SUCCESS, context_->destroyStream(stream));
}

