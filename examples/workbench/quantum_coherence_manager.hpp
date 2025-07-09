#pragma once

#include <memory>
#include <vector>

namespace sep {
namespace workbench {

struct EvolutionResult;

class QuantumCoherenceManager {
public:
    QuantumCoherenceManager() = default;
    ~QuantumCoherenceManager() = default;

    void setCoherenceThreshold(float threshold);
    void updateCoherence(const EvolutionResult& result);
    float getCoherence() const;

private:
    float coherence_threshold_ = 0.5f;
    float current_coherence_ = 0.0f;
};

} // namespace workbench
} // namespace sep