#include "memory_garden.hpp"
#include "blender/cycles_renderer.h"

namespace sep {
namespace workbench {

void MemoryGardenDemo::init() {}

void MemoryGardenDemo::update(float dt) {
    angle_ += dt;
    (void)angle_;
}

void MemoryGardenDemo::render() {
    // Visualization placeholder
}

void MemoryGardenDemo::cleanup() {}

void MemoryGardenDemo::handleKeyboard(unsigned char) {}
void MemoryGardenDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
