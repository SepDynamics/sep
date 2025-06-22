#include "core/metrics_collector.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace sep {
namespace core {
namespace test {

class MetricsCollectorTest : public ::testing::Test {
protected:
  void SetUp() override { collector = std::make_unique<MetricsCollector>(); }

  void TearDown() override {
    collector->stopCollection();
    collector.reset();
  }

  std::unique_ptr<MetricsCollector> collector;
};

TEST_F(MetricsCollectorTest, InitializationTest) {
  ASSERT_NE(collector, nullptr);

  DetailedMetrics initial = collector->getCurrentMetrics();
  EXPECT_EQ(initial.gpu_utilization.load(), 0.0f);
  EXPECT_EQ(initial.gpu_memory_usage.load(), 0.0f);
  EXPECT_EQ(initial.kernel_execution_time.load(), 0.0f);
}

TEST_F(MetricsCollectorTest, CollectionTest) {
  collector->startCollection();

  // Wait for some metrics to be collected
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  DetailedMetrics metrics = collector->getCurrentMetrics();
  EXPECT_GE(metrics.gpu_utilization.load(), 0.0f);
  EXPECT_LE(metrics.gpu_utilization.load(), 100.0f);
  EXPECT_GE(metrics.gpu_memory_usage.load(), 0.0f);
  EXPECT_LE(metrics.gpu_memory_usage.load(), 100.0f);
}

TEST_F(MetricsCollectorTest, ResetTest) {
  collector->startCollection();
  std::this_thread::sleep_for(std::chrono::milliseconds(200));

  // Get metrics before reset
  DetailedMetrics before = collector->getCurrentMetrics();

  // Reset metrics
  collector->resetMetrics();

  // Get metrics after reset
  DetailedMetrics after = collector->getCurrentMetrics();

  // Verify all metrics were reset
  EXPECT_EQ(after.gpu_utilization.load(), 0.0f);
  EXPECT_EQ(after.gpu_memory_usage.load(), 0.0f);
  EXPECT_EQ(after.kernel_execution_time.load(), 0.0f);
}

TEST_F(MetricsCollectorTest, UpdateMetricsTest) {
  DetailedMetrics new_metrics;
  new_metrics.gpu_utilization.store(50.0f);
  new_metrics.gpu_memory_usage.store(75.0f);
  new_metrics.kernel_execution_time.store(1.5f);

  collector->updateMetrics(new_metrics);

  DetailedMetrics current = collector->getCurrentMetrics();
  EXPECT_EQ(current.gpu_utilization.load(), 50.0f);
  EXPECT_EQ(current.gpu_memory_usage.load(), 75.0f);
  EXPECT_EQ(current.kernel_execution_time.load(), 1.5f);
}

TEST_F(MetricsCollectorTest, StopCollectionTest) {
  collector->startCollection();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  // Stop collection
  collector->stopCollection();

  // Get metrics after stopping
  DetailedMetrics metrics1 = collector->getCurrentMetrics();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  DetailedMetrics metrics2 = collector->getCurrentMetrics();

  // Metrics should be the same since collection was stopped
  EXPECT_EQ(metrics1.gpu_utilization.load(), metrics2.gpu_utilization.load());
  EXPECT_EQ(metrics1.gpu_memory_usage.load(), metrics2.gpu_memory_usage.load());
  EXPECT_EQ(metrics1.kernel_execution_time.load(),
            metrics2.kernel_execution_time.load());
}

} // namespace test
} // namespace core
} // namespace sep
