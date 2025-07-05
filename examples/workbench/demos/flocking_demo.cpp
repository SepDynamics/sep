#include "flocking_demo.hpp"
#include <config.hpp>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

namespace sep {
namespace workbench {

void FlockingDemo::init() {
#ifdef SEP_WORKBENCH_DEMO
    // Default parameters
    cohesion_weight_ = 1.0f;
    separation_weight_ = 1.2f;
    alignment_weight_ = 1.0f;
    neighbor_radius_ = 5.0f;
    max_speed_ = 1.0f;
    initializeAgents();
#else
    const auto& cfg = Config::getInstance().flocking();
    cohesion_weight_ = cfg.cohesion_weight;
    separation_weight_ = cfg.separation_weight;
    alignment_weight_ = cfg.alignment_weight;
    neighbor_radius_ = cfg.neighbor_radius;
    max_speed_ = cfg.max_speed;
    agents_.resize(cfg.agent_count);
    initializeAgents();
#endif
}

void FlockingDemo::initializeAgents() {
    if (agents_.empty()) {
#ifdef SEP_WORKBENCH_DEMO
        agents_.resize(50);
#endif
    }
    for (auto& a : agents_) {
        a.position = glm::linearRand(glm::vec3(-10.0f), glm::vec3(10.0f));
        a.velocity = glm::sphericalRand(1.0f);
    }
}

void FlockingDemo::update(float dt) {
    for (size_t i = 0; i < agents_.size(); ++i) {
        glm::vec3 cohesion{0.0f};
        glm::vec3 separation{0.0f};
        glm::vec3 alignment{0.0f};
        int count = 0;
        for (size_t j = 0; j < agents_.size(); ++j) {
            if (i == j) continue;
            glm::vec3 diff = agents_[j].position - agents_[i].position;
            float dist = glm::length(diff);
            if (dist < neighbor_radius_) {
                cohesion += agents_[j].position;
                alignment += agents_[j].velocity;
                separation -= diff / (dist + 0.001f);
                ++count;
            }
        }
        if (count > 0) {
            cohesion = (cohesion / static_cast<float>(count)) - agents_[i].position;
            alignment = (alignment / static_cast<float>(count)) - agents_[i].velocity;
        }
        agents_[i].velocity += cohesion * cohesion_weight_ +
                               separation * separation_weight_ +
                               alignment * alignment_weight_;
        float speed = glm::length(agents_[i].velocity);
        if (speed > max_speed_) {
            agents_[i].velocity = (agents_[i].velocity / speed) * max_speed_;
        }
    }

    for (auto& a : agents_) {
        a.position += a.velocity * dt;
    }
}

void FlockingDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& a : agents_) {
        points.push_back(a.position);
    }
    renderer_->renderPatternState(points);
}

void FlockingDemo::cleanup() {
    agents_.clear();
}

void FlockingDemo::handleKeyboard(unsigned char) {}
void FlockingDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
