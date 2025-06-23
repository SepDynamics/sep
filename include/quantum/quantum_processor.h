#pragma once

// Compatibility header - redirects to new unified processor
#include "quantum/processor.h"
#include <stdexcept>
#include <glm/vec3.hpp>
#include <memory>

namespace sep::quantum {

class QuantumProcessorError : public std::runtime_error {
public:
    explicit QuantumProcessorError(const std::string& message);
};

class QBSAProcessor;
class QuantumProcessorImpl;

// Forward compatibility - QuantumProcessor is now just Processor
// This allows existing code to continue working
class QuantumProcessor : public Processor {
public:
    using Processor::Processor;

    // Legacy Config type mapping
    struct Config {
        size_t max_qubits = 16;
        float decoherence_rate = 0.01f;
        float measurement_threshold = 0.5f;
        bool enable_gpu = false;

        // Convert to new ProcessingConfig
        operator ProcessingConfig() const;
    };

    QuantumProcessor() = default;
    explicit QuantumProcessor(const Config& config);
    ~QuantumProcessor();

    float calculateCoherence(const glm::vec3& a, const glm::vec3& b);
    float calculateStability(float coherence, float historical_stability,
                             float generation_count, float access_frequency);
    bool processPattern(const glm::vec3& data, size_t id);
    bool updatePattern(size_t id, const glm::vec3& data);
    void removePattern(size_t id);
    bool isStable(float coherence) const;
    bool isCollapsed(float coherence) const;
    bool isQuantum(float coherence) const;
    void updateConfig(const Config& new_config);

private:
    Config config_{};
    std::unique_ptr<QBSAProcessor> qbsa_processor_;
    std::unique_ptr<QuantumProcessorImpl> impl_;
};

std::unique_ptr<QuantumProcessor> createQuantumProcessor(const QuantumProcessor::Config& config);

} // namespace sep::quantum
