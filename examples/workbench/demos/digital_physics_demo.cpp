#include "digital_physics_demo.hpp"
#include <config.hpp>
#include <glm/vec3.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void DigitalPhysicsDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    width_ = 16;
    height_ = 16;
    rules_ = {3, 2, 3};
#else
    const auto& cfg = Config::getInstance().digital_physics();
    width_ = cfg.grid.width;
    height_ = cfg.grid.height;
    rules_.birth = cfg.rules.birth;
    rules_.survive_min = cfg.rules.survive_min;
    rules_.survive_max = cfg.rules.survive_max;
#endif
    grid_.resize(width_ * height_);
    for (auto& cell : grid_) {
        cell.alive = (std::rand() % 2) == 0;
        cell.data.coherence = cell.alive ? 1.f : 0.f;
    }
}

int DigitalPhysicsDemo::countAliveNeighbors(int x, int y) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = (x + dx + width_) % width_;
            int ny = (y + dy + height_) % height_;
            if (grid_[index(nx, ny)].alive) ++count;
        }
    }
    return count;
}

void DigitalPhysicsDemo::step() {
    std::vector<bool> next(grid_.size());
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int neighbors = countAliveNeighbors(x, y);
            bool alive = grid_[index(x, y)].alive;
            bool new_alive = false;
            if (alive) {
                new_alive = neighbors >= rules_.survive_min && neighbors <= rules_.survive_max;
            } else {
                new_alive = neighbors == rules_.birth;
            }
            next[index(x, y)] = new_alive;
        }
    }
    for (size_t i = 0; i < grid_.size(); ++i) {
        grid_[i].alive = next[i];
        grid_[i].data.coherence = next[i] ? 1.f : 0.f;
    }
}

void DigitalPhysicsDemo::update(float) {
    if (running_) {
        step();
    }
}

void DigitalPhysicsDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        std::vector<glm::vec3> points;
        for (int y = 0; y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                if (grid_[index(x, y)].alive) {
                    points.emplace_back(static_cast<float>(x), 0.f, static_cast<float>(y));
                }
            }
        }
        renderer_->renderPatternState(points);
    }
#else
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[index(x, y)].alive) {
                points.emplace_back(static_cast<float>(x), 0.f, static_cast<float>(y));
            }
        }
    }
    renderer_->renderPatternState(points);
#endif
}

void DigitalPhysicsDemo::cleanup() {
    grid_.clear();
}

void DigitalPhysicsDemo::handleKeyboard(unsigned char key) {
    switch (key) {
        case ' ': // toggle running
            running_ = !running_;
            break;
        case 'r': // randomize
            for (auto& cell : grid_) {
                cell.alive = (std::rand() % 2) == 0;
                cell.data.coherence = cell.alive ? 1.f : 0.f;
            }
            break;
    }
}

void DigitalPhysicsDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
