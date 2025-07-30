#ifndef SEP_QUANTUM_BITSPACE_PATTERN_PROCESSOR_H
#define SEP_QUANTUM_BITSPACE_PATTERN_PROCESSOR_H

#include <vector>
#include <string>
#include <map>

#include "trajectory.h"

namespace sep::quantum::bitspace {

struct Metrics {
    double coherence = 0.0;
    double stability = 0.0;
    double entropy = 0.0;
    double confidence = 0.0;
};

class PatternProcessor {
public:
    explicit PatternProcessor(std::map<std::string, std::vector<double>> historical_paths = {});

    Metrics processTrajectory(Trajectory& trajectory);

private:
    double calculateCoherence(const DampedValue& damped_value) const;
    double calculateStability(const DampedValue& damped_value) const;
    double calculateEntropy(const DampedValue& damped_value) const;
    double matchHistoricalPaths(const std::vector<double>& current_path) const;

    std::map<std::string, std::vector<double>> historical_paths_;
};

} // namespace sep::quantum::bitspace

#endif // SEP_QUANTUM_BITSPACE_PATTERN_PROCESSOR_H
