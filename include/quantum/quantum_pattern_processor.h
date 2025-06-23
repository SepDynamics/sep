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

// Pattern processing result
struct PatternProcessResult {
    quantum::QuantumState state;
    std::string pattern_id;
    MemoryTierEnum memory_tier{MemoryTierEnum::STM};
    bool tier_changed{false};
    float coherence_score{0.0F};
    float stability_score{0.0F};
};

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

    SEPResult init(quantum::GPUContext* ctx) override;
    void evolvePatterns() override;
    PatternData mutatePattern(const PatternData& parent) override;
    std::vector<PatternProcessResult> processBatch(const std::vector<quantum::QuantumState>& states,
                                                    const std::vector<std::string>& pattern_ids);

private:
    PatternProcessorConfig config_;
    std::unique_ptr<quantum::QuantumProcessor> quantum_processor_;
    std::vector<PatternData>& getModifiablePatterns() {
        return patterns_;
    }

    // Helper methods
    quantum::QuantumState patternToQuantumState(const PatternData& pattern) const;
    void updatePatternFromQuantumState(PatternData& pattern, const quantum::QuantumState& state);
};

}  // namespace pattern
}  // namespace sep
