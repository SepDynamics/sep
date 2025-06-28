#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <glm/vec4.hpp>

#include "quantum/types.h"
#include "memory/types.h"

namespace sep::memory {

class QuantumCoherenceManager {
public:
    struct Config {
        bool enable_cuda = false;
        std::size_t max_patterns = 10000;
        float anomaly_threshold = 0.1f;
    };

    struct PatternCoherenceData {
        std::string pattern_id;
        float coherence = 0.0f;
        float stability = 0.0f;
        uint32_t access_count = 0;
        uint64_t last_access_tick = 0;
        MemoryTierEnum current_tier = MemoryTierEnum::STM;
        std::vector<std::string> entangled_patterns;
    };

    struct CoherenceMetrics {
        float global_coherence = 0.0f;
        float tier_coherence[3] = {0.f, 0.f, 0.f};
        uint64_t total_patterns = 0;
        uint64_t coherent_patterns = 0;
        float memory_pressure = 0.0f;
        float entanglement_density = 0.0f;
    };

    enum class MigrationReason {
        HighCoherence,
        HighStability,
        FrequentAccess,
        Entanglement,
        MemoryPressure,
        LowActivity
    };

    struct TierMigration {
        std::string pattern_id;
        MemoryTierEnum from_tier = MemoryTierEnum::STM;
        MemoryTierEnum to_tier = MemoryTierEnum::STM;
        float coherence = 0.0f;
        MigrationReason reason = MigrationReason::LowActivity;
    };

    enum class AnomalyType { ExcessiveCoherence, InsufficientCoherence, RapidChange };

    struct CoherenceAnomaly {
        std::string pattern_id;
        float coherence_value = 0.0f;
        float expected_value = 0.0f;
        float severity = 0.0f;
        AnomalyType type = AnomalyType::RapidChange;
    };

    struct CoherenceResult {
        bool success = false;
        float global_coherence = 0.0f;
        float memory_pressure = 0.0f;
        uint32_t total_migrations = 0;
        std::vector<TierMigration> tier_migrations;
        std::vector<CoherenceAnomaly> anomalies;
    };

    struct EntanglementNode {
        std::string pattern_id;
        float coherence = 0.0f;
        glm::vec4 position{};
    };

    struct EntanglementEdge {
        std::size_t node1_idx = 0;
        std::size_t node2_idx = 0;
        float strength = 0.0f;
        float phase_correlation = 0.0f;
    };

    struct EntanglementGraph {
        std::vector<EntanglementNode> nodes;
        std::vector<EntanglementEdge> edges;
        float total_entanglement = 0.0f;
        uint32_t max_degree = 0;
        float clustering_coefficient = 0.0f;
    };

    struct CoherenceSnapshot {
        uint64_t timestamp = 0;
        CoherenceMetrics global_metrics;
        std::vector<PatternCoherenceData> pattern_states;
        std::array<uint32_t,3> tier_distribution{};
    };

    explicit QuantumCoherenceManager(const Config &config);
    ~QuantumCoherenceManager();

    CoherenceResult updateCoherence(const std::vector<quantum::Pattern> &patterns);
    std::vector<TierMigration> optimizeMemoryLayout();
    EntanglementGraph computeEntanglementGraph(const std::vector<quantum::Pattern> &patterns);
    void applyCoherenceDecay(float decay_factor);
    CoherenceSnapshot createSnapshot() const;
    bool restoreFromSnapshot(const CoherenceSnapshot &snapshot);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace sep::memory
