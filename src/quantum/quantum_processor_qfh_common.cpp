#include "compat/cuda.h"
#include "compat/math_common.h"
#include "core/types.h"
#include "quantum/qbsa_qfh.h"
#include "quantum/quantum_processor.h"
#include "quantum/quantum_processor_qfh.h"
#include "quantum/pattern_evolution_bridge.h"
#include "quantum/quantum_manifold_optimizer.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

namespace sep::quantum {

namespace {

float vectorCoherence(const glm::vec3& a, const glm::vec3& b) {
    // Use Euclidean distance, normalized to a [0, 1] range.
    // A smaller distance means higher coherence.
    float distance = glm::distance(a, b);
    
    // The coherence is inversely proportional to the distance.
    // The '+ 1.0f' prevents division by zero and ensures the result is <= 1.0.
    return 1.0f / (1.0f + distance);
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

void QuantumProcessorQFHCommon::clear() {
    m_patterns.clear();
    m_pattern_bits.clear();
    m_last_qfh_result = QFHResult();
}

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
    float coherence;

    if (m_patterns.empty()) {
        // The first pattern is perfectly coherent with itself.
        coherence = 1.0f;
    } else {
        if (m_patterns.back() == pattern) {
            return m_last_qfh_result.coherence;
        }
        // Calculate the average coherence with all existing patterns.
        float total_coherence = 0.0f;
        for (const auto& existing : m_patterns) {
            total_coherence += vectorCoherence(pattern, existing);
        }
        coherence = total_coherence / m_patterns.size();
    }

    // Store the original, non-normalized pattern for future comparisons.
    m_patterns.push_back(pattern);
    
    // PERFORMANCE FIX: Limit the pattern history to prevent O(N^2) slowdown.
    // This acts as a sliding window, keeping performance high for large files.
    const size_t MAX_PATTERN_HISTORY = 1024;
    if (m_patterns.size() > MAX_PATTERN_HISTORY) {
        m_patterns.erase(m_patterns.begin());
    }

    if (!m_pattern_bits.empty()) {
        analyzePatternBits();
        coherence = 0.7f * coherence + 0.3f * (1.0f - m_last_qfh_result.rupture_ratio);
    }
    m_last_qfh_result.coherence = coherence;
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

    std::vector<uint32_t> shim_bits;
    shim_bits.reserve(m_pattern_bits.size());
    for (uint32_t v : m_pattern_bits) {
        shim_bits.push_back(v);
    }
    m_last_qfh_result = qfh_processor.analyze(QFHBasedProcessor::convertToBits(shim_bits));

    // Add entropy calculation (Shannon on bits)
    if (shim_bits.empty()) {
        m_last_qfh_result.entropy = 0.0f;
        return;
    }
    auto bits = QFHBasedProcessor::convertToBits(shim_bits);
    if (bits.empty()) {
        m_last_qfh_result.entropy = 0.0f;
        return;
    }
    float p1 = static_cast<float>(std::count(bits.begin(), bits.end(), 1)) / bits.size();
    float p0 = 1.0f - p1;
    
    float entropy = 0.0f;
    if (p0 > 0) {
        entropy -= p0 * std::log2(p0 + 1e-6f);
    }
    if (p1 > 0) {
        entropy -= p1 * std::log2(p1 + 1e-6f);
    }
    
    m_last_qfh_result.entropy = std::clamp(entropy, 0.0f, 1.0f);
}

}  // namespace sep::quantum
