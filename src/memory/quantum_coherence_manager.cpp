#include "memory/quantum_coherence_manager.h"

#include "engine/internal/core.h"
#include "engine/internal/cuda_helpers.h"
#include "engine/internal/cuda_sep.h"
#include "engine/internal/memory.h"
#include "engine/internal/types.h"
#include "memory/memory_tier_manager.hpp"
#include "memory/types.h"
#include "quantum/pattern_evolution_bridge.h"
#include "quantum/quantum_manifold_optimizer.h"
#include "quantum/quantum_processor_qfh.h"

using sep::config::CudaConfig;
using sep::memory::MemoryTierEnum;
#include <tbb/concurrent_hash_map.h>
#include <tbb/parallel_for.h>

#include <glm/vec4.hpp>

#include "engine/internal/logging.h"
#include "engine/internal/standard_includes.h"

namespace sep::memory {

using ::sep::memory::MemoryTierEnum;

namespace {
    // Memory coherence constants from quantum information theory
    constexpr float COHERENCE_DECAY_RATE = 0.02f;
    constexpr float ENTANGLEMENT_THRESHOLD = 0.6f;
    constexpr float MEMORY_PRESSURE_FACTOR = 0.8f;
    constexpr uint32_t COHERENCE_UPDATE_BATCH_SIZE = 128;
    constexpr float MIN_COHERENCE_FOR_PERSISTENCE = 0.1f;
    
    // Memory tier coherence thresholds (example values)
    constexpr float LTM_COHERENCE_THRESHOLD = 0.8f;
    constexpr float MTM_COHERENCE_THRESHOLD = 0.5f;
    constexpr float STM_COHERENCE_THRESHOLD = 0.2f;
}

class QuantumCoherenceManager::Impl {
public:
    // Use the nested CoherenceMetrics struct from the outer class
    using CoherenceMetrics = QuantumCoherenceManager::CoherenceMetrics;
    using PatternCoherenceData = QuantumCoherenceManager::PatternCoherenceData;
    using TierMigration = QuantumCoherenceManager::TierMigration;
    using AnomalyType = QuantumCoherenceManager::AnomalyType;
    using CoherenceAnomaly = QuantumCoherenceManager::CoherenceAnomaly;
    using EntanglementGraph = QuantumCoherenceManager::EntanglementGraph;
    using TierAnalysis = QuantumCoherenceManager::TierAnalysis;
    using CoherenceResult = QuantumCoherenceManager::CoherenceResult;

    explicit Impl(const Config& config)
        : config_(config)
        , qfh_processor_(std::make_unique<sep::quantum::QuantumProcessorQFH>())
        , global_tick_(0) {

        
        
        initializeCoherenceTracking();
        
        if (config_.enable_cuda) {
            cuda_core_ = &cuda::CudaCore::instance();
            allocateGPUBuffers();
        }
    }

    CoherenceResult updateCoherence(const std::vector<sep::quantum::Pattern>& patterns)
    {
        CoherenceResult result;
        global_tick_++;

        // Update pattern coherence data
        tbb::parallel_for(
            tbb::blocked_range<size_t>(0, patterns.size(), COHERENCE_UPDATE_BATCH_SIZE),
            [this, &patterns](const tbb::blocked_range<size_t>& range) {
                for (size_t i = range.begin(); i != range.end(); ++i)
                {
                    updatePatternCoherence(patterns[i]);
                }
            });

        // Compute global coherence metrics
        computeGlobalMetrics();

        // Detect and handle coherence anomalies
        result.anomalies = detectCoherenceAnomalies(patterns);

        // Perform tier migrations based on coherence
        result.tier_migrations = performTierMigrations();

        // Update entanglement graph
        updateEntanglementGraph(patterns);

        // Fill result structure
        result.global_coherence = metrics_.global_coherence;
        result.memory_pressure = metrics_.memory_pressure;
        result.total_migrations = result.tier_migrations.size();
        result.success = true;

        // Fill in missing fields in result
        for (int i = 0; i < 3; ++i)
        {
            result.tier_fragmentation[i] = metrics_.tier_fragmentation[i];
            result.tier_pattern_count[i] =
                countPatternsInTier(static_cast<sep::memory::MemoryTierEnum>(i));
        }

        return result;
    }

    std::vector<TierMigration> optimizeMemoryLayout()
    {
        std::vector<TierMigration> migrations;

        analyzeTierCoherence();

        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it)
        {
            const auto& pair = *it;
            const auto& data = pair.second;
            memory::MemoryTierEnum target_tier = determineOptimalTier(data);

            if (target_tier != data.current_tier)
            {
                TierMigration migration;
                migration.pattern_id = data.pattern_id;
                migration.from_tier = data.current_tier;
                migration.to_tier = target_tier;
                migration.coherence = data.coherence;
                migration.reason = determineMigrationReason(data);
                migrations.push_back(migration);
            }
        }

        // Apply memory pressure optimizations
        if (metrics_.memory_pressure > MEMORY_PRESSURE_FACTOR)
        {
            applyMemoryPressureOptimizations(migrations);
        }

        return migrations;
    }

    EntanglementGraph computeEntanglementGraph(const std::vector<sep::quantum::Pattern>& patterns)
    {
        EntanglementGraph graph;
        graph.nodes.reserve(patterns.size());
        
        // Create nodes
        for (const auto& pattern : patterns) {
            EntanglementNode node;
            node.pattern_id = pattern.id;
            node.coherence = pattern.quantum_state.coherence;
            node.position = pattern.position;
            graph.nodes.push_back(node);
        }
        
        // Compute edges using quantum coherence measures
        for (size_t i = 0; i < patterns.size(); ++i) {
            for (size_t j = i + 1; j < patterns.size(); ++j) {
                float entanglement = computeEntanglement(patterns[i], patterns[j]);
                
                if (entanglement > ENTANGLEMENT_THRESHOLD) {
                    EntanglementEdge edge;
                    edge.node1_idx = i;
                    edge.node2_idx = j;
                    edge.strength = entanglement;
                    edge.phase_correlation = computePhaseCorrelation(patterns[i], patterns[j]);
                    graph.edges.push_back(edge);
                }
            }
        }
        
        // Compute graph metrics
        graph.total_entanglement = std::accumulate(
            graph.edges.begin(), graph.edges.end(), 0.0f,
            [](float sum, const EntanglementEdge& edge) { return sum + edge.strength; }
        );
        
        graph.max_degree = computeMaxDegree(graph);
        graph.clustering_coefficient = computeClusteringCoefficient(graph);
        
        return graph;
    }

    void applyCoherenceDecay(float decay_factor) {
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            auto& pair = *it;
            auto& data = pair.second;
            data.coherence *= (1.0f - decay_factor * COHERENCE_DECAY_RATE);

            // Remove patterns below minimum coherence
            if (data.coherence < MIN_COHERENCE_FOR_PERSISTENCE) {
                data.coherence = 0.0f;
            }
        }
        
        // Clean up zero-coherence patterns
        cleanupZeroCoherencePatterns();
    }
    
    CoherenceSnapshot createSnapshot() const {
        CoherenceSnapshot snapshot;
        snapshot.timestamp = global_tick_;
        snapshot.global_metrics = metrics_;
        
        // Capture pattern states
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            snapshot.pattern_states.push_back(pair.second);
        }
        
        // Capture tier distributions
        snapshot.tier_distribution[0] = countPatternsInTier(sep::memory::MemoryTierEnum::STM);
        snapshot.tier_distribution[1] = countPatternsInTier(sep::memory::MemoryTierEnum::MTM);
        snapshot.tier_distribution[2] = countPatternsInTier(sep::memory::MemoryTierEnum::LTM);
        
        return snapshot;
    }
    
    bool restoreFromSnapshot(const CoherenceSnapshot& snapshot) {
        // Clear current state
        coherence_map_.clear();
        
        // Restore pattern states
        for (const auto& state : snapshot.pattern_states) {
            coherence_map_.insert({state.pattern_id, state});
        }
        
        // Restore metrics
        metrics_ = snapshot.global_metrics;
        global_tick_ = snapshot.timestamp;

        return true;
    }

    const CoherenceMetrics& getMetrics() const {
        return metrics_;
    }

    uint64_t getGlobalTick() const {
        return global_tick_.load();
    }

    uint32_t getPatternCountByTier(sep::memory::MemoryTierEnum tier) const {
        return countPatternsInTier(tier);
    }

    float getTierFragmentation(sep::memory::MemoryTierEnum tier) const {
        int tier_idx = static_cast<int>(tier);
        if (tier_idx >= 0 && tier_idx < 3) {
            return metrics_.tier_fragmentation[tier_idx];
        }
        return 0.0f;
    }

private:
    Config config_;
    std::unique_ptr<sep::quantum::QuantumProcessorQFH> qfh_processor_;
    cuda::CudaCore* cuda_core_ = nullptr;
    
    // Concurrent data structures
    using CoherenceMap =
        tbb::concurrent_hash_map<std::string, QuantumCoherenceManager::PatternCoherenceData>;
    CoherenceMap coherence_map_;
    
    QuantumCoherenceManager::CoherenceMetrics metrics_;
    std::atomic<uint64_t> global_tick_;
    
    // GPU buffers for coherence computation
    std::unique_ptr<sep::cuda::DeviceMemory<float>> d_coherence_values_;
    std::unique_ptr<sep::cuda::DeviceMemory<float>> d_stability_values_;
    
    void initializeCoherenceTracking() {
        metrics_.global_coherence = 1.0f;
        metrics_.memory_pressure = 0.0f;
        metrics_.fragmented_patterns = 0;
        metrics_.entanglement_density = 0.0f;
        metrics_.total_patterns = 0;
        metrics_.coherent_patterns = 0;

        for (int i = 0; i < 3; ++i) {
            metrics_.tier_coherence[i] = 1.0f;
            metrics_.tier_fragmentation[i] = 0.0f;
        }
        
    }
    
    void allocateGPUBuffers() {
        if (cuda_core_) {
            size_t buffer_size = config_.max_patterns;
            d_coherence_values_ = std::make_unique<sep::cuda::DeviceMemory<float>>(buffer_size);
            d_stability_values_ = std::make_unique<sep::cuda::DeviceMemory<float>>(buffer_size);
        }
    }
    
    void updatePatternCoherence(const sep::quantum::Pattern& pattern) {
        CoherenceMap::accessor accessor;
        
        if (coherence_map_.find(accessor, pattern.id)) {
            // Update existing pattern
            auto& data = accessor->second;
            
            // Apply QFH processing for coherence update
            float new_coherence = qfh_processor_->processPattern(pattern.position);
            
            // Exponential moving average for stability
            data.coherence = 0.7f * data.coherence + 0.3f * new_coherence;
            data.stability = qfh_processor_->calculateStability(
                pattern.position,
                data.stability,
                pattern.quantum_state.generation,
                static_cast<float>(data.access_count) / global_tick_
            );
            
            data.access_count++;
            data.last_access_tick = global_tick_;

            // Update fragmentation score (example: inverse of stability)
            data.fragmentation_score = 1.0f - data.stability;
            
        } else {
            // Insert new pattern
            accessor.release();
            
            QuantumCoherenceManager::PatternCoherenceData new_data;
            new_data.pattern_id = pattern.id;
            new_data.coherence = qfh_processor_->processPattern(pattern.position);
            new_data.stability = qfh_processor_->calculateStability(
                pattern.position,
                0.5f, // Start with neutral stability
                pattern.quantum_state.generation,
                0.0f
            );
            new_data.access_count = 1;
            new_data.last_access_tick = global_tick_;
            new_data.current_tier = pattern.quantum_state.memory_tier;
            new_data.fragmentation_score = 1.0f - new_data.stability;
            
            coherence_map_.insert({pattern.id, new_data});
        }
    }
    
    void computeGlobalMetrics() {
        float total_coherence = 0.0f;
        uint64_t pattern_count = 0;
        uint64_t coherent_count = 0;
        float tier_sums[3] = {0.0f, 0.0f, 0.0f};
        float tier_frag_sums[3] = {0.0f, 0.0f, 0.0f};
        uint32_t tier_counts[3] = {0, 0, 0};
        
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            const auto& data = pair.second;
            total_coherence += data.coherence;
            pattern_count++;
            
            if (data.coherence > STM_COHERENCE_THRESHOLD) {
                coherent_count++;
            }
            
            int tier_idx = static_cast<int>(data.current_tier);
            tier_sums[tier_idx] += data.coherence;
            tier_frag_sums[tier_idx] += data.fragmentation_score;
            tier_counts[tier_idx]++;
        }
        
        // Update metrics
        metrics_.global_coherence = (pattern_count > 0) ? 
            total_coherence / pattern_count : 0.0f;
        metrics_.total_patterns = pattern_count;
        metrics_.coherent_patterns = coherent_count;
        
        // Compute tier coherences
        for (int i = 0; i < 3; ++i) {
            metrics_.tier_coherence[i] = (tier_counts[i] > 0) ?
                tier_sums[i] / static_cast<float>(tier_counts[i]) : 0.0f;
            metrics_.tier_fragmentation[i] = (tier_counts[i] > 0) ?
                tier_frag_sums[i] / static_cast<float>(tier_counts[i]) : 0.0f;
        }
        
        // Compute memory pressure
        float ltm_ratio = static_cast<float>(tier_counts[2]) / pattern_count;
        metrics_.memory_pressure = ltm_ratio;
        
        // Compute entanglement density
        uint32_t total_entanglements = 0;
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            total_entanglements += pair.second.entangled_patterns.size();
        }

        // Compute fragmented patterns (example: fragmentation score > 0.5)
        uint64_t fragmented_count = 0;
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            if (pair.second.fragmentation_score > 0.5f) fragmented_count++;
        }
        metrics_.fragmented_patterns = fragmented_count;

        metrics_.entanglement_density = (pattern_count > 1) ?
            static_cast<float>(total_entanglements) / (pattern_count * (pattern_count - 1)) : 0.0f;
    }

    std::vector<CoherenceAnomaly> detectCoherenceAnomalies(
        const std::vector<sep::quantum::Pattern>& patterns)
    {
        std::vector<CoherenceAnomaly> anomalies;

        // Statistical anomaly detection
        float mean_coherence = metrics_.global_coherence;
        float variance = computeCoherenceVariance();
        float std_dev = std::sqrt(variance);

        for (const auto& pattern : patterns)
        {
            CoherenceMap::const_accessor accessor;
            if (coherence_map_.find(accessor, pattern.id))
            {
                const auto& data = accessor->second;

                // Z-score based anomaly detection
                float z_score = (data.coherence - mean_coherence) / std_dev;

                if (std::abs(z_score) > 3.0f)
                {  // 3-sigma rule
                    CoherenceAnomaly anomaly;
                    anomaly.pattern_id = pattern.id;
                    anomaly.coherence_value = data.coherence;
                    anomaly.expected_value = mean_coherence;
                    anomaly.severity = std::abs(z_score) / 3.0f;
                    anomaly.type = (z_score > 0) ? AnomalyType::ExcessiveCoherence
                                                 : AnomalyType::InsufficientCoherence;

                    anomalies.push_back(anomaly);
                }

                // Detect rapid coherence changes
                if (data.access_count > 1)
                {
                    float coherence_change_rate =
                        std::abs(data.coherence - pattern.quantum_state.coherence);

                    if (coherence_change_rate > config_.anomaly_threshold)
                    {
                        CoherenceAnomaly anomaly;
                        anomaly.pattern_id = pattern.id;
                        anomaly.coherence_value = data.coherence;
                        anomaly.expected_value = pattern.quantum_state.coherence;
                        anomaly.severity = coherence_change_rate;
                        anomaly.type = AnomalyType::RapidChange;

                        anomalies.push_back(anomaly);
                    }
                }
            }
        }

        return anomalies;
    }

    std::vector<TierMigration> performTierMigrations()
    {
        std::vector<TierMigration> migrations;

        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it)
        {
            auto& pair = *it;
            auto& data = pair.second;
            sep::memory::MemoryTierEnum current_tier = data.current_tier;
            sep::memory::MemoryTierEnum target_tier = determineOptimalTier(data);

            if (current_tier != target_tier)
            {
                // Check migration conditions
                if (shouldMigrate(data, current_tier, target_tier))
                {
                    TierMigration migration;
                    migration.pattern_id = data.pattern_id;
                    migration.from_tier = current_tier;
                    migration.to_tier = target_tier;
                    migration.coherence = data.coherence;
                    migration.reason = determineMigrationReason(data);

                    migrations.push_back(migration);

                    // Update pattern tier
                    data.current_tier = target_tier;
                }
            }
        }

        return migrations;
    }

    void updateEntanglementGraph(const std::vector<sep::quantum::Pattern>& patterns)
    {
        // Clear existing entanglements
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            auto& pair = *it;
            pair.second.entangled_patterns.clear();
        }
        
        // Compute new entanglements
        for (size_t i = 0; i < patterns.size(); ++i) {
            for (size_t j = i + 1; j < patterns.size(); ++j) {
                float entanglement = computeEntanglement(patterns[i], patterns[j]);
                
                if (entanglement > ENTANGLEMENT_THRESHOLD) {
                    // Update both patterns
                    CoherenceMap::accessor accessor1, accessor2;
                    
                    if (coherence_map_.find(accessor1, patterns[i].id)) {
                        accessor1->second.entangled_patterns.push_back(patterns[j].id);
                    }
                    
                    if (coherence_map_.find(accessor2, patterns[j].id)) {
                        accessor2->second.entangled_patterns.push_back(patterns[i].id);
                    }
                }
            }
        }
    }

    sep::memory::MemoryTierEnum determineOptimalTier(const QuantumCoherenceManager::PatternCoherenceData& data) const {
        // Multi-factor tier determination
        float coherence_score = data.coherence;
        float stability_score = data.stability;
        float access_score = static_cast<float>(data.access_count) / global_tick_;
        float entanglement_score = static_cast<float>(data.entangled_patterns.size()) / 10.0f;
        
        // Weighted combination
        float total_score = 
            0.4f * coherence_score +
            0.3f * stability_score +
            0.2f * access_score +
            0.1f * glm::clamp(entanglement_score, 0.0f, 1.0f);
        
        if (total_score >= LTM_COHERENCE_THRESHOLD) {
            return sep::memory::MemoryTierEnum::LTM;
        } else if (total_score >= MTM_COHERENCE_THRESHOLD) {
            return sep::memory::MemoryTierEnum::MTM;
        } else {
            return sep::memory::MemoryTierEnum::STM;
        }
    }
    
    bool shouldMigrate(const QuantumCoherenceManager::PatternCoherenceData& data,
                      sep::memory::MemoryTierEnum from_tier,
                      sep::memory::MemoryTierEnum to_tier) const {
        // Hysteresis to prevent oscillation
        float hysteresis = 0.1f;
        
        if (to_tier > from_tier) {  // Promotion
            return data.coherence > (getThresholdForTier(to_tier) + hysteresis);
        } else {  // Demotion
            return data.coherence < (getThresholdForTier(from_tier) - hysteresis);
        }
    }
    
    MigrationReason determineMigrationReason(const QuantumCoherenceManager::PatternCoherenceData& data) const {
        if (data.coherence > 0.9f) return MigrationReason::HighCoherence;
        if (data.stability > 0.9f) return MigrationReason::HighStability;
        if (data.access_count > global_tick_ / 10) return MigrationReason::FrequentAccess;
        if (!data.entangled_patterns.empty()) return MigrationReason::Entanglement;
        if (metrics_.memory_pressure > MEMORY_PRESSURE_FACTOR) return MigrationReason::MemoryPressure;
        return MigrationReason::LowActivity;
    }

    void applyMemoryPressureOptimizations(std::vector<TierMigration>& migrations)
    {
        // Sort by coherence ascending for demotion candidates
        std::vector<std::pair<std::string, float>> demotion_candidates;

        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            if (pair.second.current_tier == sep::memory::MemoryTierEnum::LTM &&
                pair.second.coherence < LTM_COHERENCE_THRESHOLD) {
                demotion_candidates.push_back({pair.first, pair.second.coherence});
            }
        }
        
        std::sort(demotion_candidates.begin(), demotion_candidates.end(),
                 [](const auto& a, const auto& b) { return a.second < b.second; });
        
        // Demote lowest coherence patterns
        float demote_count = demotion_candidates.size() * 0.2f;  // Demote 20%
        
        for (size_t i = 0; i < demote_count && i < demotion_candidates.size(); ++i) {
            TierMigration migration;
            migration.pattern_id = demotion_candidates[i].first;
            migration.from_tier = ::sep::memory::MemoryTierEnum::LTM;
            migration.to_tier = ::sep::memory::MemoryTierEnum::MTM;
            migration.coherence = demotion_candidates[i].second;
            migration.reason = MigrationReason::MemoryPressure;
            
            migrations.push_back(migration);
        }
    }

    void cleanupZeroCoherencePatterns() {
        std::vector<std::string> to_remove;

        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            if (pair.second.coherence <= 0.0f) {
                to_remove.push_back(pair.first);
            }
        }
        
        for (const auto& id : to_remove) {
            coherence_map_.erase(id);
        }
    }
    
    float computeCoherenceVariance() const {
        float mean = metrics_.global_coherence;
        float variance = 0.0f;
        uint64_t count = 0;
        
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            float diff = pair.second.coherence - mean;
            variance += diff * diff;
            count++;
        }
        
        return (count > 0) ? variance / count : 0.0f;
    }
    
    float computeEntanglement(const sep::quantum::Pattern& p1, const sep::quantum::Pattern& p2) const {
        // Quantum entanglement measure based on state overlap
        float spatial_overlap = std::exp(-glm::length(p1.position - p2.position));
        float phase_correlation = std::cos(p1.quantum_state.phase - p2.quantum_state.phase);
        float coherence_product = p1.quantum_state.coherence * p2.quantum_state.coherence;
        
        return coherence_product * spatial_overlap * std::abs(phase_correlation);
    }
    
    float computePhaseCorrelation(const sep::quantum::Pattern& p1, const sep::quantum::Pattern& p2) const {
        return std::cos(p1.quantum_state.phase - p2.quantum_state.phase);
    }
    
    uint32_t computeMaxDegree(const EntanglementGraph& graph) const {
        std::vector<uint32_t> degrees(graph.nodes.size(), 0);

        for (const auto& edge : graph.edges) {
            degrees[edge.node1_idx]++;
            degrees[edge.node2_idx]++;
        }
        
        return *std::max_element(degrees.begin(), degrees.end());
    }
    
    float computeClusteringCoefficient(const EntanglementGraph& graph) const {
        if (graph.nodes.size() < 3) return 0.0f;
        
        float total_clustering = 0.0f;
        
        for (size_t i = 0; i < graph.nodes.size(); ++i) {
            std::vector<size_t> neighbors;

            // Find neighbors of node i
            for (const auto& edge : graph.edges) {
                if (edge.node1_idx == i) neighbors.push_back(edge.node2_idx);
                else if (edge.node2_idx == i) neighbors.push_back(edge.node1_idx);
            }
            
            if (neighbors.size() >= 2) {
                // Count triangles
                uint32_t triangles = 0;
                for (size_t j = 0; j < neighbors.size(); ++j) {
                    for (size_t k = j + 1; k < neighbors.size(); ++k) {
                        // Check if neighbors are connected
                        for (const auto& edge : graph.edges) {
                            if ((edge.node1_idx == neighbors[j] && edge.node2_idx == neighbors[k]) ||
                                (edge.node1_idx == neighbors[k] && edge.node2_idx == neighbors[j])) {
                                triangles++;
                                break;
                            }
                        }
                    }
                }
                
                float possible_triangles = neighbors.size() * (neighbors.size() - 1) / 2.0f;
                total_clustering += triangles / possible_triangles;
            }
        }
        
        return total_clustering / graph.nodes.size();
    }
    
    uint32_t countPatternsInTier(sep::memory::MemoryTierEnum tier) const {
        uint32_t count = 0;
        for (auto it = coherence_map_.begin(); it != coherence_map_.end(); ++it) {
            const auto& pair = *it;
            if (pair.second.current_tier == tier) {
                count++;
            }
        }
        return count;
    }
    
    float getThresholdForTier(sep::memory::MemoryTierEnum tier) const {
        switch (tier) {
            case ::sep::memory::MemoryTierEnum::LTM: return LTM_COHERENCE_THRESHOLD;
            case ::sep::memory::MemoryTierEnum::MTM: return MTM_COHERENCE_THRESHOLD;
            case ::sep::memory::MemoryTierEnum::STM: return STM_COHERENCE_THRESHOLD;
            default: return 0.0f;
        }
    }
    
    TierAnalysis analyzeTierCoherence() const {
        TierAnalysis analysis;
        
        for (int i = 0; i < 3; ++i) {
            analysis.tier_coherence[i] = metrics_.tier_coherence[i];
            analysis.tier_pattern_count[i] = countPatternsInTier(static_cast<::sep::memory::MemoryTierEnum>(i));
        }
        
        analysis.optimal_distribution = computeOptimalDistribution();
        
        return analysis;
    }
    
    std::array<float, 3> computeOptimalDistribution() const {
        // Optimal tier distribution based on memory pressure and coherence
        float pressure = metrics_.memory_pressure;
        
        return {
            0.5f * (1.0f - pressure),  // STM: More when low pressure
            0.3f,                      // MTM: Constant buffer
            0.2f + 0.3f * pressure     // LTM: More when high pressure
        };
    }
};

// Public interface implementation
QuantumCoherenceManager::QuantumCoherenceManager(const Config& config)
    : impl_(std::make_unique<Impl>(config)) {}

QuantumCoherenceManager::~QuantumCoherenceManager() = default;

QuantumCoherenceManager::CoherenceResult QuantumCoherenceManager::updateCoherence(
    const std::vector<sep::quantum::Pattern>& patterns)
{
    return impl_->updateCoherence(patterns);
}

std::vector<QuantumCoherenceManager::TierMigration> QuantumCoherenceManager::optimizeMemoryLayout()
{
    return impl_->optimizeMemoryLayout();
}

QuantumCoherenceManager::EntanglementGraph QuantumCoherenceManager::computeEntanglementGraph(
    const std::vector<sep::quantum::Pattern>& patterns)
{
    return impl_->computeEntanglementGraph(patterns);
}

void QuantumCoherenceManager::applyCoherenceDecay(float decay_factor) {
    impl_->applyCoherenceDecay(decay_factor);
}

QuantumCoherenceManager::CoherenceSnapshot
QuantumCoherenceManager::createSnapshot() const {
    return impl_->createSnapshot();
}

bool QuantumCoherenceManager::restoreFromSnapshot(const CoherenceSnapshot& snapshot) {
    return impl_->restoreFromSnapshot(snapshot);
}

const QuantumCoherenceManager::CoherenceMetrics& QuantumCoherenceManager::getMetrics() const {
    return impl_->getMetrics();
}

uint64_t QuantumCoherenceManager::getGlobalTick() const {
    return impl_->getGlobalTick();
}

uint32_t QuantumCoherenceManager::getPatternCountByTier(MemoryTierEnum tier) const {
    return impl_->getPatternCountByTier(tier);
}

float QuantumCoherenceManager::getTierFragmentation(MemoryTierEnum tier) const {
    return impl_->getTierFragmentation(tier);
}

// Factory function
std::unique_ptr<QuantumCoherenceManager> createQuantumCoherenceManager(const QuantumCoherenceManager::Config& config) {
    return std::make_unique<QuantumCoherenceManager>(config);
}

} // namespace sep::memory
