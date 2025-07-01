#include "compat/cuda.h"
#include "compat/math_common.h"
#include "quantum/types.h"
#include "quantum/qbsa_qfh.h"
#include "quantum/quantum_processor.h"
#include "quantum/quantum_processor_qfh.h"
#include "quantum/pattern_evolution_bridge.h"
#include "quantum/quantum_manifold_optimizer.h"
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>

namespace sep::quantum {

namespace {

float vectorCoherence(const glm::vec3& a, const glm::vec3& b) { // Remove unused parameter
    float dot = glm::dot(a, b);
    float mags = glm::length(a) * glm::length(b);
    if (mags <= 0.0f)
        return 0.0f;
    float cos_theta = std::clamp(dot / mags, -1.0f, 1.0f);
    return (cos_theta + 1.0f) * 0.5f;
}

float relationshipStrength(float ca, float cb, float interaction_frequency) {
    float coherence_similarity = 1.0f - std::abs(ca - cb);
    float freq = std::clamp(interaction_frequency, 0.0f, 1.0f);
    return std::clamp(coherence_similarity * freq, 0.0f, 1.0f);
}

float patternStability(float coherence, float historical_stability, float generation_count, float access_frequency) {
    float base = (coherence + historical_stability) * 0.5f;
    float freq = std::clamp(access_frequency, 0.0f, 1.0f);
    float gen = std::clamp(generation_count / 100.0f, 0.0f, 1.0f);
    return std::clamp(base * freq + gen, 0.0f, 1.0f);
}

}  // namespace

QuantumProcessorQFHCommon::QuantumProcessorQFHCommon() : qbsa_processor_(createQFHBasedQBSAProcessor({})) {}

const QFHResult& QuantumProcessorQFHCommon::getLastQFHResult() const {
    return m_last_qfh_result;
}

const QFHResult& QuantumProcessorQFHCommon::lastQFHResult() const {
    return m_last_qfh_result;
}

float QuantumProcessorQFHCommon::calculateMutationRate(float base_rate, int successful_mutations,
                                                       int stabilization_count) {
    float success_factor = 1.0f + static_cast<float>(successful_mutations) * 0.05f;
    float stability_factor = 1.0f / (1.0f + static_cast<float>(stabilization_count) * 0.1f);
    float rate = base_rate * success_factor * stability_factor;
    return std::clamp(rate, 0.0f, 1.0f);
}

float QuantumProcessorQFHCommon::processPattern(const glm::vec3& pattern) {
    auto normalized = glm::normalize(pattern);
    float coherence = 0.0f;

    for (const auto& existing : m_patterns) {
        float pattern_coherence = vectorCoherence(normalized, existing);
        coherence = std::max(coherence, pattern_coherence);
    }

    if (coherence >= 0.1f) {
        m_patterns.push_back(normalized);
    }

    if (!m_pattern_bits.empty()) {
        analyzePatternBits();
    }

    return coherence;
}

float QuantumProcessorQFHCommon::calculateStability(const glm::vec3& pattern, float historical_stability,
                                                    int generation_count, float access_frequency) {
    float coherence = processPattern(pattern);
    return patternStability(coherence, historical_stability, static_cast<float>(generation_count), access_frequency);
}

glm::vec3 QuantumProcessorQFHCommon::mutatePattern(const glm::vec3& pattern, float base_rate, int successful_mutations,
                                                   int stabilization_count) {
    float rate = calculateMutationRate(base_rate, successful_mutations, stabilization_count);

    glm::vec3 mutation = glm::vec3(rate * std::sin(pattern.x * 7.1f + pattern.y * 3.2f),
                                   rate * std::sin(pattern.y * 5.6f + pattern.z * 2.3f),
                                   rate * std::sin(pattern.z * 4.7f + pattern.x * 1.9f));

    return glm::normalize(pattern + mutation);
}

float QuantumProcessorQFHCommon::updateRelationship(const glm::vec3& pattern_a, const glm::vec3& pattern_b,
                                                    float interaction_frequency) {
    float coherence_a = processPattern(pattern_a);
    float coherence_b = processPattern(pattern_b);

    return relationshipStrength(coherence_a, coherence_b, interaction_frequency);
}

bool QuantumProcessorQFHCommon::isCollapsed(const glm::vec3& pattern) {
    float coherence = processPattern(pattern);
    bool traditional_collapse = coherence < sep::pattern::COLLAPSE_THRESHOLD;
    bool qfh_collapse = m_last_qfh_result.collapse_detected;
    return qfh_collapse || traditional_collapse;
}

bool QuantumProcessorQFHCommon::isStable(const glm::vec3& pattern) {
    float coherence = processPattern(pattern);
    bool traditional_stable = coherence >= sep::pattern::STABILITY_THRESHOLD;
    bool qfh_stable = m_last_qfh_result.rupture_ratio < 0.3f;
    return traditional_stable && qfh_stable;
}

bool QuantumProcessorQFHCommon::isQuantum(const glm::vec3& pattern) {
    float coherence = processPattern(pattern);
    bool traditional_quantum =
        coherence >= sep::pattern::MIN_COHERENCE &&
        coherence < sep::pattern::COLLAPSE_THRESHOLD;
    bool qfh_quantum = m_last_qfh_result.flip_ratio > 0.3f && m_last_qfh_result.rupture_ratio < 0.5f;
    return traditional_quantum || qfh_quantum;
}

void QuantumProcessorQFHCommon::processPatternBits(const std::vector<uint32_t>& pattern_bits) {
    m_pattern_bits = pattern_bits;
    analyzePatternBits(); 
}

void QuantumProcessorQFHCommon::analyzePatternBits() {
    if (m_pattern_bits.empty())
        return;

    QFHOptions options;
    options.collapse_threshold = 0.6f;
    QFHBasedProcessor qfh_processor(options);

    ::sep::shim::vector<uint32_t> shim_bits;
    shim_bits.reserve(m_pattern_bits.size());
    for (uint32_t v : m_pattern_bits) {
        shim_bits.push_back(v);
    }
    m_last_qfh_result = qfh_processor.analyze(QFHBasedProcessor::convertToBits(shim_bits));
}

}  // namespace sep::quantum
