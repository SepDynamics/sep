#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "demo_base.hpp"
#include "demo_manager.hpp"
#include "engine/types.h"
#include "imgui.h"
#include "memory/quantum_coherence_manager.h"
#include "quantum/quantum_processor.h"
#include "apps/workbench/renderer.h"

// Forward declarations
namespace sep {
namespace core {
class Engine;
}
class SimpleRenderer;
}

namespace sep {
namespace workbench {

class AnnealingSimDemo : public Demo {
public:
    void on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer) override;
    void on_ui_render() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button);

private:
    sep::SimpleRenderer* renderer_{nullptr};
    struct Particle {
        glm::vec3 position{0.0f};
        glm::vec3 velocity{0.0f};
        glm::vec3 color{1.0f};
    };

    std::unique_ptr<sep::quantum::QuantumProcessor> processor_;
    std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_mgr_;
    std::vector<Particle> particles_;
    std::vector<float> temperature_schedule_;
    std::size_t current_step_{0};
    bool paused_{false};
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
};

} // namespace workbench
}  // namespace sep