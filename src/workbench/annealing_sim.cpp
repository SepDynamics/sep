#include "sep_engine_wrapper.h"
#include "quantum/qfh.h"
#include "quantum/quantum_processor.h"
#include <glm/gtc/random.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

int main() {
    const std::size_t particle_count = 20;
    std::vector<sep::pattern::PatternData> particles(particle_count);

    for (auto &p : particles) {
        glm::vec3 pos = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
        p.position = glm::vec4(pos, 1.0f);
        p.velocity = glm::vec4(0.0f);
        p.attributes.x = 1.0f; // mass
    }

    sep::quantum::QuantumProcessor::Config cfg;
    auto processor = sep::quantum::createQuantumProcessor(cfg);
    sep::quantum::QFHBasedProcessor qfh_analyzer;

    const float dt = 0.01f;
    const float eps = 1e-3f;
    for (int step = 0; step < 100; ++step) {
        for (std::size_t i = 0; i < particles.size(); ++i) {
            for (std::size_t j = i + 1; j < particles.size(); ++j) {
                glm::vec3 diff = glm::vec3(particles[j].position) - glm::vec3(particles[i].position);
                float dist = glm::length(diff) + eps;
                glm::vec3 dir = diff / dist;
                float invDist6 = 1.0f / std::pow(dist, 6);
                float fmag = 24.0f * invDist6 * (2.0f * invDist6 - 1.0f) / dist;
                glm::vec3 force = dir * fmag;
                particles[i].velocity += glm::vec4(force * dt, 0.0f);
                particles[j].velocity -= glm::vec4(force * dt, 0.0f);
            }
        }

        for (auto &p : particles) {
            p.position += p.velocity * dt;
            glm::vec3 data = glm::vec3(p.position);
            processor->processPattern(data, &p - particles.data());
        }

        std::vector<uint32_t> raw_bits;
        raw_bits.reserve(particles.size() * 3);
        for (const auto &p : particles) {
            uint32_t bx, by, bz;
            std::memcpy(&bx, &p.position.x, sizeof(uint32_t));
            std::memcpy(&by, &p.position.y, sizeof(uint32_t));
            std::memcpy(&bz, &p.position.z, sizeof(uint32_t));
            raw_bits.push_back(bx);
            raw_bits.push_back(by);
            raw_bits.push_back(bz);
        }
        auto bits = sep::quantum::QFHBasedProcessor::convertToBits(raw_bits);
        auto result = qfh_analyzer.analyze(bits);

        for (const auto &ev : result.events) {
            if (ev.state == sep::quantum::QFHState::RUPTURE) {
                std::cout << "[step " << step << "] rupture at index " << ev.index << "\n";
            }
        }
        if (result.collapse_detected) {
            std::cout << "[step " << step << "] collapse detected (ratio=" << result.rupture_ratio << ")\n";
        }
    }

    return 0;
}

