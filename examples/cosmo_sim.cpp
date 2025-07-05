#include <iostream>
#include <vector>
#include <random>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <quantum/data.hpp>
#include <quantum/quantum_processor.h>

// Simple cosmological simulation using PatternData and QuantumProcessor

int main() {
    const std::size_t body_count = 20;
    const float box_size = 50.0f;
    const float G = 1.0f;
    const float dt = 0.01f;

    std::vector<sep::pattern::PatternData> bodies(body_count);
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> pos_dist(0.0f, box_size);
    std::uniform_real_distribution<float> vel_dist(-0.5f, 0.5f);

    for (std::size_t i = 0; i < body_count; ++i) {
        bodies[i].id = std::to_string(i);
        bodies[i].position = glm::vec4(pos_dist(rng), pos_dist(rng), pos_dist(rng), 1.0f);
        bodies[i].velocity = glm::vec4(vel_dist(rng), vel_dist(rng), vel_dist(rng), 0.0f);
        bodies[i].attributes.x = 1.0f; // mass
    }

    sep::quantum::QuantumProcessor::Config cfg;
    sep::quantum::QuantumProcessor processor(cfg);

    const int steps = 100;
    for (int step = 0; step < steps; ++step) {
        // Gravitational integration
        std::vector<glm::vec3> acc(body_count, glm::vec3(0.0f));
        for (std::size_t i = 0; i < body_count; ++i) {
            for (std::size_t j = i + 1; j < body_count; ++j) {
                glm::vec3 diff = glm::vec3(bodies[j].position) - glm::vec3(bodies[i].position);
                float dist2 = glm::dot(diff, diff) + 1e-5f;
                float invDist3 = 1.0f / (std::sqrt(dist2) * dist2);
                float m1 = bodies[i].attributes.x;
                float m2 = bodies[j].attributes.x;
                glm::vec3 force = G * m1 * m2 * invDist3 * diff;
                acc[i] += force / m1;
                acc[j] -= force / m2;
            }
        }
        for (std::size_t i = 0; i < body_count; ++i) {
            glm::vec3 vel = glm::vec3(bodies[i].velocity) + acc[i] * dt;
            glm::vec3 pos = glm::vec3(bodies[i].position) + vel * dt;
            bodies[i].velocity = glm::vec4(vel, 0.0f);
            bodies[i].position = glm::vec4(pos, 1.0f);
        }

        // Coherence statistics
        float total_coherence = 0.0f;
        int bound_pairs = 0;
        int pair_count = 0;
        for (std::size_t i = 0; i < body_count; ++i) {
            for (std::size_t j = i + 1; j < body_count; ++j) {
                float coherence = processor.calculateCoherence(
                    glm::vec3(bodies[i].position), glm::vec3(bodies[j].position));
                total_coherence += coherence;
                ++pair_count;

                glm::vec3 diff = glm::vec3(bodies[j].position) - glm::vec3(bodies[i].position);
                float dist = glm::length(diff) + 1e-5f;
                glm::vec3 rel_vel = glm::vec3(bodies[j].velocity) - glm::vec3(bodies[i].velocity);
                float kinetic = 0.5f * (bodies[i].attributes.x + bodies[j].attributes.x) * glm::length2(rel_vel);
                float potential = -G * bodies[i].attributes.x * bodies[j].attributes.x / dist;
                if (kinetic + potential < 0.0f) bound_pairs++;
            }
        }

        float avg_coherence = pair_count > 0 ? total_coherence / pair_count : 0.0f;
        float bound_fraction = pair_count > 0 ? static_cast<float>(bound_pairs) / pair_count : 0.0f;
        std::cout << "Step " << step
                  << ": avg coherence=" << avg_coherence
                  << ", bound fraction=" << bound_fraction << std::endl;
    }
    return 0;
}

