#pragma once

#include "demo_manager.hpp"
#include "memory/memory_tier_manager.hpp"
#include "memory/quantum_coherence_manager.h"
#include <memory>
#include <vector>

namespace sep {
namespace workbench {

class MemoryGardenDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

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
