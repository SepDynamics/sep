#include "sep_engine_wrapper.h"
#include "quantum/quantum_processor_qfh.h"
#include <glm/gtx/random.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace sep;

struct Particle {
    pattern::PatternData data;
};

int main() {
    const int particleCount = 32;
    const int steps = 100;
    const float dt = 0.01f;
    const float eps = 1e-3f;
    const float sigma = 1.0f;
    const float epsilon = 1.0f;

    std::vector<Particle> particles(particleCount);
    for (auto &p : particles) {
        glm::vec3 pos = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
        p.data.position = glm::vec4(pos, 1.0f);
        p.data.velocity = glm::vec4(0.0f);
    }

    quantum::QuantumProcessorQFH processor;

    for (int step = 0; step < steps; ++step) {
        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                glm::vec3 diff = glm::vec3(particles[j].data.position) - glm::vec3(particles[i].data.position);
                float dist = glm::length(diff) + eps;
                glm::vec3 dir = diff / dist;
                float inv6 = std::pow(sigma / dist, 6);
                float fmag = 24.f * epsilon * inv6 * (2.f * inv6 - 1.f) / dist;
                glm::vec3 force = dir * fmag;
                particles[i].data.velocity += glm::vec4(force * dt, 0.0f);
                particles[j].data.velocity -= glm::vec4(force * dt, 0.0f);
            }
        }

        for (auto &p : particles) {
            p.data.position += p.data.velocity * dt;
        }

        float avg_coherence = 0.f;
        std::vector<uint32_t> bits;
        bits.reserve(particles.size());
        for (const auto &p : particles) {
            glm::vec3 pos = glm::vec3(p.data.position);
            avg_coherence += processor.processPattern(pos);
            bits.push_back(pos.x > 0.f ? 1u : 0u);
        }
        avg_coherence /= static_cast<float>(particles.size());
        processor.processPatternBits(bits);
        auto qfh = processor.getLastQFHResult();

        if (qfh.rupture_ratio > 0.5f) {
            std::cout << "Step " << step << ": rupture detected (ratio=" << qfh.rupture_ratio << ")\n";
        }

        if (step % 10 == 0) {
            std::cout << "step " << step << " avg coherence=" << avg_coherence
                      << " flip_ratio=" << qfh.flip_ratio
                      << " rupture_ratio=" << qfh.rupture_ratio << "\n";
        }
    }

    return 0;
}
