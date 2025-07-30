#pragma once

#include <array>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <memory>
#include <string>
#include <vector>

#include "engine/internal/types.h"
#include "memory/types.h"
#include "quantum/types.h"

namespace sep::memory {

class QuantumCoherenceManager {
  public:
    struct Config {
        std::size_t max_patterns{1000};
        float anomaly_threshold{0.1f};
        bool enable_cuda{false};
    };

    enum class AnomalyType {
        ExcessiveCoherence,
        InsufficientCoherence,
        RapidChange
    };

    enum class MigrationReason {
        HighCoherence,
        HighStability,
        FrequentAccess,
        Entanglement,
        MemoryPressure,
        LowActivity
    };

    struct CoherenceAnomaly {
        std::string pattern_id;
        float coherence_value{0.f};
        float expected_value{0.f};
        float severity{0.f};
        AnomalyType type{AnomalyType::RapidChange};
    };

    struct TierMigration {
        std::string pattern_id;
        ::sep::memory::MemoryTierEnum from_tier{::sep::memory::MemoryTierEnum::STM};
        ::sep::memory::MemoryTierEnum to_tier{::sep::memory::MemoryTierEnum::STM};
        float coherence{0.f};
        MigrationReason reason{MigrationReason::LowActivity};
    };

    struct EntanglementNode {
        std::string pattern_id;
        float coherence{0.f};
        glm::vec4 position{0.f};
    };

    struct EntanglementEdge {
        std::size_t node1_idx{0};
        std::size_t node2_idx{0};
        float strength{0.f};
        float phase_correlation{0.f};
    };

    struct EntanglementGraph {
        std::vector<EntanglementNode> nodes;
        std::vector<EntanglementEdge> edges;
        float total_entanglement{0.f};
        std::uint32_t max_degree{0};
        float clustering_coefficient{0.f};
    };

    struct CoherenceMetrics {
        float global_coherence{0.f};
        float tier_coherence[3]{0.f, 0.f, 0.f};
        float tier_fragmentation[3]{0.f, 0.f, 0.f};
        std::uint64_t total_patterns{0};
        std::uint64_t coherent_patterns{0};
        std::uint64_t fragmented_patterns{0};
        float memory_pressure{0.f};
        float entanglement_density{0.f};
    };

    struct PatternCoherenceData {
        std::string pattern_id;
        float coherence{0.f};
        float stability{0.f};
        std::uint32_t access_count{0};
        std::uint64_t last_access_tick{0};
        ::sep::memory::MemoryTierEnum current_tier{::sep::memory::MemoryTierEnum::STM};
        std::vector<std::string> entangled_patterns;
        float fragmentation_score{0.f};
    };

    struct CoherenceResult {
        bool success{false};
        float global_coherence{0.f};
        float memory_pressure{0.f};
        std::size_t total_migrations{0};
        float tier_fragmentation[3]{0.f, 0.f, 0.f};
        std::uint32_t tier_pattern_count[3]{0, 0, 0};
        std::vector<CoherenceAnomaly> anomalies;
        std::vector<TierMigration> tier_migrations;
    };

    struct TierAnalysis {
        float tier_coherence[3]{0.f, 0.f, 0.f};
        std::uint32_t tier_pattern_count[3]{0, 0, 0};
        std::array<float, 3> optimal_distribution{};
    };

    struct CoherenceSnapshot {
        std::uint64_t timestamp{0};
        CoherenceMetrics global_metrics;
        std::vector<PatternCoherenceData> pattern_states;
        std::array<std::uint32_t, 3> tier_distribution{};
    };

    explicit QuantumCoherenceManager(const Config& config);
    ~QuantumCoherenceManager();

    CoherenceResult updateCoherence(const std::vector<sep::quantum::Pattern>& patterns);
    std::vector<TierMigration> optimizeMemoryLayout();
    EntanglementGraph computeEntanglementGraph(const std::vector<sep::quantum::Pattern>& patterns);
    void applyCoherenceDecay(float decay_factor);
    CoherenceSnapshot createSnapshot() const;
    bool restoreFromSnapshot(const CoherenceSnapshot& snapshot);

    // Metrics and diagnostics
    const CoherenceMetrics& getMetrics() const;
    uint64_t getGlobalTick() const;
    uint32_t getPatternCountByTier(MemoryTierEnum tier) const;
    float getTierFragmentation(MemoryTierEnum tier) const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

// Factory helper
std::unique_ptr<QuantumCoherenceManager> createQuantumCoherenceManager(const QuantumCoherenceManager::Config& config);

} // namespace sep::memory
