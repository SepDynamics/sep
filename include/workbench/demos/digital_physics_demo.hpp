#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <algorithm>

namespace sep {
namespace workbench {

class DigitalPhysicsDemo : public Demo {
public:
    void init() override;
    void update(float dt) override;
    void render() override;
    void cleanup() override;
    void handleKeyboard(unsigned char key) override;
    void handleMouse(int x, int y, int button) override;

private:
    std::size_t width_{0};
    std::size_t height_{0};
    std::vector<sep::pattern::PatternData> grid_;
    std::vector<int> birth_rules_;
    std::vector<int> survival_rules_;

    std::size_t index(std::size_t x, std::size_t y) const { return y * width_ + x; }
};

} // namespace workbench
} // namespace sep
