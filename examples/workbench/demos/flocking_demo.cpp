#include "flocking_demo.hpp"
#include "sep_engine_wrapper.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>

namespace sep {
namespace workbench {

void FlockingDemo::init() {
    agents_.clear();
    std::default_random_engine rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> pos(-10.f, 10.f);
    std::uniform_real_distribution<float> vel(-1.f, 1.f);

    const int count = 50; // Default agent count
    for (int i = 0; i < count; ++i) {
        sep::pattern::PatternData agent;
        agent.position = glm::vec4(pos(rng), pos(rng), pos(rng), 1.0f);
        agent.velocity = glm::vec4(vel(rng), vel(rng), vel(rng), 0.0f);
        agents_.push_back(agent);
    }
}

void FlockingDemo::update(float dt) {
    for (auto& agent : agents_) {
        glm::vec3 pos(agent.position);
        glm::vec3 vel(agent.velocity);
        glm::vec3 cohesion(0.0f);
        glm::vec3 separation(0.0f);
        glm::vec3 alignment(0.0f);
        int neighbor_count = 0;

        for (const auto& other : agents_) {
            if (&agent == &other) continue;
            glm::vec3 other_pos(other.position);
            float dist = glm::distance(pos, other_pos);
            if (dist < neighbor_radius_) {
                cohesion += other_pos;
                alignment += glm::vec3(other.velocity);
                ++neighbor_count;
                if (dist < separation_distance_) {
                    separation -= (other_pos - pos) / (dist + 0.01f);
                }
            }
        }

        if (neighbor_count > 0) {
            cohesion = (cohesion / static_cast<float>(neighbor_count)) - pos;
            alignment /= static_cast<float>(neighbor_count);
        }

        glm::vec3 accel = cohesion + separation + alignment;
        vel += accel * dt;
        if (glm::length2(vel) > max_speed_ * max_speed_) {
            vel = glm::normalize(vel) * max_speed_;
        }
        pos += vel * dt;

        agent.position = glm::vec4(pos, 1.0f);
        agent.velocity = glm::vec4(vel, 0.0f);
    }
}

void FlockingDemo::render() {
    if (!renderer_) return;
    std::vector<glm::vec3> points;
    for (const auto& a : agents_) {
        points.emplace_back(a.position.x, a.position.y, a.position.z);
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
