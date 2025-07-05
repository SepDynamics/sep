#include "sep_engine_wrapper.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>
#include <vector>
#include <random>

int main() {
    using sep::pattern::PatternData;
    const int agent_count = 30;
    std::vector<PatternData> agents;
    agents.reserve(agent_count);

    std::default_random_engine rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> pos(-10.f, 10.f);
    std::uniform_real_distribution<float> vel(-1.f, 1.f);

    for (int i = 0; i < agent_count; ++i) {
        PatternData agent;
        agent.position = glm::vec4(pos(rng), pos(rng), pos(rng), 1.0f);
        agent.velocity = glm::vec4(vel(rng), vel(rng), vel(rng), 0.0f);
        agent.coherence = 0.5f;
        agents.push_back(agent);
    }

    sep::quantum::QuantumProcessor::Config cfg;
    auto qproc = sep::quantum::createQuantumProcessor(cfg);
    sep::memory::MemoryTierManager mem_mgr;

    const float neighbor_radius = 5.0f;
    const float separation_distance = 1.0f;
    const float max_speed = 2.0f;
    const float dt = 0.1f;

    for (int step = 0; step < 100; ++step) {
        for (auto& agent : agents) {
            glm::vec3 pos(agent.position);
            glm::vec3 vel(agent.velocity);
            glm::vec3 cohesion(0.0f);
            glm::vec3 separation(0.0f);
            glm::vec3 alignment(0.0f);
            int neighbor_count = 0;

            for (const auto& other : agents) {
                if (&agent == &other) continue;
                glm::vec3 other_pos(other.position);
                float dist = glm::distance(pos, other_pos);
                if (dist < neighbor_radius) {
                    cohesion += other_pos;
                    alignment += glm::vec3(other.velocity);
                    ++neighbor_count;
                    if (dist < separation_distance) {
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
            if (glm::length2(vel) > max_speed * max_speed) {
                vel = glm::normalize(vel) * max_speed;
            }
            pos += vel * dt;

            agent.position = glm::vec4(pos, 1.0f);
            agent.velocity = glm::vec4(vel, 0.0f);
        }

        float coherence_sum = 0.0f;
        for (size_t i = 0; i < agents.size(); ++i) {
            auto pos = glm::vec3(agents[i].position);
            qproc->processPattern(pos, i);
            mem_mgr.registerPattern(i, agents[i]);
            coherence_sum += qproc->calculateCoherence(pos, glm::vec3(0.0f));
        }
        float avg_coherence = coherence_sum / agents.size();
        std::cout << "Step " << step << " average coherence: " << avg_coherence << std::endl;
    }

    return 0;
}

