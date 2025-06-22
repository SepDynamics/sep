#include "mock_resource_predictor.hpp"
#include <algorithm>

namespace sep::context {

MockResourcePredictor::MockResourcePredictor(std::size_t history_size)
    : max_history_size_(history_size) {
    current_state_.used_memory = 0;
    current_state_.cpu_utilization = 0.0f;
    current_state_.gpu_utilization = 0.0f;
    current_state_.active_batches = 0;
    current_state_.total_memory = 1024 * 1024 * 1024;  // 1GB
    current_state_.free_memory = current_state_.total_memory;
}

ResourcePrediction MockResourcePredictor::predictResourceNeeds(const Batch& batch) {
    ResourcePrediction prediction;
    prediction.estimated_memory = batch.contexts.size() * 1024 * 1024; // 1MB per context
    prediction.optimal_batch_size = std::min(batch.contexts.size(), max_history_size_);
    prediction.expected_processing_time = 0.1 * batch.contexts.size();
    prediction.confidence_score = 0.9f;
    prediction.estimated_cpu_usage = 0.5f;
    prediction.estimated_gpu_usage = 0.7f;
    return prediction;
}

void MockResourcePredictor::recordBatchProcessing(const Batch& batch,
                                               std::size_t memory_used,
                                               double processing_time) {
    if (history_.size() >= max_history_size_) {
        history_.pop_front();
    }
    history_.push_back(batch);

    metrics_.peak_memory_usage = std::max(metrics_.peak_memory_usage, memory_used);
    metrics_.average_memory_usage = (metrics_.average_memory_usage + memory_used) / 2;
    metrics_.average_processing_time = (metrics_.average_processing_time + processing_time) / 2;
    metrics_.total_batches_processed++;
}

void MockResourcePredictor::recordUsagePattern(const UsagePattern& pattern) {
    if (usage_patterns_.size() >= max_history_size_) {
        usage_patterns_.pop_front();
    }
    usage_patterns_.push_back(pattern);
}

ResourceState MockResourcePredictor::getCurrentState() const {
    return current_state_;
}

void MockResourcePredictor::updateResourceLimits(std::size_t max_memory,
                                              float max_cpu_usage,
                                              float max_gpu_usage) {
    current_state_.used_memory = std::min(current_state_.used_memory, max_memory);
    current_state_.cpu_utilization = std::min(current_state_.cpu_utilization, max_cpu_usage);
    current_state_.gpu_utilization = std::min(current_state_.gpu_utilization, max_gpu_usage);
    current_state_.total_memory = max_memory;
    current_state_.free_memory = max_memory - current_state_.used_memory;
}

void MockResourcePredictor::resetModel() {
    history_.clear();
    usage_patterns_.clear();
    current_state_ = ResourceState{};
    metrics_ = ResourceMetrics{};
}

ResourceMetrics MockResourcePredictor::getResourceMetrics() const {
    return metrics_;
}

float MockResourcePredictor::calculateResourceEfficiency() const {
    if (metrics_.total_batches_processed == 0) {
        return 0.0f;
    }
    return 1.0f - (metrics_.average_memory_usage / metrics_.peak_memory_usage);
}

std::vector<std::size_t> MockResourcePredictor::suggestBatchSizes(std::size_t target_throughput) const {
    std::vector<std::size_t> batch_sizes;
    std::size_t remaining = target_throughput;
    std::size_t optimal_size = max_history_size_ / 2;

    while (remaining > 0) {
        std::size_t batch_size = std::min(remaining, optimal_size);
        batch_sizes.push_back(batch_size);
        remaining -= batch_size;
    }

    return batch_sizes;
}

} // namespace sep::context
