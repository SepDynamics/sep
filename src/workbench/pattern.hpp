#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace sep {
namespace workbench {

// Enhanced Pattern representation for the workbench demo
struct Pattern {
    struct QuantumState {
        float coherence = 0.5f;       // Quantum coherence (0.0-1.0)
        float stability = 0.5f;       // Pattern stability (0.0-1.0)
        float energy = 1.0f;          // Energy level
        float coupling = 0.5f;        // Coupling strength with other patterns
        int generation = 1;           // Evolution generation counter
        float phase = 0.0f;           // Quantum phase (0.0-2π)
        float entanglement = 0.0f;    // Entanglement measure (0.0-1.0)
    };

    // Core pattern properties
    std::string id;                   // Unique identifier
    QuantumState quantum_state;       // Quantum properties
    std::vector<float> values;        // Pattern vector values (dimensions/features)
    
    // Visualization properties
    glm::vec3 position{0.0f};         // 3D position for visualization
    glm::vec3 velocity{0.0f};         // Velocity for animation
    glm::vec3 color{1.0f, 1.0f, 1.0f};// RGB color for rendering
    float scale = 1.0f;               // Size scaling factor
    int memory_tier = 0;              // Memory tier (0=STM, 1=MTM, 2=LTM)
    
    // Relationship properties
    std::vector<std::string> related_patterns; // IDs of related patterns
    std::vector<float> relationship_strengths; // Strength of each relationship

    // Simple evolution method
    void evolve(float rate) {
        // Update quantum state
        quantum_state.coherence = std::clamp(
            quantum_state.coherence + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
            0.0f, 1.0f);

        quantum_state.stability = std::clamp(
            quantum_state.stability + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
            0.0f, 1.0f);
            
        quantum_state.energy = std::clamp(
            quantum_state.energy + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
            0.1f, 2.0f);
            
        quantum_state.phase += ((float)rand() / RAND_MAX) * 0.1f * rate;
        if (quantum_state.phase > 6.28318f) quantum_state.phase -= 6.28318f;
        
        quantum_state.entanglement = std::clamp(
            quantum_state.entanglement + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
            0.0f, 1.0f);

        quantum_state.generation++;
        
        // Update visualization properties based on quantum state
        updateVisualization();
    }
    
    // Update visualization properties based on quantum state
    void updateVisualization() {
        // Map coherence to color brightness
        float brightness = 0.5f + 0.5f * quantum_state.coherence;
        
        // Create a color based on stability (red-blue spectrum)
        color.r = brightness * (1.0f - quantum_state.stability);
        color.g = brightness * quantum_state.phase / 6.28318f;
        color.b = brightness * quantum_state.stability;
        
        // Scale based on energy
        scale = 0.5f + quantum_state.energy * 0.5f;
        
        // Memory tier affects vertical position
        position.y = -1.0f + memory_tier * 1.0f;
    }
    
    // Convert from SEP engine pattern format
    static Pattern fromEnginePattern(const std::string& id, float coherence, float stability,
                                     int memory_tier, const glm::vec3& position,
                                     float energy = 1.0f, float coupling = 0.5f) {
        Pattern pattern;
        pattern.id = id;
        pattern.quantum_state.coherence = coherence;
        pattern.quantum_state.stability = stability;
        pattern.memory_tier = memory_tier;
        pattern.position = position;
        
        // Set other properties
        pattern.quantum_state.energy = energy;
        pattern.quantum_state.coupling = coupling;
        
        // Update visualization based on the new properties
        pattern.updateVisualization();
        
        return pattern;
    }
};

} // namespace workbench
} // namespace sep