#include <iostream>
#include <vector>
#include <random>
#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>

#include "quantum/quantum_processor.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/data.hpp"

int main() {
    constexpr int kAgentCount = 32;
    constexpr float neighborRadius = 1.5f;
    constexpr float separationDist = 0.2f;
    constexpr float maxSpeed = 1.0f;
    constexpr float dt = 0.05f;

    std::vector<sep::pattern::PatternData> agents(kAgentCount);
    std::default_random_engine rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    for (int i = 0; i < kAgentCount; ++i) {
        agents[i].id = std::to_string(i);
        agents[i].position = glm::vec4(dist(rng), dist(rng), dist(rng), 1.0f);
        agents[i].velocity = glm::vec4(dist(rng) * 0.1f, dist(rng) * 0.1f, dist(rng) * 0.1f, 0.0f);
    }

    auto qp = sep::quantum::createQuantumProcessor({});
    auto& mem = sep::memory::MemoryTierManager::getInstance();

    for (int i = 0; i < kAgentCount; ++i) {
        mem.registerPattern(i, agents[i]);
        qp->processPattern(glm::vec3(agents[i].position), i);
    }

    for (int frame = 0; frame < 100; ++frame) {
        for (int i = 0; i < kAgentCount; ++i) {
            auto& a = agents[i];
            glm::vec3 pos(a.position);
            glm::vec3 vel(a.velocity);
            glm::vec3 coh(0.0f), sep(0.0f), ali(0.0f);
            int count = 0;
            for (int j = 0; j < kAgentCount; ++j) {
                if (i == j) continue;
                const auto& b = agents[j];
                glm::vec3 bpos(b.position);
                float dist2 = glm::length2(bpos - pos);
                float dist = std::sqrt(dist2);
                if (dist < neighborRadius) {
                    coh += bpos;
                    ali += glm::vec3(b.velocity);
                    ++count;
                    if (dist < separationDist) {
                        sep -= (bpos - pos) / (dist + 0.001f);
                    }
                }
            }
            if (count > 0) {
                coh = coh / static_cast<float>(count) - pos;
                ali /= static_cast<float>(count);
            }
            glm::vec3 accel = coh + sep + ali;
            vel += accel * dt;
            if (glm::length2(vel) > maxSpeed * maxSpeed)
                vel = glm::normalize(vel) * maxSpeed;
            pos += vel * dt;
            a.position = glm::vec4(pos, 1.0f);
            a.velocity = glm::vec4(vel, 0.0f);
            qp->updatePattern(i, glm::vec3(a.position));
            mem.registerPattern(i, a);
        }
        float total_coh = 0.0f;
        int pairs = 0;
        for (int i = 0; i < kAgentCount; ++i)
            for (int j = i + 1; j < kAgentCount; ++j) {
                total_coh += qp->calculateCoherence(glm::vec3(agents[i].position), glm::vec3(agents[j].position));
                ++pairs;
            }
        std::cout << "Frame " << frame << ": avg coherence = " << (pairs ? total_coh / pairs : 0.0f) << std::endl;
    }
    return 0;
}
