#include "quantum/bitspace/qbsa.h"

#include <cstdint>
#include <vector>
#include "quantum/bitspace/trajectory.h"

namespace sep::quantum {

QBSAProcessor::QBSAProcessor(const QBSAOptions& options) : options_(options) {}

QBSAResult QBSAProcessor::analyze(const std::vector<uint32_t>& probe_indices,
                                  const std::vector<uint32_t>& expectations)
{
    QBSAResult result{};
    if (probe_indices.empty() || probe_indices.size() != expectations.size()) return result;

    // Count corrections needed
    for (std::size_t i = 0; i < probe_indices.size(); ++i)
    {
        if (probe_indices[i] != expectations[i])
        {
            result.corrections.push_back(static_cast<uint32_t>(i));
        }
    }

    // Calculate correction ratio
    result.correction_ratio =
        static_cast<float>(result.corrections.size()) / static_cast<float>(probe_indices.size());

    // Detect collapse based on correction ratio
    result.collapse_detected = (result.correction_ratio >= options_.collapse_threshold);

    return result;
}

bool QBSAProcessor::detectCollapse(const QBSAResult& result, std::size_t total_bits) const {
  // If we already detected collapse in the result, return that 
  if (result.collapse_detected) {
    return true;
  }

  // Additional check based on total bits in the system
  if (total_bits > 0) {
    float error_density =
        static_cast<float>(result.corrections.size()) / static_cast<float>(total_bits);
    return error_density >= options_.collapse_threshold;
  }

  return false;
}

const QBSAOptions& QBSAProcessor::getOptions() const {
  return options_;
}

QBSAResult QBSAProcessor::duplicateForPackage(const std::vector<uint8_t>& bitstream) {
    // This is a simplified implementation. A real implementation would involve
    // creating a new QBSAProcessor instance for each package and processing it.
    QBSAResult result;
    double accumulated_value = 0.0;
    double lambda = 0.1; // Decay constant, should be tuned

    for (size_t i = 1; i < bitstream.size(); ++i) {
        double future_bit = bitstream[i];
        double current_bit = bitstream[0];
        accumulated_value += (future_bit - current_bit) * std::exp(-lambda * i);
    }

    result.damped_value = accumulated_value;
    return result;
}

}  // namespace sep::quantum
