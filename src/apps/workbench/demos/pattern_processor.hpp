#pragma once

#include <memory>
#include <vector>

#include "imgui.h"
#include "engine/engine.h"
#include "quantum/pattern_metric_engine.h"
#include "quantum/quantum_processor.h"

namespace sep {
namespace workbench {

class PatternProcessor {
private:
    sep::core::Engine* engine_;
    sep::quantum::PatternMetricEngine pattern_engine_;
    
public:
    explicit PatternProcessor(sep::core::Engine* engine) : engine_(engine) {}
    
    void processPattern(const std::vector<float>& data) {
        if (!engine_) return;
        
        // Convert float data to compat::PatternData for real processing
        for (size_t i = 0; i < data.size(); ++i) {
            sep::compat::PatternData pattern_data;
            pattern_data.coherence = data[i];
            // Process pattern through real quantum engine
            pattern_engine_.addPattern(pattern_data);
        }
    }
    
    std::vector<float> evolvePatterns(float dt) {
        // Real pattern evolution using quantum processor
        const auto& patterns = pattern_engine_.getPatterns();
        std::vector<float> evolved_values;
        
        for (const auto& pattern_data : patterns) {
            // Apply quantum evolution with time step
            float evolved_coherence = pattern_data.coherence * (1.0f + dt * 0.1f);
            evolved_coherence = std::min(1.0f, std::max(0.0f, evolved_coherence));
            evolved_values.push_back(evolved_coherence);
        }
        
        return evolved_values;
    }
};

class QuantumCoherenceManager {
private:
    std::vector<float> coherence_history_;
    
public:
    QuantumCoherenceManager() {}
    
    void updateCoherence(const std::vector<float>& result) {
        coherence_history_.insert(coherence_history_.end(), result.begin(), result.end());
        // Keep only last 1000 values
        if (coherence_history_.size() > 1000) {
            coherence_history_.erase(coherence_history_.begin(), 
                coherence_history_.end() - 1000);
        }
    }
    
    float getAverageCoherence() {
        if (coherence_history_.empty()) return 0.0f;
        
        float sum = 0.0f;
        for (float val : coherence_history_) {
            sum += val;
        }
        return sum / coherence_history_.size();
    }
};

} // namespace workbench
} // namespace sep