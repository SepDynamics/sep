#include "memory_garden.hpp"
#include "config.hpp"
#include <glm/vec3.hpp>

namespace sep {
namespace workbench {

void MemoryGardenDemo::init() {
    const auto& cfg = Config::getInstance().memory_garden();
    nodes_.resize(10);
    for (std::size_t i = 0; i < nodes_.size(); ++i) {
        nodes_[i].position = glm::vec3(i, 0.0f, 0.0f);
        nodes_[i].coherence = 1.0f - static_cast<float>(i) / nodes_.size();
    }
}

void MemoryGardenDemo::update(float) {
    // Simple rotation for demonstration
    for (auto& n : nodes_) {
        n.position.y += 0.01f;
    }
}

void MemoryGardenDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& n : nodes_) {
        points.push_back(n.position);
    }
    renderer_->renderDebugPoints(points);
}

void MemoryGardenDemo::cleanup() {
    nodes_.clear();
}

void MemoryGardenDemo::handleKeyboard(unsigned char) {}
void MemoryGardenDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
