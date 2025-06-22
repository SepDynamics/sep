#pragma once

#include "core/system_hooks.h"
#include "core/types.h"
#include "memory/memory_tier.h"
#include "pattern/pattern_evolution.h"
#include "quantum/quantum_processor.h"
#include "quantum/types.h"
#include <string>
#include <vector>

namespace sep {
namespace pattern {

struct PatternProcessorConfig {
    float minimum_coherence{0.1f};
    float stability_threshold{0.85f};
    float coherence_threshold{0.7f};
    float demotion_threshold{0.3f};
    bool enable_quantum_effects{true};
};

class QuantumPatternProcessor : public CPUPatternProcessor {
public:
    explicit QuantumPatternProcessor(const PatternProcessorConfig& config);
    ~QuantumPatternProcessor() override = default;

    SEPResult init(GPUContext* ctx) override;
    void evolvePatterns() override;
    PatternData mutatePattern(const PatternData& parent) override;
    std::vector<PatternProcessResult> processBatch(const std::vector<QuantumState>& states,
                                                   const std::vector<std::string>& pattern_ids) override;

private:
    PatternProcessorConfig config_;
    std::vector<PatternData>& getModifiablePatterns() {
        return patterns_;
    }

    // Helper methods
    QuantumState patternToQuantumState(const PatternData& pattern) const;
    void updatePatternFromQuantumState(PatternData& pattern, const QuantumState& state);
};

inline QuantumState QuantumPatternProcessor::patternToQuantumState(const PatternData& pattern) const {
    QuantumState state{};
    state.coherence = pattern.coherence;
    state.stability = pattern.stability;
    state.entropy = pattern.entropy;
    state.generation = pattern.generation;
    state.access_frequency = static_cast<float>(pattern.mutations) / (pattern.generation > 0 ? pattern.generation : 1);
    return state;
}

inline void QuantumPatternProcessor::updatePatternFromQuantumState(PatternData& pattern, const QuantumState& state) {
    pattern.coherence = state.coherence;
    pattern.stability = state.stability;
    pattern.entropy = state.entropy;
    pattern.generation = state.generation;
    pattern.mutations = static_cast<uint32_t>(state.access_frequency * state.generation);
}

inline QuantumPatternProcessor::QuantumPatternProcessor(const PatternProcessorConfig& config) : config_(config) {}

inline SEPResult QuantumPatternProcessor::init(GPUContext* ctx) {
    return CPUPatternProcessor::init(ctx);
}

}  // namespace pattern
}  // namespace sep
