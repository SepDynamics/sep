#include "quantum/qbsa_qfh.h"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <vector>

#include "engine/internal/standard_includes.h"
#include "quantum/bitspace/qbsa.h"
#include "quantum/bitspace/qfh.h"

namespace sep::quantum {

// QFH-based implementation of the QBSA interface
class QFHBasedQBSAProcessor : public QBSAProcessor {
public:
    explicit QFHBasedQBSAProcessor(const QBSAOptions& options = {})
        : QBSAProcessor(options), qfh_processor_(createQFHOptions(options)) {}

    QBSAResult analyze(const std::vector<uint32_t>& probe_indices,
                       const std::vector<uint32_t>& expectations) override
    {
        QBSAResult result{};
        if (probe_indices.empty() || probe_indices.size() != expectations.size()) return result;

        // Compare each uint32_t value directly to identify which indices need correction
        for (size_t i = 0; i < probe_indices.size(); ++i) {
            if (probe_indices[i] != expectations[i]) {
                result.corrections.push_back(static_cast<uint32_t>(i));
            }
        }

        // Calculate correction ratio
        result.correction_ratio = static_cast<float>(result.corrections.size()) /
                                 static_cast<float>(probe_indices.size());

        // Use QFH to analyze the probe bits for collapse detection
        std::vector<uint8_t> probe_bits = convertToBits(probe_indices);
        QFHResult qfh_result = qfh_processor_.analyze(probe_bits);

        // Detect collapse based on rupture ratio from QFH
        result.collapse_detected = qfh_result.collapse_detected;

        return result;
    }

    bool detectCollapse(const QBSAResult& result, std::size_t total_bits) const override {
        // If we already detected collapse in the result, return that
        if (result.collapse_detected) {
            return true;
        }

        // Additional check based on total bits in the system
        if (total_bits > 0) {
            float error_density =
                static_cast<float>(result.corrections.size()) / static_cast<float>(total_bits);
            return error_density >= getOptions().collapse_threshold;
        }

        return false;
    }

private:
    QFHBasedProcessor qfh_processor_;

    // Convert QBSA options to QFH options
    static QFHOptions createQFHOptions(const QBSAOptions& qbsa_options) {
        QFHOptions qfh_options;
        qfh_options.collapse_threshold = qbsa_options.collapse_threshold;
        return qfh_options;
    }

    // Convert uint32_t values to bit sequences
    std::vector<uint8_t> convertToBits(const std::vector<uint32_t>& values)
    {
        std::vector<uint32_t> shim_values;
        shim_values.reserve(values.size());
        for (uint32_t v : values) {
            shim_values.push_back(v);
        }
        return QFHBasedProcessor::convertToBits(shim_values);
    }
};

// Factory function to create a QFH-based QBSA processor
std::unique_ptr<QBSAProcessor> createQFHBasedQBSAProcessor(const QBSAOptions& options) {
    return std::unique_ptr<QBSAProcessor>(new QFHBasedQBSAProcessor(options));
}

} // namespace sep::quantum
