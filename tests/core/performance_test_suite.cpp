#include "core/engine.h"
#include "core/metrics_collector.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace sep {
namespace core {
namespace test {

class PerformanceTestSuite : public ::testing::Test {
protected:
  void SetUp() override {
    collector = std::make_unique<MetricsCollector>();
    engine = std::make_unique<Engine>();
    ASSERT_TRUE(engine->init());
    collector->startCollection();

    // Warm up the system
    warmupSystem();
  }

  void TearDown() override { collector->stopCollection(); }

  void warmupSystem() {
    // Run some operations to warm up the GPU
    std::vector<sep::PinState> warmup_inputs(1024);
    std::vector<uint32_t> indices, expectations;

    for (int i = 0; i < 5; ++i) {
      engine->generate_probes(warmup_inputs, indices, expectations, 0);
    }

    // Wait for system to stabilize
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  void validatePerformanceThresholds(const DetailedMetrics &metrics) {
    // GPU utilization is currently not collected
    EXPECT_FLOAT_EQ(metrics.gpu_utilization.load(), 0.0f);

    // Memory usage should be efficient (below 85%)
    EXPECT_LE(metrics.gpu_memory_usage.load(), 85.0f);

    // Kernel execution time should be within expected range
    EXPECT_LE(metrics.kernel_execution_time.load(), 100.0f); // 100ms max
  }

  std::unique_ptr<MetricsCollector> collector;
  std::unique_ptr<Engine> engine;
};

TEST_F(PerformanceTestSuite, KernelExecutionPerformance) {
  const size_t batch_size = 1024;
  std::vector<sep::PinState> inputs(batch_size);
  std::vector<uint32_t> indices, expectations;

  // Start metrics collection
  collector->resetMetrics();

  // Run batch processing
  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 10; ++i) {
    engine->generate_probes(inputs, indices, expectations, i);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  // Get metrics
  DetailedMetrics metrics = collector->getCurrentMetrics();

  // Validate performance
  validatePerformanceThresholds(metrics);

  // Average processing time should be under 10ms per batch
  EXPECT_LE(duration.count() / 10, 10);
}

TEST_F(PerformanceTestSuite, MemoryOverheadTest) {
  const size_t batch_size = 1024;
  std::vector<sep::PinState> inputs(batch_size);
  cuda::QBSAResult qbsa_result;
  cuda::QSHResult qsh_result;

  // Start metrics collection
  collector->resetMetrics();

  // Process multiple batches
  for (int i = 0; i < 5; ++i) {
    engine->process_batch(inputs, i, qbsa_result, qsh_result);
  }

  // Get metrics
  DetailedMetrics metrics = collector->getCurrentMetrics();

  // Validate memory efficiency
  EXPECT_LE(metrics.gpu_memory_usage.load(), 85.0f); // Below 85% usage
}

TEST_F(PerformanceTestSuite, ConcurrentProcessingTest) {
  const size_t batch_size = 512;
  const int num_threads = 4;
  std::vector<std::thread> threads;

  // Start metrics collection
  collector->resetMetrics();

  // Launch multiple processing threads
  for (int t = 0; t < num_threads; ++t) {
    threads.emplace_back([this, batch_size]() {
      std::vector<sep::PinState> inputs(batch_size);
      std::vector<uint32_t> indices, expectations;

      for (int i = 0; i < 5; ++i) {
        engine->generate_probes(inputs, indices, expectations, i);
      }
    });
  }

  // Wait for all threads to complete
  for (auto &thread : threads) {
    thread.join();
  }

  // Get metrics
  DetailedMetrics metrics = collector->getCurrentMetrics();

  // Validate concurrent performance
  validatePerformanceThresholds(metrics);
}

TEST_F(PerformanceTestSuite, StabilityTest) {
  const size_t batch_size = 1024;
  std::vector<sep::PinState> inputs(batch_size);
  cuda::QBSAResult qbsa_result;
  cuda::QSHResult qsh_result;

  // Run continuous processing for 5 seconds
  auto end_time = std::chrono::steady_clock::now() + std::chrono::seconds(5);
  int iterations = 0;

  while (std::chrono::steady_clock::now() < end_time) {
    engine->process_batch(inputs, iterations++, qbsa_result, qsh_result);

    DetailedMetrics metrics = collector->getCurrentMetrics();
    validatePerformanceThresholds(metrics);

    // GPU utilization remains placeholder
    EXPECT_FLOAT_EQ(metrics.gpu_utilization.load(), 0.0f);
  }

  // Verify system remained stable
  DetailedMetrics final_metrics = collector->getCurrentMetrics();
  EXPECT_GE(iterations,
            100); // Should complete at least 100 iterations in 5 seconds
  validatePerformanceThresholds(final_metrics);
}

} // namespace test
} // namespace core
} // namespace sep
