#pragma once

#include "demo_manager.hpp"
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
    struct Parcel {
        glm::vec3 position;
        glm::vec3 velocity{0.0f};
        float mass{1.0f};
    };

    std::vector<Parcel> parcels_;
    float box_size_{100.0f};
    float timestep_{0.01f};
    bool show_potential_{false};

    void applyGravity();
    void enforceBounds();
};

} // namespace workbench
} // namespace sep
