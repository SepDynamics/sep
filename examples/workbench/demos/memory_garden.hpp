#pragma once

#include "demo_manager.hpp"
#include "config.hpp"
#include "memory/memory_tier_manager.hpp"

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
    memory::MemoryTierManager* manager_{nullptr};
};

} // namespace workbench
} // namespace sep
