#pragma once

#include "demo_manager.hpp"
#include "quantum/data.hpp"
#include <vector>
#include <algorithm>

namespace sep {
namespace workbench {

class DigitalPhysicsDemo : public Demo {
public:
    void on_load() override;
    void on_update(float dt) override;
    void on_render() override;
    void on_unload() override;
    void on_key_press(unsigned char key) override;
    void on_mouse(int x, int y, int button);

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
