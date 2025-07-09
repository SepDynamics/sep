#include "pattern_processor.hpp"
#include <algorithm>
#include <cmath>
#include "core/engine.h"

namespace sep {
namespace workbench {

PatternProcessor::PatternProcessor(sep::core::Engine* engine)
    : engine_(engine) {
    // Initialize with deterministic patterns for demo
    current_pattern_state_.resize(100);
    
    // Initialize with deterministic pattern based on position
    for (size_t i = 0; i < current_pattern_state_.size(); ++i) {
        float phase = (float)i / current_pattern_state_.size() * 2.0f * M_PI;
        current_pattern_state_[i] = glm::vec3(
            std::cos(phase),
            std::sin(phase),
            std::cos(phase * 2.0f)
        );
    }
}

PatternProcessor::~PatternProcessor() = default;

void PatternProcessor::initializeState(const QuantumState& state) {
    state_ = state;
    
    // Resize pattern state based on dimensions
    size_t pattern_size = static_cast<size_t>(state_.dimensions[0] * 
                                             state_.dimensions[1] * 
                                             state_.dimensions[2]);
    
    current_pattern_state_.resize(pattern_size);
    
    // Initialize with some deterministic patterns
    float scale = 2.0f * M_PI / static_cast<float>(pattern_size);
    for (size_t i = 0; i < pattern_size; ++i) {
        float phase = scale * static_cast<float>(i);
        current_pattern_state_[i] = glm::vec3(
            std::cos(phase),
            std::sin(phase),
            std::cos(phase * 2.0f)
        );
    }
}

EvolutionResult PatternProcessor::evolvePatterns(float dt) {
    EvolutionResult result;
    
    // Use evolution rate from state
    float evolution_scale = dt * state_.evolution_rate;
    
    // Calculate previous coherence
    float previous_coherence = calculateCoherence();
    
    // Deterministic evolution approach
    for (auto& vec : current_pattern_state_) {
        // Apply a small phase shift based on evolution_scale
        float angle = evolution_scale * 0.1f;
        float cos_angle = std::cos(angle);
        float sin_angle = std::sin(angle);
        
        // Rotate the vector slightly
        float new_x = vec.x * cos_angle - vec.y * sin_angle;
        float new_y = vec.x * sin_angle + vec.y * cos_angle;
        vec.x = new_x;
        vec.y = new_y;
        
        // Normalize to keep vectors on unit sphere
        float length = glm::length(vec);
        if (length > 0.001f) {
            vec /= length;
        }
    }
    
    // Calculate new coherence
    float new_coherence = calculateCoherence();
    
    result.overall_coherence = new_coherence;
    result.coherence_delta = new_coherence - previous_coherence;
    result.pattern_count = current_pattern_state_.size();
    
    return result;
}

std::vector<glm::vec3> PatternProcessor::getCurrentState() const {
    return current_pattern_state_;
}

float PatternProcessor::calculateCoherence() const {
    // A simple measure of coherence based on average dot product between vectors
    if (current_pattern_state_.size() <= 1) {
        return 1.0f;
    }
    
    float total_dot = 0.0f;
    size_t count = 0;
    
    // Sample pairs of vectors
    const size_t max_samples = 1000;
    size_t n = current_pattern_state_.size();
    size_t stride = std::max(size_t(1), n / 50);
    
    for (size_t i = 0; i < n; i += stride) {
        for (size_t j = i + stride; j < n; j += stride) {
            if (count >= max_samples) break;
            
            float dot = std::abs(glm::dot(
                glm::normalize(current_pattern_state_[i]),
                glm::normalize(current_pattern_state_[j])
            ));
            
            total_dot += dot;
            count++;
        }
        
        if (count >= max_samples) break;
    }
    
    return (count > 0) ? (0.5f + 0.5f * (total_dot / static_cast<float>(count))) : 1.0f;
}

} // namespace workbench
} // namespace sep