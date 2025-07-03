#pragma once

#include <memory>
#include "../demo_manager.hpp"
#include "../config.hpp"

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
    float angle_{0.0f};
};

} // namespace workbench
} // namespace sep
