#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class CosmoDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

private:
    struct Particle {
        glm::vec3 position;
        glm::vec3 velocity;
        float mass;
    };

    std::vector<Particle> particles_;

    float box_size_{50.0f};
    float time_step_{0.01f};

    void initParticles();
    void integrate(float dt);
};

} // namespace workbench
} // namespace sep
