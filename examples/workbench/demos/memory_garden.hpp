#pragma once

#include "examples/workbench/demo_manager.hpp"
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

    std::vector<Node> nodes_;
};

} // namespace workbench
} // namespace sep
