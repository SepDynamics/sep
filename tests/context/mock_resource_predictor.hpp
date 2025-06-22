#ifndef SEP_CONTEXT_MOCK_RESOURCE_PREDICTOR_HPP
#define SEP_CONTEXT_MOCK_RESOURCE_PREDICTOR_HPP

#include "context/resource_predictor.h"
#include "context/types.h"
#include <algorithm>
#include <cstddef>
#include <deque>
#include <vector>

namespace sep::context {

class MockResourcePredictor : public ResourcePredictor {
private:
    std::deque<Batch> history_;
    std::deque<UsagePattern> usage_patterns_;
    std::size_t max_history_size_;
    ResourceState current_state_;
    ResourceMetrics metrics_;

public:
    explicit MockResourcePredictor(std::size_t history_size);

    ResourcePrediction predictResourceNeeds(const Batch& batch) override;

    void recordBatchProcessing(const Batch& batch, std::size_t memory_used,
                            double processing_time) override;

    void recordUsagePattern(const UsagePattern& pattern) override;

    ResourceState getCurrentState() const override;

    void updateResourceLimits(std::size_t max_memory, float max_cpu_usage,
                            float max_gpu_usage) override;

    void resetModel() override;

    ResourceMetrics getResourceMetrics() const override;

    float calculateResourceEfficiency() const override;

    std::vector<std::size_t>
    suggestBatchSizes(std::size_t target_throughput) const override;
};

} // namespace sep::context
} // namespace sep

#endif // SEP_CONTEXT_MOCK_RESOURCE_PREDICTOR_HPP
