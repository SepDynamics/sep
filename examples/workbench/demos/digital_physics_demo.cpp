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
    birth_rules_ = {3};
    survival_rules_ = {2,3};
#else
    const auto& cfg = getConfigManager().getEngineConfig().digital_physics();
    width_ = static_cast<std::size_t>(cfg.grid.width);
    height_ = static_cast<std::size_t>(cfg.grid.height);
    birth_rules_ = cfg.rules.birth;
    survival_rules_ = cfg.rules.survival;
#endif

    grid_.resize(width_ * height_);
    for (std::size_t y = 0; y < height_; ++y) {
        for (std::size_t x = 0; x < width_; ++x) {
            auto& cell = grid_[index(x, y)];
            cell.position = glm::vec4(static_cast<float>(x), 0.f, static_cast<float>(y), 1.f);
            cell.attributes.x = static_cast<float>(std::rand() % 2);
        }
    }
}

void DigitalPhysicsDemo::update(float) {
    std::vector<pattern::PatternData> next = grid_;

    auto alive = [&](int x, int y) -> bool {
        if (x < 0) x += static_cast<int>(width_);
        if (y < 0) y += static_cast<int>(height_);
        if (x >= static_cast<int>(width_)) x -= static_cast<int>(width_);
        if (y >= static_cast<int>(height_)) y -= static_cast<int>(height_);
        return grid_[index(static_cast<std::size_t>(x), static_cast<std::size_t>(y))].attributes.x > 0.5f;
    };

    for (std::size_t y = 0; y < height_; ++y) {
        for (std::size_t x = 0; x < width_; ++x) {
            int count = 0;
            for (int dy = -1; dy <= 1; ++dy)
                for (int dx = -1; dx <= 1; ++dx)
                    if (dx || dy)
                        count += alive(static_cast<int>(x) + dx, static_cast<int>(y) + dy) ? 1 : 0;

            bool current = grid_[index(x, y)].attributes.x > 0.5f;
            bool next_state;
            if (current) {
                next_state = std::find(survival_rules_.begin(), survival_rules_.end(), count) != survival_rules_.end();
            } else {
                next_state = std::find(birth_rules_.begin(), birth_rules_.end(), count) != birth_rules_.end();
            }
            next[index(x, y)].attributes.x = next_state ? 1.0f : 0.0f;
        }
    }

    grid_.swap(next);
}

void DigitalPhysicsDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    points.reserve(width_ * height_);
    for (const auto& cell : grid_) {
        if (cell.attributes.x > 0.5f) {
            points.emplace_back(cell.position.x, cell.position.y, cell.position.z);
        }
    }
    renderer_->renderPatternState(points);
}

void DigitalPhysicsDemo::cleanup() {
    grid_.clear();
}

void DigitalPhysicsDemo::handleKeyboard(unsigned char key) {
    if (key == 'r') {
        for (auto& cell : grid_) {
            cell.attributes.x = static_cast<float>(std::rand() % 2);
        }
    }
}

void DigitalPhysicsDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
