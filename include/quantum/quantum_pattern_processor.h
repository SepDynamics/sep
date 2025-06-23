#pragma once

#include "core/system_hooks.h"
#include "core/types.h"
#include "memory/memory_tier.hpp"
#include "quantum/pattern_evolution.h"
#include "quantum/quantum_processor.h"
#include "quantum/types.h"
#include "quantum/gpu_context.h"
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

}  // namespace pattern
}  // namespace sep
