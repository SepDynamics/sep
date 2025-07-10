#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "pattern.hpp" // For workbench::Pattern

// Define a sentinel to avoid including this file when the real Pattern is already defined
#ifndef SEP_QUANTUM_PATTERN_ALREADY_DEFINED
#define SEP_QUANTUM_PATTERN_ALREADY_DEFINED

namespace sep {
namespace quantum {

// Simplified Pattern implementation for workbench compatibility
class Pattern {
public:
    struct QuantumState {
        float coherence = 0.5f;
        float stability = 0.5f;
        int generation = 1;
    };

    std::string id;
    QuantumState quantum_state;
    
    // Compatibility with quantum/types.h Pattern structure
    glm::vec4 position;
    std::vector<float> data;
};

} // namespace quantum
} // namespace sep

#endif // SEP_QUANTUM_PATTERN_ALREADY_DEFINED
    std::vector<float> values;

    // Conversion from workbench::Pattern
    Pattern(const sep::workbench::Pattern& wp) {
        id = wp.id;
        quantum_state.coherence = wp.quantum_state.coherence;
        quantum_state.stability = wp.quantum_state.stability;
        quantum_state.generation = wp.quantum_state.generation;
        values = wp.values;
    }

    // Default constructor
    Pattern() = default;
    
    // Simple evolution method to match workbench::Pattern
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

} // namespace quantum
} // namespace sep