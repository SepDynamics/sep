#ifndef SEP_QUANTUM_PROCESSOR_QFH_H
#define SEP_QUANTUM_PROCESSOR_QFH_H

#include <cstdint>
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <vector>

#include "engine/internal/types.h"
#include "memory/types.h"
#include "quantum/bitspace/qbsa.h"
#include "quantum/bitspace/qfh.h"

namespace sep::quantum {

/**
 * QFH-enhanced Quantum Processor
 *
 * Integrates the Quantum Fourier Hierarchy (QFH) approach from the testbed
 * project into the main SEP codebase.
 */
class QuantumProcessorQFHCommon {
public:
    QuantumProcessorQFHCommon();

    void clear();
    float calculateMutationRate(float base_rate, int successful_mutations, int stabilization_count);
    float processPattern(const glm::vec3& pattern);

    const QFHResult& getLastQFHResult() const;

    float calculateStability(const glm::vec3& pattern, float historical_stability, int generation_count,
                             float access_frequency);

    glm::vec3 mutatePattern(const glm::vec3& pattern, float base_rate, int successful_mutations,
                            int stabilization_count);

    float updateRelationship(const glm::vec3& pattern_a, const glm::vec3& pattern_b, float interaction_frequency);

    bool isCollapsed(const glm::vec3& pattern);
    bool isStable(const glm::vec3& pattern);
    bool isQuantum(const glm::vec3& pattern);

    void processPatternBits(const std::vector<uint32_t>& pattern_bits);

protected:
    std::vector<glm::vec3> m_patterns;
    std::vector<uint32_t> m_pattern_bits;
    std::unique_ptr<QBSAProcessor> qbsa_processor_;
    QFHResult m_last_qfh_result;

    const QFHResult& lastQFHResult() const;

private:
    void analyzePatternBits();
    mutable std::mutex m_mutex;
};

/**
 * Concrete processor interface used by the rest of the codebase.
 */
class QuantumProcessorQFH : public QuantumProcessorQFHCommon {
public:

    const QFHResult& getLastQFHResult() const;

    ::sep::memory::MemoryTierEnum determineMemoryTier(float coherence, float stability, uint32_t generation_count) const;
};

}  // namespace sep::quantum

#endif  // SEP_QUANTUM_PROCESSOR_QFH_H
