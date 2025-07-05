#include "flocking_demo.hpp"
#include <glm/gtc/constants.hpp>
#include <cstdlib>

namespace sep {
namespace workbench {

void FlockingDemo::init() {
    agents_.resize(50);
    for (auto &a : agents_) {
        a.position = glm::vec3(
            static_cast<float>(std::rand() % 100) / 100.0f,
            0.0f,
            static_cast<float>(std::rand() % 100) / 100.0f);
        a.velocity = glm::vec3(
            static_cast<float>(std::rand() % 100 - 50) / 500.0f,
            0.0f,
            static_cast<float>(std::rand() % 100 - 50) / 500.0f);
    }
}

void FlockingDemo::update(float dt) {
    const float neighbor_radius = 0.1f;
    const float separation_radius = 0.05f;

    for (std::size_t i = 0; i < agents_.size(); ++i) {
        glm::vec3 cohesion(0.0f);
        glm::vec3 separation(0.0f);
        glm::vec3 alignment(0.0f);
        int neighbor_count = 0;

        for (std::size_t j = 0; j < agents_.size(); ++j) {
            if (i == j) continue;
            glm::vec3 diff = agents_[j].position - agents_[i].position;
            float dist = glm::length(diff);
            if (dist < neighbor_radius && dist > 0.0001f) {
                cohesion += agents_[j].position;
                alignment += agents_[j].velocity;
                if (dist < separation_radius) {
                    separation -= diff / dist;
                }
                neighbor_count++;
            }
        }

        if (neighbor_count > 0) {
            cohesion = (cohesion / static_cast<float>(neighbor_count) - agents_[i].position) * cohesion_weight_;
            alignment = (alignment / static_cast<float>(neighbor_count) - agents_[i].velocity) * alignment_weight_;
        }

        separation *= separation_weight_;
        agents_[i].velocity += (cohesion + alignment + separation) * dt;
    }

    for (auto &a : agents_) {
        a.position += a.velocity * dt;
    }
}

void FlockingDemo::render() {
#ifdef SEP_WORKBENCH_DEMO
    if (renderer_) {
        std::vector<glm::vec3> points;
        for (const auto &a : agents_) points.push_back(a.position);
        renderer_->renderPatternState(points);
    }
#endif
}

void FlockingDemo::cleanup() {
    agents_.clear();
}

void FlockingDemo::handleKeyboard(unsigned char key) {
    if (key == ' ') {
        // reset positions
        init();
    }
}

void FlockingDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
