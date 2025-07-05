#include "digital_physics_demo.hpp"
#include <config.hpp>
#include <glm/vec3.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DigitalPhysicsDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    width_ = 20;
    height_ = 20;
    birth_ = {3};
    survive_ = {2, 3};
#else
    const auto& cfg = sep::core::config::ConfigManager::getInstance().getEngineConfig().digital_physics();
    width_ = cfg.grid.width;
    height_ = cfg.grid.height;
    birth_ = cfg.rules.birth;
    survive_ = cfg.rules.survive;
#endif

    grid_.resize(width_ * height_);
    next_.resize(width_ * height_);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            grid_[index(x, y)].active = (std::rand() % 2) == 0;
            grid_[index(x, y)].data.position = glm::vec4(x, 0.f, y, 1.f);
        }
    }
}

int DigitalPhysicsDemo::countNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + width_) % width_;
            int ny = (y + dy + height_) % height_;
            if (grid_[index(nx, ny)].active)
                ++count;
        }
    }
    return count;
}

void DigitalPhysicsDemo::step() {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int neighbors = countNeighbors(x, y);
            bool alive = grid_[index(x, y)].active;
            bool new_state = false;
            if (alive) {
                new_state = std::find(survive_.begin(), survive_.end(), neighbors) != survive_.end();
            } else {
                new_state = std::find(birth_.begin(), birth_.end(), neighbors) != birth_.end();
            }
            next_[index(x, y)] = grid_[index(x, y)];
            next_[index(x, y)].active = new_state;
        }
    }
    grid_.swap(next_);
}

void DigitalPhysicsDemo::update(float) {
    step();
}

void DigitalPhysicsDemo::render() {
    if (!renderer_) return;

    std::vector<glm::vec3> points;
    points.reserve(width_ * height_);
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[index(x, y)].active) {
                points.emplace_back(static_cast<float>(x), 0.f, static_cast<float>(y));
            }
        }
    }

    renderer_->setColorMode("coherence");
    renderer_->renderPatternState(points);
}

void DigitalPhysicsDemo::cleanup() {
    grid_.clear();
    next_.clear();
}

void DigitalPhysicsDemo::handleKeyboard(unsigned char key) {
    if (key == 'r') {
        for (auto& c : grid_) {
            c.active = (std::rand() % 2) == 0;
        }
    }
}

void DigitalPhysicsDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
