#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace sep {
namespace workbench {

// Simple Pattern representation for the workbench demo
struct Pattern {
    struct QuantumState {
        float coherence = 0.5f;
        float stability = 0.5f;
        int generation = 1;
    };

    std::string id;
    QuantumState quantum_state;
    std::vector<float> values;

    // Simple evolution method
    void evolve(float rate) {
        // Simple evolution simulation
        quantum_state.coherence = std::clamp(
            quantum_state.coherence + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
            0.0f, 1.0f);

        quantum_state.stability = std::clamp(
            quantum_state.stability + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
            0.0f, 1.0f);

        quantum_state.generation++;
    }
};

} // namespace workbench
} // namespace sep