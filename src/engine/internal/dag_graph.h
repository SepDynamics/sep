#pragma once

#include <cstdint>
#include <glm/vec3.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include "engine/internal/standard_includes.h"

namespace sep {
namespace dag {

// Enhanced DAG node for quantitative trading data analysis
struct DagNode
{
    uint64_t              id;
    glm::vec3             pattern;          // Original pattern data
    float                 coherence;        // Quantum coherence
    std::vector<uint64_t> parents;          // Parent node IDs

    // Quant-specific attributes
    float                 volatility{0.0f}; // Market volatility
    float                 price{0.0f};      // Current price or value
    float                 volume{0.0f};     // Trading volume
    float                 stability{0.0f};  // Pattern stability metric
    uint32_t              generation{0};    // Pattern generation/evolution count
    
    // Risk and correlation metrics
    float                 tail_risk{0.0f};  // Tail risk derived from low coherence
    float                 alpha{0.0f};      // Alpha generation potential
    float                 correlation{0.0f}; // Correlation strength with parent nodes
};

class DagGraph
{
public:
    // Core node operations
    uint64_t addNode(const glm::vec3& pattern, float coherence,
                     const std::vector<uint64_t>& parents);
    uint64_t addNodeWithId(uint64_t id, const glm::vec3& pattern, float coherence,
                           const std::vector<uint64_t>& parents);
    void updateCoherence(uint64_t id, float coherence);
    void updateNodeParents(uint64_t id, const std::vector<uint64_t>& parents);
    std::vector<uint64_t> getParents(uint64_t id) const;
    void removeNode(uint64_t id);
    bool hasNode(uint64_t id) const;
    
    // Quant-specific node operations
    uint64_t addMarketDataNode(const glm::vec3& pattern, float coherence, float price,
                               float volatility, float volume,
                               const std::vector<uint64_t>& parents);

    // Node attribute updates
    void updateVolatility(uint64_t id, float volatility);
    void updatePrice(uint64_t id, float price);
    void updateVolume(uint64_t id, float volume);
    void updateStability(uint64_t id, float stability);
    void updateGeneration(uint64_t id, uint32_t generation);
    
    // Metrics calculation
    void calculateNodeCorrelations();
    void calculateTailRisk();
    void calculateAlpha();
    
    // Quant-specific attribute getters
    float getVolatility(uint64_t id) const;
    float getPrice(uint64_t id) const;
    float getVolume(uint64_t id) const;
    float getStability(uint64_t id) const;
    float getTailRisk(uint64_t id) const;
    float getAlpha(uint64_t id) const;
    float getCorrelation(uint64_t id) const;
    uint32_t getGeneration(uint64_t id) const;

    // Get most recently added node, or nullptr if empty
    const DagNode* getMostRecentNode() const;
    
    // JSON serialization for metrics output
    std::string exportAsJson() const;

    // Compatibility overload for integral identifiers that are not uint64_t
    template <typename T>
    typename std::enable_if_t<std::is_integral_v<T> && !std::is_same_v<T, uint64_t>, uint64_t>
    addNode(T /*pattern_id*/, float coherence, const std::vector<T>& parents)
    {
        std::vector<uint64_t> converted_parents;
        converted_parents.reserve(parents.size());
        for (const auto& p : parents)
        {
            converted_parents.push_back(static_cast<uint64_t>(p));
        }
        glm::vec3 default_pattern(0.0f);
        return addNode(default_pattern, coherence, converted_parents);
    }

private:
    uint64_t                              next_id_{1};
    std::unordered_map<uint64_t, DagNode> nodes_;
};

}  // namespace dag
}  // namespace sep
