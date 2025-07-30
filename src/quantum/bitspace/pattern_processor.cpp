#include "pattern_processor.h"

#include <numeric>
#include <cmath>

namespace sep::quantum::bitspace {

PatternProcessor::PatternProcessor(std::map<std::string, std::vector<double>> historical_paths)
    : historical_paths_(std::move(historical_paths)) {}

Metrics PatternProcessor::processTrajectory(Trajectory& trajectory) {
    DampedValue damped_value = trajectory.calculateDampedValue();
    
    Metrics metrics;
    metrics.coherence = calculateCoherence(damped_value);
    metrics.stability = calculateStability(damped_value);
    metrics.entropy = calculateEntropy(damped_value);
    metrics.confidence = matchHistoricalPaths(damped_value.path);
    
    return metrics;
}

double PatternProcessor::calculateCoherence(const DampedValue& damped_value) const {
    if (damped_value.path.size() < 2) {
        return 0.5; // Neutral coherence for single-point path
    }
    // Coherence: 1 - normalized variance of the path
    double mean = std::accumulate(damped_value.path.begin(), damped_value.path.end(), 0.0) / damped_value.path.size();
    double sq_sum = std::inner_product(damped_value.path.begin(), damped_value.path.end(), damped_value.path.begin(), 0.0);
    double variance = (sq_sum / damped_value.path.size()) - (mean * mean);
    return 1.0 - std::min(1.0, variance); // Clamp to [0, 1]
}

double PatternProcessor::calculateStability(const DampedValue& damped_value) const {
    if (!damped_value.converged) {
        return 0.0; // Unstable if not converged
    }
    // Stability: Inverse of the number of steps to converge, normalized
    double stability = 1.0 / static_cast<double>(damped_value.path.size());
    return std::max(0.0, std::min(1.0, stability * 10.0)); // Scale and clamp
}

double PatternProcessor::calculateEntropy(const DampedValue& damped_value) const {
    if (damped_value.path.size() < 2) {
        return 0.0;
    }
    // Entropy: Mean of absolute differences between path points
    double total_diff = 0.0;
    for (size_t i = 1; i < damped_value.path.size(); ++i) {
        total_diff += std::abs(damped_value.path[i] - damped_value.path[i-1]);
    }
    return total_diff / (damped_value.path.size() - 1);
}

double PatternProcessor::matchHistoricalPaths(const std::vector<double>& current_path) const {
    if (historical_paths_.empty()) {
        return 0.5; // Neutral confidence if no history
    }

    double max_similarity = 0.0;

    // Using cosine similarity for path matching
    for (const auto& pair : historical_paths_) {
        const auto& historical_path = pair.second;
        if (current_path.size() != historical_path.size() || current_path.empty()) {
            continue;
        }

        double dot_product = 0.0;
        double norm_current = 0.0;
        double norm_historical = 0.0;

        for (size_t i = 0; i < current_path.size(); ++i) {
            dot_product += current_path[i] * historical_path[i];
            norm_current += current_path[i] * current_path[i];
            norm_historical += historical_path[i] * historical_path[i];
        }

        if (norm_current > 0 && norm_historical > 0) {
            double similarity = dot_product / (std::sqrt(norm_current) * std::sqrt(norm_historical));
            if (similarity > max_similarity) {
                max_similarity = similarity;
            }
        }
    }

    return max_similarity;
}

} // namespace sep::quantum::bitspace
