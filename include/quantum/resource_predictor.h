#ifndef SEP_CONTEXT_RESOURCE_PREDICTOR_H
#define SEP_CONTEXT_RESOURCE_PREDICTOR_H

#include "types.h"
#include "compat/shim.h"

namespace sep::context {

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
  virtual ::sep::shim::vector<std::size_t>
  suggestBatchSizes(std::size_t target_throughput) const = 0;
};

} // namespace sep::context

#endif // SEP_CONTEXT_RESOURCE_PREDICTOR_H
