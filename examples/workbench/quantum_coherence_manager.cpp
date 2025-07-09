#include "quantum_coherence_manager.hpp"
#include "pattern_processor.hpp"

namespace sep {
namespace workbench {

void QuantumCoherenceManager::setCoherenceThreshold(float threshold) {
    coherence_threshold_ = threshold;
}

void QuantumCoherenceManager::updateCoherence(const EvolutionResult& result) {
    current_coherence_ = result.overall_coherence;
}

float QuantumCoherenceManager::getCoherence() const {
    return current_coherence_;
}

} // namespace workbench
} // namespace sep