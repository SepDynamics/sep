#include "flocking_demo.hpp"

#include <cstdlib>
#include <glm/glm.hpp>

#include "../simple_renderer.h"
#include "config.hpp"

namespace sep
{
    namespace workbench
    {

        void FlockingDemo::on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer)
        {
            (void)engine;
            renderer_ = renderer;
            const auto& cfg = Config::getInstance().flocking();
            std::size_t agent_count = cfg.agent_count;
            max_speed_ = cfg.max_speed;
            neighbor_radius_ = cfg.neighbor_radius;
            agents_.resize(agent_count);
            for (auto& a : agents_)
            {
                float px =
                    (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 20.f;
                float py =
                    (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 20.f;
                float pz =
                    (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 20.f;
                float vx =
                    (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2.f;
                float vy =
                    (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2.f;
                float vz =
                    (static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) - 0.5f) * 2.f;
                a.position = glm::vec4(px, py, pz, 0.f);
                a.velocity = glm::vec4(vx, vy, vz, 0.f);
            }
        }

        void FlockingDemo::on_update(float dt)
        {
            for (auto& agent : agents_)
            {
                glm::vec3 cohesion(0.f);
                glm::vec3 alignment(0.f);
                glm::vec3 separation(0.f);
                int neighbors = 0;
                for (const auto& other : agents_)
                {
                    if (&agent == &other) continue;
                    glm::vec3 diff = glm::vec3(other.position) - glm::vec3(agent.position);
                    float dist = glm::length(diff);
                    if (dist < neighbor_radius_)
                    {
                        cohesion += glm::vec3(other.position);
                        alignment += glm::vec3(other.velocity);
                        separation -= diff / (dist + 0.01f);
                        neighbors++;
                    }
                }
                if (neighbors > 0)
                {
                    cohesion = cohesion / static_cast<float>(neighbors) - glm::vec3(agent.position);
                    alignment =
                        alignment / static_cast<float>(neighbors) - glm::vec3(agent.velocity);
                }
                // Convert the vec3 sum to vec4 before adding it to agent.velocity
                glm::vec4 force = glm::vec4(cohesion + alignment + separation, 0.0f) * 0.01f;
                agent.velocity += force;
                float velocity_magnitude = glm::length(glm::vec3(agent.velocity));
                if (velocity_magnitude > max_speed_)
                {
                    glm::vec3 normalized_velocity =
                        glm::normalize(glm::vec3(agent.velocity)) * max_speed_;
                    agent.velocity = glm::vec4(normalized_velocity, agent.velocity.w);
                }
                agent.position += glm::vec4(glm::vec3(agent.velocity) * dt, 0.f);
            }
        }

        void FlockingDemo::on_render()
        {
            if (!renderer_) return;
            std::vector<glm::vec3> points;
            points.reserve(agents_.size());
            for (const auto& a : agents_)
            {
                points.emplace_back(a.position.x, a.position.y, a.position.z);
            }
            renderer_->renderPatternState(points);
        }

        void FlockingDemo::on_unload() { agents_.clear(); }

        void FlockingDemo::on_ui_render()
        {
            ImGui::Begin("Flocking Demo Controls");
            ImGui::SliderFloat("Max Speed", &max_speed_, 0.1f, 5.0f);
            ImGui::SliderFloat("Neighbor Radius", &neighbor_radius_, 1.0f, 20.0f);
            ImGui::Text("Agents: %zu", agents_.size());
            ImGui::End();
        }

        void FlockingDemo::on_key_press(int key) { (void)key; }
        void FlockingDemo::on_mouse(int, int, int) {}

    }  // namespace workbench
}  // namespace sep
