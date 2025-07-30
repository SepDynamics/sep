#include "quantum/quantum_processor.h"

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "engine/internal/math_common.h"
#include "engine/internal/types.h"
#include "quantum/qbsa_qfh.h"
#include "quantum/quantum_processor_cuda.h"
#include "quantum/quantum_processor_qfh.h"

namespace sep::quantum {

// QuantumProcessorError implementation
QuantumProcessorError::QuantumProcessorError(const std::string& message)
    : std::runtime_error("QuantumProcessor: " + message)
{
}

// Internal implementation class to maintain existing functionality
class QuantumProcessorImpl : public QuantumProcessorQFHCommon {
public:
    QuantumProcessorImpl() = default;

    bool processPatternWithId(const glm::vec3& pattern_data, size_t pattern_id) {
        if (glm::length(pattern_data) < 1e-6f) {
            return false;
        }
        float coherence = processPattern(pattern_data);
        pattern_coherence_map_[pattern_id] = coherence;
        pattern_data_map_[pattern_id] = pattern_data;
        return coherence >= 0.1f;
    }

    bool updatePatternWithId(size_t pattern_id, const glm::vec3& new_data) {
        if (glm::length(new_data) < 1e-6f) {
            return false;
        }
        float coherence = processPattern(new_data);
        pattern_coherence_map_[pattern_id] = coherence;
        pattern_data_map_[pattern_id] = new_data;
        return true;
    }

    void removePatternWithId(size_t pattern_id) {
        pattern_coherence_map_.erase(pattern_id);
        pattern_data_map_.erase(pattern_id);
    }

private:
    std::unordered_map<size_t, float> pattern_coherence_map_;
    std::unordered_map<size_t, glm::vec3> pattern_data_map_;
};

QuantumProcessor::~QuantumProcessor() = default;

QuantumProcessor::QuantumProcessor(const Config& config)
    : Processor(static_cast<ProcessingConfig>(config)), config_(config) {
    impl_ = std::make_unique<QuantumProcessorImpl>();
    qbsa_processor_ = createQFHBasedQBSAProcessor({});
}

float QuantumProcessor::calculateCoherence(const glm::vec3& pattern_a, const glm::vec3& pattern_b) {
    // Calculate dot product normalized by magnitudes
    float dot_product = glm::dot(pattern_a, pattern_b);
    float mag_a = glm::length(pattern_a);
    float mag_b = glm::length(pattern_b);
    
    // Avoid division by zero
    if (mag_a < 1e-6f || mag_b < 1e-6f) {
        return 0.0f;
    }
    
    // Return normalized coherence value between 0 and 1
    return glm::clamp(dot_product / (mag_a * mag_b), 0.0f, 1.0f);
}

float QuantumProcessor::calculateStability(float coherence, float historical_stability,
                                          float generation_count, float access_frequency) {
    // Weight factors for stability calculation
    const float coherence_weight = 0.4f;
    const float history_weight = 0.3f;
    const float generation_weight = 0.2f;
    const float access_weight = 0.1f;
    
    // Calculate stability score
    float stability = coherence_weight * coherence +
                      history_weight * historical_stability +
                      generation_weight * (1.0f / (1.0f + generation_count * 0.01f)) +
                      access_weight * access_frequency;
    
    return glm::clamp(stability, 0.0f, 1.0f);
}

bool QuantumProcessor::processPattern(const glm::vec3& pattern_data, size_t pattern_id) {
    if (!impl_) {
        return false;
    }
    return impl_->processPatternWithId(pattern_data, pattern_id);
}

bool QuantumProcessor::updatePattern(size_t pattern_id, const glm::vec3& new_data) {
    if (!impl_) {
        return false;
    }
    return impl_->updatePatternWithId(pattern_id, new_data);
}

void QuantumProcessor::removePattern(size_t pattern_id) {
    if (!impl_) {
        return;
    }
    impl_->removePatternWithId(pattern_id);
}

bool QuantumProcessor::isStable(float coherence) const {
    return coherence >= config_.measurement_threshold;
}

namespace {
constexpr float kCollapseThreshold = 0.3f;
} // namespace

bool QuantumProcessor::isCollapsed(float coherence) const {
    return coherence < kCollapseThreshold;
}

bool QuantumProcessor::isQuantum(float coherence) const {
    return coherence >= config_.decoherence_rate &&
           coherence < config_.measurement_threshold;
}

void QuantumProcessor::updateConfig(const Config& new_config) {
    config_ = new_config;
    impl_ = std::make_unique<QuantumProcessorImpl>();
}


QuantumProcessor::Config::operator ProcessingConfig() const {
    ProcessingConfig pc;
    pc.max_patterns = max_qubits;
    pc.mutation_rate = decoherence_rate;
    pc.ltm_coherence_threshold = measurement_threshold;
    pc.enable_cuda = enable_gpu;
    return pc;
}

std::unique_ptr<QuantumProcessor> createQuantumProcessor(
    const QuantumProcessor::Config& config) {
    return std::make_unique<QuantumProcessor>(config);
}
} // namespace sep::quantum
