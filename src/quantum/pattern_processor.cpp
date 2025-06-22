#include "quantum/pattern_processor.h"
#include "quantum/types.h"
#include "quantum/quantum_processor.h"

#include <algorithm>
#include <memory>

namespace sep::quantum {

namespace {
class PatternQuantumProcessorImpl final : public PatternQuantumProcessor {
public:
    explicit PatternQuantumProcessorImpl(const QuantumProcessor::Config& config)
        : quantum_processor_(createQuantumProcessor(config)) {}

    PatternProcessResult processPattern(
        const QuantumState& state,
        const std::string& pattern_id) override {
        PatternProcessResult result;
        result.state = state;
        result.pattern_id = pattern_id;
        
        // Process using quantum processor
        quantum_processor_->evolveState(result.state, pattern_id);
        result.coherence_score = result.state.coherence;
        result.stability_score = result.state.stability;

        // Determine memory tier
        if (result.coherence_score >= constants::LTM_COHERENCE_THRESHOLD &&
            result.stability_score >= constants::STABILITY_THRESHOLD) {
            result.memory_tier = MemoryTierEnum::LTM;
        } else if (result.coherence_score >= constants::MTM_COHERENCE_THRESHOLD) {
            result.memory_tier = MemoryTierEnum::MTM;
        }

        return result;
    }

    std::vector<PatternProcessResult> processBatch(
        const std::vector<QuantumState>& states,
        const std::vector<std::string>& pattern_ids) override {
        std::vector<PatternProcessResult> results;
        results.reserve(states.size());
        
        for (size_t i = 0; i < states.size(); ++i) {
            results.push_back(processPattern(states[i], pattern_ids[i]));
        }
        
        return results;
    }

    float calculateCoherence(
        const QuantumState& state_a,
        const QuantumState& state_b) const override {
        return quantum_processor_->calculateCoherence(state_a.amplitudes, state_b.amplitudes);
    }

    bool isStable(const QuantumState& state) const override {
        return state.stability >= constants::STABILITY_THRESHOLD;
    }

    bool isCollapsed(const QuantumState& state) const override {
        return state.coherence < constants::MIN_COHERENCE;
    }

    bool isQuantum(const QuantumState& state) const override {
        return state.coherence >= constants::MTM_COHERENCE_THRESHOLD;
    }

private:
    std::unique_ptr<QuantumProcessor> quantum_processor_;
};
} // namespace

std::unique_ptr<PatternQuantumProcessor> createPatternQuantumProcessor(
    const QuantumProcessor::Config& config) {
    return std::make_unique<PatternQuantumProcessorImpl>(config);
}

} // namespace sep::quantum
