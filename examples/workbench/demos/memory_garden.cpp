#include "memory_garden.hpp"

namespace sep {
namespace workbench {

void MemoryGardenDemo::init() {
    manager_ = &memory::MemoryTierManager::getInstance();
}

void MemoryGardenDemo::update(float) {
    // For now just query total allocation
    manager_->getTotalAllocated();
}

void MemoryGardenDemo::render() {}

void MemoryGardenDemo::cleanup() {
    manager_ = nullptr;
}

void MemoryGardenDemo::handleKeyboard(unsigned char) {}
void MemoryGardenDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
