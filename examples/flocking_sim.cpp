#include <iostream>
#include <vector>
#include <random>
#include <glm/glm.hpp>

#include "quantum/quantum_processor.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/data.hpp"

int main() {
    using sep::pattern::PatternData;
    using sep::memory::MemoryTierManager;

    // Quantum processor used for simple coherence calculations
    sep::quantum::QuantumProcessor::Config qcfg{};
    auto processor = sep::quantum::createQuantumProcessor(qcfg);
    MemoryTierManager memory_manager;

    const int agent_count = 20;
    std::vector<PatternData> agents(agent_count);

    std::default_random_engine rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> pos_dist(-10.f, 10.f);
    std::uniform_real_distribution<float> vel_dist(-1.f, 1.f);

    for (int i = 0; i < agent_count; ++i) {
        agents[i].id = std::to_string(i);
        agents[i].position = glm::vec4(pos_dist(rng), pos_dist(rng), pos_dist(rng), 1.0f);
        agents[i].velocity = glm::vec4(vel_dist(rng), vel_dist(rng), vel_dist(rng), 0.0f);
        memory_manager.registerPattern(i, agents[i]);
    }

    const float neighbor_radius = 5.0f;
    const float separation_distance = 1.0f;
    const float max_speed = 2.0f;

    for (int frame = 0; frame < 100; ++frame) {
        float total_coherence = 0.0f;
        for (int i = 0; i < agent_count; ++i) {
            auto& agent = agents[i];
            glm::vec3 pos(agent.position);
            glm::vec3 vel(agent.velocity);
            glm::vec3 cohesion(0.0f);
            glm::vec3 separation(0.0f);
            glm::vec3 alignment(0.0f);
            glm::vec3 avg_vel(0.0f);
            int neighbors = 0;

            for (int j = 0; j < agent_count; ++j) {
                if (i == j) continue;
                const auto& other = agents[j];
                glm::vec3 other_pos(other.position);
                float dist = glm::distance(pos, other_pos);
                if (dist < neighbor_radius) {
                    cohesion += other_pos;
                    avg_vel += glm::vec3(other.velocity);
                    ++neighbors;
                    if (dist < separation_distance) {
                        separation -= (other_pos - pos) / (dist + 0.01f);
                    }
                }
            }

            if (neighbors > 0) {
                cohesion = (cohesion / static_cast<float>(neighbors)) - pos;
                alignment = (avg_vel / static_cast<float>(neighbors)) - vel;
            }

            glm::vec3 accel = cohesion + separation + alignment;
            vel += accel * 0.05f;
            if (glm::length(vel) > max_speed) {
                vel = glm::normalize(vel) * max_speed;
            }
            pos += vel * 0.1f;

            agent.position = glm::vec4(pos, 1.0f);
            agent.velocity = glm::vec4(vel, 0.0f);

            // Simple coherence metric based on velocity alignment
            glm::vec3 alignment_ref = neighbors > 0 ? avg_vel / static_cast<float>(neighbors) : vel;
            agent.coherence = processor->calculateCoherence(vel, alignment_ref);
            agent.stability = processor->calculateStability(agent.coherence, agent.stability,
                                                            static_cast<float>(agent.generation), 1.0f);
            memory_manager.registerPattern(i, agent);
            total_coherence += agent.coherence;
        }
        std::cout << "Frame " << frame << " average coherence: "
                  << total_coherence / static_cast<float>(agent_count) << std::endl;
    }

    return 0;
}
