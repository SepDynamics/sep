#pragma once

#include "demo_manager.hpp"
#include <quantum/data.hpp>
#include <vector>

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
        bool alive{false};
    };

    std::vector<Cell> grid_;
    int width_{16};
    int height_{16};
    bool running_{true};

    struct {
        int birth{3};
        int survive_min{2};
        int survive_max{3};
    } rules_;

    int index(int x, int y) const { return y * width_ + x; }
    int countAliveNeighbors(int x, int y) const;
    void step();
};

} // namespace workbench
} // namespace sep
