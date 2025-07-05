#include <iostream>
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include "quantum/quantum_processor.h"
#include "quantum/data.hpp"

// Simple cosmology simulation using SEP building blocks

int main() {
    using sep::pattern::PatternData;
    using sep::quantum::QuantumProcessor;

    const std::size_t parcel_count = 32;
    const float box_size = 50.0f;
    const float dt = 0.01f;
    const float G = 1.0f;
    const int steps = 100;

    std::mt19937 rng{1234u};
    std::uniform_real_distribution<float> dist(0.0f, box_size);

    std::vector<PatternData> parcels(parcel_count);
    for (std::size_t i = 0; i < parcel_count; ++i) {
        parcels[i].id = std::to_string(i);
        parcels[i].position = glm::vec4(dist(rng), dist(rng), dist(rng), 1.0f);
        parcels[i].velocity = glm::vec4(0.0f);
        parcels[i].attributes.x = 1.0f; // mass
    }

    QuantumProcessor processor({});
    for (std::size_t i = 0; i < parcels.size(); ++i) {
        processor.processPattern(glm::vec3(parcels[i].position), i);
    }

    auto integrate = [&](float timestep) {
        std::vector<glm::vec3> acc(parcels.size(), glm::vec3(0.0f));
        for (std::size_t i = 0; i < parcels.size(); ++i) {
            for (std::size_t j = i + 1; j < parcels.size(); ++j) {
                glm::vec3 diff = glm::vec3(parcels[j].position) - glm::vec3(parcels[i].position);
                float dist2 = glm::dot(diff, diff) + 1e-5f;
                glm::vec3 force = G * parcels[i].attributes.x * parcels[j].attributes.x * diff / (dist2 * glm::sqrt(dist2));
                acc[i] += force / parcels[i].attributes.x;
                acc[j] -= force / parcels[j].attributes.x;
            }
        }
        for (std::size_t i = 0; i < parcels.size(); ++i) {
            glm::vec3 vel = glm::vec3(parcels[i].velocity) + acc[i] * timestep;
            glm::vec3 pos = glm::vec3(parcels[i].position) + vel * timestep;
            parcels[i].velocity = glm::vec4(vel, 0.0f);
            parcels[i].position = glm::vec4(pos, 1.0f);
        }
    };

    for (int step = 0; step < steps; ++step) {
        integrate(dt);
        for (std::size_t i = 0; i < parcels.size(); ++i) {
            processor.updatePattern(i, glm::vec3(parcels[i].position));
        }

        float total_coh = 0.0f;
        int pair_cnt = 0;
        for (std::size_t i = 0; i < parcels.size(); ++i) {
            for (std::size_t j = i + 1; j < parcels.size(); ++j) {
                float c = processor.calculateCoherence(glm::vec3(parcels[i].position), glm::vec3(parcels[j].position));
                total_coh += c;
                ++pair_cnt;
            }
        }
        float avg_coh = pair_cnt ? total_coh / pair_cnt : 0.0f;
        std::cout << "Step " << step << ": average coherence " << avg_coh << "\n";
    }

    return 0;
}

