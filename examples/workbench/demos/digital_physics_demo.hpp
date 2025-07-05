#pragma once

#include "demo_manager.hpp"
#include <vector>
#include <string>

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
    int width_{0};
    int height_{0};
    std::vector<int> birth_rules_;
    std::vector<int> survive_rules_;
    std::string rule_string_{"B3/S23"};

    int index(int x, int y) const { return y * width_ + x; }
    int neighborCount(int x, int y) const;
    void parseRule(const std::string& rule);
    void step();
};

} // namespace workbench
} // namespace sep
