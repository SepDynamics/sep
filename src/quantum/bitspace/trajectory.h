#ifndef SEP_QUANTUM_BITSPACE_TRAJECTORY_H
#define SEP_QUANTUM_BITSPACE_TRAJECTORY_H

#include <vector>
#include <string>
#include <cmath>

namespace sep::quantum::bitspace {

// Represents a single point in a time-series trajectory
struct TrajectoryPoint {
    double value;
    uint64_t timestamp;
};

// Defines the decay function for weighting future points
enum class DecayModel {
    EXPONENTIAL,
    LINEAR
};

// Holds the final damped value and its history
struct DampedValue {
    double final_value = 0.0;
    std::vector<double> path;
    double confidence = 0.0;
    bool converged = false;
};

// Manages a trajectory and calculates its damped value
class Trajectory {
public:
    explicit Trajectory(std::vector<TrajectoryPoint> points, DecayModel model = DecayModel::EXPONENTIAL)
        : points_(std::move(points)), decay_model_(model) {}

    // Calculates the damped value by integrating future points
    DampedValue calculateDampedValue(double decay_rate = 0.1, double convergence_threshold = 1e-5, int max_iterations = 100) {
        DampedValue result;
        if (points_.empty()) {
            return result;
        }

        double current_value = points_[0].value;
        result.path.push_back(current_value);

        for (int i = 0; i < max_iterations; ++i) {
            double next_value = 0.0;
            double total_weight = 0.0;

            for (size_t j = 0; j < points_.size(); ++j) {
                double weight = calculateWeight(j, decay_rate);
                next_value += points_[j].value * weight;
                total_weight += weight;
            }

            if (total_weight > 0) {
                next_value /= total_weight;
            }

            result.path.push_back(next_value);

            if (std::abs(next_value - current_value) < convergence_threshold) {
                result.converged = true;
                break;
            }
            current_value = next_value;
        }

        result.final_value = current_value;
        return result;
    }

private:
    double calculateWeight(size_t index, double decay_rate) const {
        switch (decay_model_) {
            case DecayModel::EXPONENTIAL:
                return std::exp(-static_cast<double>(index) * decay_rate);
            case DecayModel::LINEAR:
                return std::max(0.0, 1.0 - static_cast<double>(index) * decay_rate);
            default:
                return 1.0;
        }
    }

    std::vector<TrajectoryPoint> points_;
    DecayModel decay_model_;
};

} // namespace sep::quantum::bitspace

#endif // SEP_QUANTUM_BITSPACE_TRAJECTORY_H
