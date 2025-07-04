#pragma once

#include "demo_manager.hpp"
#include <memory>
#include <vector>

namespace sep {
namespace workbench {

class MemoryGardenDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Node {
        glm::vec3 position;
        float coherence{0.f};
    };

    struct Relationship {
        size_t from;
        size_t to;
        float strength;
    };

    std::unique_ptr<MemoryTierManager> memory_manager_;
    std::unique_ptr<QuantumCoherenceManager> coherence_manager_;

    std::vector<Node> nodes_;
    std::vector<Relationship> relationships_;

    // Layout parameters
    float stm_radius_{10.0f};
    float mtm_radius_{20.0f};
    float ltm_radius_{30.0f};

    // Visualization settings
    bool show_connections_{true};
    float connection_opacity_{0.5f};
    float pattern_scale_{1.0f};

private:
    void createInitialPatterns();
    void updateRelationships();
    glm::vec3 calculateNodePosition(float coherence);
};

} // namespace workbench
} // namespace sep
