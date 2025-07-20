#pragma once

#include <atomic>
#include <future>
#include <glm/glm.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "engine/types.h"
#include "quantum/types.h"

namespace sep::quantum {

enum class QuantumPhase {
    Coherent,
    Superposition,
    Entangled
};

struct PhaseTransition {
    shim::string pattern_id;
    QuantumPhase from_phase;
    QuantumPhase to_phase;
    float transition_energy;
};

struct EntanglementPair {
    shim::string pattern_id1;
    shim::string pattern_id2;
    float strength;
    float phase_correlation;
};

struct CollapseEvent {
    bool detected;
    glm::vec4 collapse_center;
    float affected_radius;
    float severity;
    shim::vector<shim::string> collapsed_pattern_ids;
};

struct EvolutionResult {
    shim::vector<Pattern> evolved_patterns;
    shim::vector<PhaseTransition> phase_transitions;
    float total_coherence;
    float entropy_change;
    float stability_metric;
};

class PatternEvolutionBridge {
public:
    struct Config {
        float entanglement_threshold{0.5f};
        float collapse_variance_threshold{0.3f};
        float environment_coupling{0.01f};
        float target_coherence{0.8f};
        float target_stability{0.7f};
        float convergence_threshold{0.001f};
        float evolution_step_size{0.05f};
        float interaction_radius{2.0f};
        float coupling_strength{0.42f};  // Added for quantum state coupling
        size_t num_threads{4};
    };

    explicit PatternEvolutionBridge(const Config& config);
    ~PatternEvolutionBridge();

    EvolutionResult evolvePatterns(shim::vector<Pattern>& patterns, float time_step);
    shim::vector<EntanglementPair> computeEntanglements(const shim::vector<Pattern>& patterns);
    CollapseEvent detectCollapse(const shim::vector<Pattern>& patterns);

    void initializeEvolutionState();
    void updatePatterns(shim::vector<Pattern>& patterns);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sep::quantum
