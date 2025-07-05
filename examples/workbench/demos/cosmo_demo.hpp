#pragma once

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"
#include <vector>
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

class CosmoDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    struct Body {
        glm::vec3 position;
        glm::vec3 velocity;
        float mass;
    };

    std::vector<Body> bodies_;
    float box_size_{100.0f};
    float time_step_{0.01f};
    float gravity_const_{1.0f};

    void initializeBodies();
    void integrate(float dt);
};

} // namespace workbench
} // namespace sep
