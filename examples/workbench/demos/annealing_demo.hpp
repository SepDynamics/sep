#pragma once

#include "../demo_manager.hpp"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class AnnealingDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

private:
    struct Particle {
        glm::vec3 position{0.0f};
        glm::vec3 velocity{0.0f};
    };

    std::vector<Particle> particles_;
    float temperature_{1.0f};
    float cooling_rate_{0.99f};
    bool paused_{false};
};

} // namespace workbench
} // namespace sep
