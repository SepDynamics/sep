#include "context/resource_predictor.h"

#include <gtest/gtest.h>

#include <memory>
#include <thread>
#include <vector>

#include "context/types.h"
#include "sep/result_defs.h"
#include "mock_resource_predictor.hpp"

using namespace sep::context;

namespace sep::context {
std::unique_ptr<ResourcePredictor> createResourcePredictor(size_t history_size);
}

class ResourcePredictorTest : public ::testing::Test {
 protected:
  MockResourcePredictor predictor_{100};  // Direct member instead of unique_ptr

  void SetUp() override {
    // Add some sample usage patterns
    for (size_t i = 0; i < 5; ++i) {
      UsagePattern pattern;
      pattern.memory_used = 1024 * 1024 * (i + 1);  // Increasing memory usage
      pattern.batch_size = 10 * (i + 1);            // Increasing batch size
      pattern.processing_time = 0.1 * (i + 1);      // Increasing processing time
      pattern.cpu_utilization = 0.2f * (i + 1);     // Increasing CPU utilization
      pattern.gpu_utilization = 0.2f * (i + 1);     // Increasing GPU utilization
      predictor_.recordUsagePattern(pattern);
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
};

TEST_F(ResourcePredictorTest, PredictResourceNeeds) {
  Batch batch;
  batch.contexts.resize(20);  // Batch size of 20

  auto prediction = predictor_.predictResourceNeeds(batch);

  EXPECT_GT(prediction.estimated_memory, 0);
  EXPECT_GT(prediction.optimal_batch_size, 0);
  EXPECT_GT(prediction.expected_processing_time, 0);
  EXPECT_GE(prediction.confidence_score, 0.0f);
  EXPECT_LE(prediction.confidence_score, 1.0f);
}

TEST_F(ResourcePredictorTest, ResourceMetrics) {
  auto metrics = predictor_.getResourceMetrics();

  EXPECT_GT(metrics.peak_memory_usage, 0);
  EXPECT_GT(metrics.average_memory_usage, 0);
  EXPECT_GT(metrics.average_processing_time, 0);
  EXPECT_GT(metrics.total_batches_processed, 0);
  EXPECT_GE(metrics.resource_efficiency, 0.0f);
  EXPECT_LE(metrics.resource_efficiency, 1.0f);
}

TEST_F(ResourcePredictorTest, BatchSizeSuggestions) {
  auto batch_sizes = predictor_.suggestBatchSizes(100);

  EXPECT_FALSE(batch_sizes.empty());
  size_t total_contexts = 0;
  for (auto size : batch_sizes) {
    EXPECT_GT(size, 0);
    total_contexts += size;
  }
  EXPECT_EQ(total_contexts, 100);
}

TEST_F(ResourcePredictorTest, ResourceEfficiency) {
  float efficiency = predictor_.calculateResourceEfficiency();

  EXPECT_GE(efficiency, 0.0f);
  EXPECT_LE(efficiency, 1.0f);
}

TEST_F(ResourcePredictorTest, HistoryManagement) {
  // Fill history beyond capacity
  for (size_t i = 0; i < 200; ++i) {
    UsagePattern pattern;
    pattern.memory_used = 1024 * 1024;
    pattern.batch_size = 10;
    pattern.processing_time = 0.1;
    pattern.gpu_utilization = 0.5f;
    predictor_.recordUsagePattern(pattern);
  }

  auto metrics = predictor_.getResourceMetrics();
  EXPECT_LE(metrics.total_batches_processed,
            100);  // Should respect history size
}

TEST_F(ResourcePredictorTest, AnomalyDetection) {
  // Record normal pattern
  UsagePattern normal_pattern;
  normal_pattern.memory_used = 1024 * 1024;
  normal_pattern.batch_size = 10;
  normal_pattern.processing_time = 0.1;
  normal_pattern.gpu_utilization = 0.5f;
  predictor_.recordUsagePattern(normal_pattern);

  // Record anomalous pattern (10x higher values)
  UsagePattern high_pattern;
  high_pattern.memory_used = 10 * 1024 * 1024;
  high_pattern.batch_size = 100;
  high_pattern.processing_time = 1.0;
  high_pattern.gpu_utilization = 1.0f;
  predictor_.recordUsagePattern(high_pattern);

  auto metrics = predictor_.getResourceMetrics();
  // Average should not be heavily influenced by the anomaly
  EXPECT_LT(metrics.average_memory_usage, 5 * 1024 * 1024);
}

TEST(ResourcePredictorImplMetrics, SystemMetricsQuery) {
  auto predictor = createResourcePredictor(5);
  auto state = predictor->getCurrentState();

  EXPECT_GT(state.total_memory, 0u);
  EXPECT_GT(state.free_memory, 0u);
  EXPECT_LE(state.free_memory, state.total_memory);
  EXPECT_GE(state.cpu_utilization, 0.0f);
  EXPECT_LE(state.cpu_utilization, 1.0f);
}
