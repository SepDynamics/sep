#include <iostream>
#include <vector>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "sep_engine_wrapper.h"
#include "quantum/quantum_processor.h"
#include "quantum/qfh.h"

using namespace sep;

struct Particle {
    pattern::PatternData data;
};

int main() {
    const std::size_t particle_count = 32;
    const float dt = 0.01f;
    const int steps = 200;

    quantum::QuantumProcessor::Config qcfg;
    auto processor = quantum::createQuantumProcessor(qcfg);
    quantum::QFHBasedProcessor qfh_proc;

    std::vector<Particle> particles(particle_count);
    for(std::size_t i=0;i<particle_count;++i) {
        particles[i].data.id = "p" + std::to_string(i);
        particles[i].data.position = glm::vec4(glm::linearRand(glm::vec3(-1.f), glm::vec3(1.f)), 1.f);
        particles[i].data.velocity = glm::vec4(0.f);
    }

    for(int step=0; step<steps; ++step) {
        // Lennard-Jones forces
        for(std::size_t i=0;i<particle_count;++i) {
            for(std::size_t j=i+1;j<particle_count;++j) {
                glm::vec3 diff = glm::vec3(particles[j].data.position) - glm::vec3(particles[i].data.position);
                float dist = glm::length(diff) + 1e-3f;
                glm::vec3 dir = diff / dist;
                float inv6 = 1.f / std::pow(dist, 6);
                float fmag = 24.f * inv6 * (2.f*inv6 - 1.f) / dist;
                glm::vec3 f = dir * fmag;
                particles[i].data.velocity += glm::vec4(f*dt,0.f);
                particles[j].data.velocity -= glm::vec4(f*dt,0.f);
            }
        }
        for(auto& p : particles) {
            p.data.position += p.data.velocity * dt;
        }

        // Process with QuantumProcessor
        for(std::size_t i=0;i<particle_count;++i) {
            glm::vec3 pos = glm::vec3(particles[i].data.position);
            processor->processPattern(pos, i);
        }

        // QFH analysis on raw position bits
        std::vector<uint32_t> vals;
        vals.reserve(particle_count*3);
        for(const auto& p : particles) {
            uint32_t bx,by,bz;
            std::memcpy(&bx, &p.data.position.x, sizeof(uint32_t));
            std::memcpy(&by, &p.data.position.y, sizeof(uint32_t));
            std::memcpy(&bz, &p.data.position.z, sizeof(uint32_t));
            vals.push_back(bx); vals.push_back(by); vals.push_back(bz);
        }
        quantum::QFHResult res = qfh_proc.analyze(quantum::QFHBasedProcessor::convertToBits(vals));
        if(res.rupture_count > 0) {
            std::cout << "step " << step << ": ruptures=" << res.rupture_count
                      << " ratio=" << res.rupture_ratio << std::endl;
        }
    }

    std::cout << "Simulation finished" << std::endl;
    return 0;
}

