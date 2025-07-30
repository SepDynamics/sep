#include <cmath>
#include <glm/geometric.hpp>
#include <iomanip>
#include <sstream>

#include "dag_graph.h"

namespace sep {
namespace dag {

    uint64_t DagGraph::addMarketDataNode(const glm::vec3& pattern, float coherence, float price,
                                         float volatility, float volume,
                                         const std::vector<uint64_t>& parents)
    {
        uint64_t id = addNode(pattern, coherence, parents);

        if (nodes_.find(id) != nodes_.end())
        {
            nodes_[id].price = price;
            nodes_[id].volatility = volatility;
            nodes_[id].volume = volume;
            nodes_[id].stability = coherence;  // Initial stability equals coherence
        }

        return id;
    }

void DagGraph::updateVolatility(uint64_t id, float volatility) {
    auto it = nodes_.find(id);
    if (it != nodes_.end()) {
        it->second.volatility = volatility;
    }
}

void DagGraph::updatePrice(uint64_t id, float price) {
    auto it = nodes_.find(id);
    if (it != nodes_.end()) {
        it->second.price = price;
    }
}

void DagGraph::updateVolume(uint64_t id, float volume) {
    auto it = nodes_.find(id);
    if (it != nodes_.end()) {
        it->second.volume = volume;
    }
}

void DagGraph::updateStability(uint64_t id, float stability) {
    auto it = nodes_.find(id);
    if (it != nodes_.end()) {
        it->second.stability = stability;
    }
}

void DagGraph::updateGeneration(uint64_t id, uint32_t generation) {
    auto it = nodes_.find(id);
    if (it != nodes_.end()) {
        it->second.generation = generation;
    }
}

void DagGraph::calculateNodeCorrelations() {
    // Calculate correlations between nodes based on pattern similarity
    for (auto& [id1, node1] : nodes_) {
        float totalCorrelation = 0.0f;
        int count = 0;
        
        for (const auto& [id2, node2] : nodes_) {
            if (id1 != id2) {
                // Calculate correlation based on pattern distance
                glm::vec3 diff = node1.pattern - node2.pattern;
                float distance = glm::length(diff);
                float correlation = 1.0f / (1.0f + distance);
                
                // Weight by coherence
                correlation *= (node1.coherence + node2.coherence) * 0.5f;
                
                totalCorrelation += correlation;
                count++;
            }
        }
        
        if (count > 0) {
            node1.correlation = totalCorrelation / count;
        }
    }
}

void DagGraph::calculateTailRisk() {
    // Calculate tail risk based on volatility and correlation
    for (auto& [id, node] : nodes_) {
        // High volatility + low correlation = high tail risk
        float riskFactor = node.volatility * (1.0f - node.correlation);
        
        // Consider volume impact
        float volumeImpact = 1.0f - std::min(1.0f, node.volume / 1000000.0f); // Normalize to 1M volume
        
        node.tail_risk = riskFactor * (1.0f + volumeImpact * 0.5f);
    }
}

void DagGraph::calculateAlpha() {
    // Calculate alpha based on coherence, generation, and stability
    for (auto& [id, node] : nodes_) {
        // Base alpha from coherence
        float baseAlpha = node.coherence;
        
        // Generation bonus (more evolved patterns have higher alpha)
        float generationBonus = std::log1p(static_cast<float>(node.generation)) * 0.1f;
        
        // Stability factor
        float stabilityFactor = node.stability;
        
        // Risk-adjusted alpha
        float riskAdjustment = 1.0f - (node.tail_risk * 0.5f);
        
        node.alpha = (baseAlpha + generationBonus) * stabilityFactor * riskAdjustment;
    }
}

float DagGraph::getVolatility(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.volatility : 0.0f;
}

float DagGraph::getPrice(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.price : 0.0f;
}

float DagGraph::getVolume(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.volume : 0.0f;
}

float DagGraph::getStability(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.stability : 0.0f;
}

float DagGraph::getTailRisk(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.tail_risk : 0.0f;
}

float DagGraph::getAlpha(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.alpha : 0.0f;
}

float DagGraph::getCorrelation(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.correlation : 0.0f;
}

uint32_t DagGraph::getGeneration(uint64_t id) const {
    auto it = nodes_.find(id);
    return (it != nodes_.end()) ? it->second.generation : 0;
}

const DagNode* DagGraph::getMostRecentNode() const {
    if (nodes_.empty()) return nullptr;
    auto it = nodes_.find(next_id_ - 1);
    if (it != nodes_.end()) return &it->second;
    return nullptr;
}

std::string DagGraph::exportAsJson() const
{
    std::stringstream json;
    json << std::fixed << std::setprecision(4);
    json << "{\n";
    
    // Export nodes
    json << "  \"nodes\": [\n";
    bool firstNode = true;
    for (const auto& [id, node] : nodes_) {
        if (!firstNode) json << ",\n";
        firstNode = false;
        
        json << "    {\n";
        json << "      \"id\": " << id << ",\n";
        json << "      \"pattern\": [" << node.pattern.x << ", " << node.pattern.y << ", " << node.pattern.z << "],\n";
        json << "      \"coherence\": " << node.coherence << ",\n";
        json << "      \"price\": " << node.price << ",\n";
        json << "      \"volatility\": " << node.volatility << ",\n";
        json << "      \"volume\": " << node.volume << ",\n";
        json << "      \"stability\": " << node.stability << ",\n";
        json << "      \"generation\": " << node.generation << ",\n";
        json << "      \"tail_risk\": " << node.tail_risk << ",\n";
        json << "      \"alpha\": " << node.alpha << ",\n";
        json << "      \"correlation\": " << node.correlation << "\n";
        json << "    }";
    }
    json << "\n  ],\n";
    
    // Export edges (derived from parent relationships in nodes)
    json << "  \"edges\": [\n";
    bool firstEdge = true;
    int edgeCount = 0;
    for (const auto& [childId, childNode] : nodes_) {
        for (uint64_t parentId : childNode.parents) {
            if (!firstEdge) json << ",\n";
            firstEdge = false;
            edgeCount++;
            
            // Calculate edge strength based on nodes' coherence
            float strength = 0.5f;
            auto parentIt = nodes_.find(parentId);
            if (parentIt != nodes_.end()) {
                strength = (childNode.coherence + parentIt->second.coherence) * 0.5f;
            }
            
            json << "    {\n";
            json << "      \"from\": " << parentId << ",\n";
            json << "      \"to\": " << childId << ",\n";
            json << "      \"strength\": " << strength << "\n";
            json << "    }";
        }
    }
    json << "\n  ],\n";
    
    // Export metrics
    json << "  \"metrics\": {\n";
    
    // Calculate aggregate metrics
    float totalAlpha = 0.0f;
    float totalRisk = 0.0f;
    float avgCoherence = 0.0f;
    float avgStability = 0.0f;
    int nodeCount = nodes_.size();
    
    for (const auto& [id, node] : nodes_) {
        totalAlpha += node.alpha;
        totalRisk += node.tail_risk;
        avgCoherence += node.coherence;
        avgStability += node.stability;
    }
    
    if (nodeCount > 0) {
        avgCoherence /= nodeCount;
        avgStability /= nodeCount;
        totalRisk /= nodeCount;
    }
    
    json << "    \"total_alpha\": " << totalAlpha << ",\n";
    json << "    \"avg_risk\": " << totalRisk << ",\n";
    json << "    \"avg_coherence\": " << avgCoherence << ",\n";
    json << "    \"avg_stability\": " << avgStability << ",\n";
    json << "    \"node_count\": " << nodeCount << ",\n";
    json << "    \"edge_count\": " << edgeCount << "\n";
    json << "  }\n";
    
    json << "}";
    
    return json.str();
}

} // namespace dag
} // namespace sep