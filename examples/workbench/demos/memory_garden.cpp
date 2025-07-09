#include "memory_garden.hpp"
#include <config.hpp>
#include <glm/vec3.hpp>
#include "memory/memory_tier_manager.hpp"
#include "memory/quantum_coherence_manager.h"

namespace sep {
namespace workbench {

void MemoryGardenDemo::on_load() {
    const auto& cfg = Config::getInstance().memory_garden();
    
    // Initialize memory tier manager
    memory_manager_ = std::make_unique<MemoryTierManager>();
    coherence_manager_ = std::make_unique<QuantumCoherenceManager>();
    
    // Configure tier radii
    stm_radius_ = cfg.layout.stm_radius;
    mtm_radius_ = cfg.layout.mtm_radius;
    ltm_radius_ = cfg.layout.ltm_radius;
    
    // Initialize visualization settings
    show_connections_ = cfg.visualization.show_connections;
    connection_opacity_ = cfg.visualization.connection_opacity;
    pattern_scale_ = cfg.visualization.pattern_scale;
    
    // Create some initial patterns for demonstration
    createInitialPatterns();
}

void MemoryGardenDemo::createInitialPatterns() {
    // Create patterns with varying coherence levels
    for (int i = 0; i < 5; ++i) {
        Node node;
        node.coherence = 0.2f + (i * 0.2f);  // Spread across coherence range
        node.position = calculateNodePosition(node.coherence);
        nodes_.push_back(node);
    }
}

glm::vec3 MemoryGardenDemo::calculateNodePosition(float coherence) {
    float radius;
    if (coherence < 0.2f) {
        radius = stm_radius_;
    } else if (coherence < 0.6f) {
        radius = mtm_radius_;
    } else {
        radius = ltm_radius_;
    }
    
    // Calculate position on circular layout
    float angle = static_cast<float>(nodes_.size()) * glm::pi<float>() * 0.5f;
    return glm::vec3(
        radius * std::cos(angle),
        0.0f,
        radius * std::sin(angle)
    );
}

void MemoryGardenDemo::on_update(float dt) {
    // Update pattern positions based on coherence
    for (auto& node : nodes_) {
        // Simulate coherence changes
        node.coherence += (std::rand() % 100 - 50) * 0.001f * dt;
        node.coherence = glm::clamp(node.coherence, 0.0f, 1.0f);
        
        // Update position based on new coherence
        glm::vec3 target = calculateNodePosition(node.coherence);
        node.position = glm::mix(node.position, target, dt);
    }
    
    // Update pattern relationships
    updateRelationships();
}

void MemoryGardenDemo::updateRelationships() {
    relationships_.clear();
    
    // Create relationships between patterns with similar coherence
    for (size_t i = 0; i < nodes_.size(); ++i) {
        for (size_t j = i + 1; j < nodes_.size(); ++j) {
            float coherence_diff = std::abs(nodes_[i].coherence - nodes_[j].coherence);
            if (coherence_diff < 0.3f) {  // Threshold for relationship formation
                float strength = 1.0f - (coherence_diff / 0.3f);
                relationships_.push_back({i, j, strength});
            }
        }
    }
}

void MemoryGardenDemo::on_render() {
    if (!renderer_) return;
    
    // Render tier boundaries
    renderer_->setColorMode("coherence");
    renderer_->setEmissionMode("stability");
    
    // Render patterns
    std::vector<glm::vec3> points;
    for (const auto& node : nodes_) {
        points.push_back(node.position);
    }
    renderer_->renderPatternState(points);
    
    // Render relationships
    if (show_connections_) {
        for (const auto& rel : relationships_) {
            const auto& start = nodes_[rel.from].position;
            const auto& end = nodes_[rel.to].position;
            renderer_->renderConnection(start, end, rel.strength * connection_opacity_);
        }
    }
}

void MemoryGardenDemo::on_unload() {
    nodes_.clear();
}

void MemoryGardenDemo::on_key_press(int key)
{
    switch (key) {
        case 'n':  // New pattern
            {
                Node node;
                node.coherence = 0.2f;  // Start in STM
                node.position = calculateNodePosition(node.coherence);
                nodes_.push_back(node);
            }
            break;
        case 'c':  // Toggle connections
            show_connections_ = !show_connections_;
            break;
        case '+':  // Increase connection opacity
            connection_opacity_ = std::min(1.0f, connection_opacity_ + 0.1f);
            break;
        case '-':  // Decrease connection opacity
            connection_opacity_ = std::max(0.1f, connection_opacity_ - 0.1f);
            break;
        case 's':  // Scale patterns up
            pattern_scale_ = std::min(2.0f, pattern_scale_ + 0.1f);
            break;
        case 'S':  // Scale patterns down
            pattern_scale_ = std::max(0.5f, pattern_scale_ - 0.1f);
            break;
        case 'r':  // Reset visualization
            show_connections_ = true;
            connection_opacity_ = 0.5f;
            pattern_scale_ = 1.0f;
            break;
        case ' ':  // Clear all patterns
            nodes_.clear();
            relationships_.clear();
            createInitialPatterns();
            break;
    }
}

void MemoryGardenDemo::on_mouse(int x, int y, int button) {
    const float ROTATION_SPEED = 0.01f;
    const float ZOOM_SPEED = 0.1f;
    
    if (button == 0) {  // Left button - rotate
        for (auto& node : nodes_) {
            float angle = x * ROTATION_SPEED;
            float cos_angle = std::cos(angle);
            float sin_angle = std::sin(angle);
            float old_x = node.position.x;
            float old_z = node.position.z;
            node.position.x = old_x * cos_angle - old_z * sin_angle;
            node.position.z = old_x * sin_angle + old_z * cos_angle;
        }
    }
    else if (button == 1) {  // Right button - zoom
        float zoom = 1.0f + (y * ZOOM_SPEED);
        stm_radius_ *= zoom;
        mtm_radius_ *= zoom;
        ltm_radius_ *= zoom;
        
        // Update pattern positions
        for (auto& node : nodes_) {
            node.position *= zoom;
        }
    }
}

} // namespace workbench
} // namespace sep
