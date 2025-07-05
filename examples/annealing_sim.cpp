#include <iostream>
#include <vector>
#include <string>
#include <glm/gtc/random.hpp>
#include <glm/glm.hpp>
#include "quantum/quantum_processor.h"
#include "quantum/quantum_processor_qfh.h"
#include "quantum/data.hpp"

using sep::pattern::PatternData;
using sep::quantum::QuantumProcessor;
using sep::quantum::QuantumProcessorQFH;
using sep::quantum::QFHState;

struct Particle {
    PatternData data;
};

int main() {
    constexpr std::size_t count = 16;
    constexpr float dt = 0.01f;
    constexpr float sigma = 1.0f;
    constexpr float epsilon = 1.0f;
    constexpr float eps = 1e-6f;

    std::vector<Particle> particles(count);
    for (std::size_t i = 0; i < count; ++i) {
        particles[i].data.id = std::to_string(i);
        glm::vec3 p = glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f));
        particles[i].data.position = glm::vec4(p, 0.f);
        particles[i].data.velocity = glm::vec4(0.f);
    }

    QuantumProcessor qp({});
    QuantumProcessorQFH qfh;

    for (int step = 0; step < 200; ++step) {
        std::vector<uint32_t> rupture_bits;
        for (std::size_t i = 0; i < count; ++i) {
            for (std::size_t j = i + 1; j < count; ++j) {
                glm::vec3 diff = glm::vec3(particles[j].data.position) - glm::vec3(particles[i].data.position);
                float r = glm::length(diff) + eps;
                glm::vec3 dir = diff / r;
                float sr6 = powf(sigma / r, 6.0f);
                float force_mag = 24.f * epsilon * sr6 * (2.f * sr6 - 1.f) / r;
                glm::vec3 f = dir * force_mag;
                particles[i].data.velocity += glm::vec4(f * dt, 0.f);
                particles[j].data.velocity -= glm::vec4(f * dt, 0.f);
                bool rupture = r > 2.5f;
                rupture_bits.push_back(rupture ? 1u : 0u);
            }
        }

        for (auto& p : particles) {
            p.data.position += p.data.velocity * dt;
            qp.processPattern(glm::vec3(p.data.position), std::stoul(p.data.id));
        }

        qfh.processPatternBits(rupture_bits);
        const auto& result = qfh.getLastQFHResult();
        if (result.rupture_count > 0) {
            std::cout << "[step " << step << "] " << result.rupture_count << " ruptures detected\n";
        }
    }

    return 0;
}
