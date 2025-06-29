#pragma once

#include "quantum/types.h"
#include <string>
#include <vector>
#include <memory>
#include <future>
#include <mutex>
#include <thread>
#include <atomic>
#include <glm/glm.hpp>

namespace sep::quantum {

// Forward declarations
class QuantumManifoldOptimizer;

enum class QuantumPhase {
    Coherent,
    Superposition,
    Entangled
};

struct PhaseTransition {
    std::string pattern_id;
    QuantumPhase from_phase;
    QuantumPhase to_phase;
    float transition_energy;
};

struct EntanglementPair {
    std::string pattern_id1;
    std::string pattern_id2;
    float strength;
    float phase_correlation;
};

struct CollapseEvent {
    bool detected;
    glm::vec4 collapse_center;
    float affected_radius;
    float severity;
    std::vector<std::string> collapsed_pattern_ids;
};

struct EvolutionResult {
    std::vector<sep::quantum::Pattern> evolved_patterns;
    std::vector<PhaseTransition> phase_transitions;
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

    EvolutionResult evolvePatterns(std::vector<sep::quantum::Pattern>& patterns, float time_step);
    std::vector<EntanglementPair> computeEntanglements(const std::vector<sep::quantum::Pattern>& patterns);
    CollapseEvent detectCollapse(const std::vector<sep::quantum::Pattern>& patterns);
    
    // Added based on implementation file
    void initializeEvolutionState();
    void updatePatterns(const std::vector<sep::quantum::Pattern>& patterns);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sep::quantum
