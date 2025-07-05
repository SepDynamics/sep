#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <algorithm>
#include <quantum/data.hpp>

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
    struct Cell {
        pattern::PatternData data;
        bool active{false};
    };

    int width_{0};
    int height_{0};
    std::vector<Cell> grid_;
    std::vector<Cell> next_;

    std::vector<int> birth_;
    std::vector<int> survive_;

    int index(int x, int y) const { return y * width_ + x; }
    int countNeighbors(int x, int y) const;
    void step();
};

} // namespace workbench
} // namespace sep
