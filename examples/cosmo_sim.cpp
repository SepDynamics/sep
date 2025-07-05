#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <glm/glm.hpp>
#include "quantum/data.hpp"
#include "quantum/quantum_processor.h"

using sep::pattern::PatternData;
using sep::quantum::QuantumProcessor;
using sep::quantum::createQuantumProcessor;

struct CosmoSim {
    std::vector<PatternData> parcels;
    std::unique_ptr<QuantumProcessor> qproc;
    float box_size{50.0f};
    float time_step{0.01f};
    float G{1.0f};

    explicit CosmoSim(std::size_t count) {
        QuantumProcessor::Config cfg{};
        qproc = createQuantumProcessor(cfg);
        initParcels(count);
    }

    void initParcels(std::size_t count) {
        parcels.clear();
        parcels.resize(count);
        for (std::size_t i = 0; i < count; ++i) {
            auto &p = parcels[i];
            p.id = std::to_string(i);
            p.position = glm::vec4(random01() * box_size,
                                    random01() * box_size,
                                    random01() * box_size,
                                    1.0f);
            p.velocity = glm::vec4(0.0f);
            p.attributes.x = 1.0f; // mass
            p.coherence = 1.0f;
            qproc->processPattern(glm::vec3(p.position), i);
        }
    }

    static float random01() { return static_cast<float>(std::rand()) / RAND_MAX; }

    void step() {
        std::vector<glm::vec3> acc(parcels.size(), glm::vec3(0.0f));
        // Pairwise Newtonian gravity
        for (std::size_t i = 0; i < parcels.size(); ++i) {
            for (std::size_t j = i + 1; j < parcels.size(); ++j) {
                glm::vec3 diff = glm::vec3(parcels[j].position) - glm::vec3(parcels[i].position);
                float dist2 = glm::dot(diff, diff) + 1e-5f;
                glm::vec3 force = G * parcels[i].attributes.x * parcels[j].attributes.x /
                                  (dist2 * glm::sqrt(dist2)) * diff;
                acc[i] += force / parcels[i].attributes.x;
                acc[j] -= force / parcels[j].attributes.x;
            }
        }
        // Integrate velocities and positions
        for (std::size_t i = 0; i < parcels.size(); ++i) {
            glm::vec3 vel = glm::vec3(parcels[i].velocity) + acc[i] * time_step;
            glm::vec3 pos = glm::vec3(parcels[i].position) + vel * time_step;
            for (int k = 0; k < 3; ++k) {
                if (pos[k] < 0.0f) pos[k] += box_size;
                else if (pos[k] > box_size) pos[k] -= box_size;
            }
            parcels[i].velocity = glm::vec4(vel, 0.0f);
            parcels[i].position = glm::vec4(pos, 1.0f);
        }
        updateCoherence();
    }

    void updateCoherence() {
        glm::vec3 com(0.0f);
        float total_mass = 0.0f;
        for (const auto &p : parcels) {
            com += glm::vec3(p.position) * p.attributes.x;
            total_mass += p.attributes.x;
        }
        if (total_mass > 0.0f) com /= total_mass;

        for (std::size_t i = 0; i < parcels.size(); ++i) {
            glm::vec3 pos = glm::vec3(parcels[i].position);
            glm::vec3 vel = glm::vec3(parcels[i].velocity);
            float radius = glm::length(pos - com) + 1e-5f;
            float vesc2 = 2.f * G * total_mass / radius;
            float speed2 = glm::length2(vel);
            float bound = speed2 < vesc2 ? 1.0f : 0.0f;
            parcels[i].coherence = bound;
            qproc->updatePattern(i, glm::vec3(bound, radius, speed2));
        }
    }

    void run(std::size_t steps) {
        for (std::size_t s = 0; s < steps; ++s) {
            step();
            float avg = 0.0f;
            for (const auto &p : parcels) avg += p.coherence;
            avg /= static_cast<float>(parcels.size());
            std::cout << "Step " << s << ": average coherence " << avg << "\n";
        }
    }
};

int main(int argc, char *argv[]) {
    std::size_t parcel_count = 100;
    std::size_t steps = 100;
    if (argc > 1) parcel_count = static_cast<std::size_t>(std::atoi(argv[1]));
    if (argc > 2) steps = static_cast<std::size_t>(std::atoi(argv[2]));

    CosmoSim sim(parcel_count);
    sim.run(steps);
    return 0;
}

