#ifndef SEP_CONTEXT_RESOURCE_PREDICTOR_H
#define SEP_CONTEXT_RESOURCE_PREDICTOR_H

#include <cstddef>
#include <nlohmann/json.hpp>
#include <vector>

#include "engine/internal/standard_includes.h"

// Minimal context types for compilation
namespace sep::context {
struct Context {
    std::string type;
    nlohmann::json content;
    std::vector<nlohmann::json> relationships;
    std::vector<std::string> tags;
    nlohmann::json metadata;
    nlohmann::json processorResult;
};

struct CheckResult {
  enum class Status { VALID = 0, INVALID = 1, STABLE = 2 };
  Status status{Status::VALID};
  float score{0.0f};
  std::string error;
};

struct Batch {
    std::string layer;
    std::vector<Context> contexts;
};

struct ResourcePrediction {
  std::size_t estimated_memory{0};
  std::size_t optimal_batch_size{0};
  double expected_processing_time{0.0};
  float confidence_score{0.0f};
  float estimated_cpu_usage{0.0f};
  float estimated_gpu_usage{0.0f};
};

struct UsagePattern {
  std::size_t memory_used{0};
  std::size_t batch_size{0};
  double processing_time{0.0};
  float cpu_utilization{0.0f};
  float gpu_utilization{0.0f};
};

struct ResourceState {
  std::size_t total_memory{0};
  std::size_t free_memory{0};
  std::size_t used_memory{0};
  std::size_t active_batches{0};
  float cpu_utilization{0.0f};
  float gpu_utilization{0.0f};
};

struct ResourceMetrics {
  std::size_t peak_memory_usage{0};
  std::size_t average_memory_usage{0};
  double average_processing_time{0.0};
  std::size_t total_batches_processed{0};
  float resource_efficiency{0.0f};
};

class ResourcePredictor {
public:
  virtual ~ResourcePredictor() = default;

  // Predict resource needs for a batch
  virtual ResourcePrediction predictResourceNeeds(const Batch &batch) = 0;

  // Record actual resource usage for a batch
  virtual void recordBatchProcessing(const Batch &batch,
                                     std::size_t memory_used,
                                     double processing_time) = 0;

  // Get current resource state
  virtual ResourceState getCurrentState() const = 0;

  // Update resource limits
  virtual void updateResourceLimits(std::size_t max_memory, float max_cpu_usage,
                                    float max_gpu_usage) = 0;

  // Reset prediction model
  virtual void resetModel() = 0;

  // Record usage pattern
  virtual void recordUsagePattern(const UsagePattern &pattern) = 0;

  // Get resource metrics
  virtual ResourceMetrics getResourceMetrics() const = 0;

  // Calculate resource efficiency
  virtual float calculateResourceEfficiency() const = 0;

  // Suggest batch sizes for target throughput
  virtual std::vector<std::size_t> suggestBatchSizes(std::size_t target_throughput) const = 0;
};

} // namespace sep::context

#endif // SEP_CONTEXT_RESOURCE_PREDICTOR_H
