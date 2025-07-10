#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <algorithm> // For std::clamp
#include "pattern.hpp" // For workbench::Pattern
#include "quantum/types.h" // For the canonical quantum::Pattern definition

namespace sep {
namespace quantum {

// Conversion functions between workbench::Pattern and quantum::Pattern
inline Pattern convert_from_workbench(const sep::workbench::Pattern& wp) {
    Pattern qp;
    qp.id = wp.id;
    qp.quantum_state.coherence = wp.quantum_state.coherence;
    qp.quantum_state.stability = wp.quantum_state.stability;
    qp.quantum_state.generation = wp.quantum_state.generation;
    qp.data = wp.values; // Copy values to data
    return qp;
}

inline sep::workbench::Pattern convert_to_workbench(const Pattern& qp) {
    sep::workbench::Pattern wp;
    wp.id = qp.id;
    wp.quantum_state.coherence = qp.quantum_state.coherence;
    wp.quantum_state.stability = qp.quantum_state.stability;
    wp.quantum_state.generation = qp.quantum_state.generation;
    wp.values = qp.data; // Copy data to values
    return wp;
}

// Add a convenience constructor to Pattern (via inheritance) for workbench compatibility
// This allows implicit conversion from workbench::Pattern to quantum::Pattern
class WorkbenchCompatiblePattern : public Pattern {
public:
    WorkbenchCompatiblePattern() = default;
    
    WorkbenchCompatiblePattern(const sep::workbench::Pattern& wp) {
        id = wp.id;
        quantum_state.coherence = wp.quantum_state.coherence;
        quantum_state.stability = wp.quantum_state.stability;
        quantum_state.generation = wp.quantum_state.generation;
        data = wp.values;
    }
    
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