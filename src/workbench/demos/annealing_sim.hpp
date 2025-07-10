#pragma once

#include <glm/vec3.hpp>
#include <memory>
#include <vector>

#include "../../workbench_demo_adapter.hpp"
#include "demo_manager.hpp"
#include "memory/quantum_coherence_manager.h"
#include "quantum/processor.h"

namespace sep {
namespace workbench {



class AnnealingSimDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(int key) override;
    void on_mouse(int x, int y, int button) override;

private:
    struct Particle {
        glm::vec3 position{0.0f};
        glm::vec3 velocity{0.0f};
        glm::vec3 color{1.0f};
    };

    std::unique_ptr<sep::pattern::PatternProcessor> processor_;
    std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_mgr_;
    std::vector<Particle> particles_;
    std::vector<float> temperature_schedule_;
    std::size_t current_step_{0};
    bool paused_{false};
};

} // namespace workbench
} // namespace sep
