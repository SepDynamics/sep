#pragma once

#include "context/processor.h"
#include "quantum/types.h"
#include "core/types.h"
#include "memory/memory_tier.h"
#include <vector>
#include <memory>
#include <string>

namespace sep::quantum {

// Forward declarations
class GPUContext;
namespace core { class SystemHooks; }

// Constants for pattern processing
namespace constants {
constexpr float RELATIONSHIP_THRESHOLD = 0.6F;
constexpr float LTM_COHERENCE_THRESHOLD = 0.9F;
constexpr float MTM_COHERENCE_THRESHOLD = 0.7F;
} // namespace constants

// Pattern processing result
struct PatternProcessResult {
    QuantumState state;
    std::string pattern_id;
    MemoryTierEnum memory_tier{MemoryTierEnum::STM};
    bool tier_changed{false};
    float coherence_score{0.0F};
    float stability_score{0.0F};
};

// Pattern quantum processor interface
class PatternQuantumProcessor {
public:
    virtual ~PatternQuantumProcessor() = default;

    [[nodiscard]] virtual PatternProcessResult processPattern(
        const QuantumState& state,
        const std::string& pattern_id = "") = 0;

    [[nodiscard]] virtual std::vector<PatternProcessResult> processBatch(
        const std::vector<QuantumState>& states,
        const std::vector<std::string>& pattern_ids) = 0;

    [[nodiscard]] virtual float calculateCoherence(
        const QuantumState& state_a,
        const QuantumState& state_b) const = 0;

    [[nodiscard]] virtual bool isStable(const QuantumState& state) const = 0;
    [[nodiscard]] virtual bool isCollapsed(const QuantumState& state) const = 0;
    [[nodiscard]] virtual bool isQuantum(const QuantumState& state) const = 0;
};

// Factory function - updated to use ProcessingConfig
[[nodiscard]] std::unique_ptr<PatternQuantumProcessor> createPatternQuantumProcessor(
    const ProcessingConfig& config = ProcessingConfig{}
);

} // namespace sep::quantum
