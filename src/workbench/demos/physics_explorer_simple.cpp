#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "../../workbench_demo_adapter.hpp"
#include "memory/types.h"
#include "quantum/data.hpp"
#include "quantum/evolution.h"

using namespace sep;
using namespace sep::quantum;

int main() {
    const int grid = 10;
    std::vector<Pattern> patterns;
    patterns.reserve(grid * grid);

    for (int x = 0; x < grid; ++x) {
        for (int y = 0; y < grid; ++y) {
            Pattern p;
            p.id = "p_" + std::to_string(x) + "_" + std::to_string(y);
            p.position = glm::vec4(static_cast<float>(x), static_cast<float>(y), 0.0f, 1.0f);
            p.quantum_state.coherence = 0.5f;
            p.quantum_state.stability = 0.5f;
            p.quantum_state.memory_tier = memory::MemoryTierEnum::STM;
            patterns.push_back(p);
        }
    }

    glm::vec3 center(grid / 2.0f, grid / 2.0f, 0.0f);

    for (int step = 0; step < 50; ++step) {
        for (auto &p : patterns) {
            quantum::Pattern q;
            q.id = p.id;
            q.position = p.position;
            q.momentum = glm::vec3(0.0f);
            q.quantum_state.generation = p.quantum_state.generation;
            q.quantum_state.state = quantum::QuantumState::Status::SUPERPOSITION;

            evolution::applyGravity(q, center, 0.02f);
            evolution::randomPerturbation(q, 0.01f);

            p.position = q.position;

            if (glm::length(glm::vec3(p.position) - center) < 0.5f) {
                p.quantum_state.coherence = std::min(1.0f, p.quantum_state.coherence + 0.05f);
                p.quantum_state.stability = std::min(1.0f, p.quantum_state.stability + 0.05f);
                if (p.quantum_state.coherence > 0.9f && p.quantum_state.stability > 0.9f) {
                    p.quantum_state.memory_tier = memory::MemoryTierEnum::LTM;
                }
            }
        }
    }

    std::cout << "Stable LTM patterns:\n";
    for (const auto &p : patterns) {
        if (p.quantum_state.memory_tier == memory::MemoryTierEnum::LTM) {
            std::cout << p.id << " at (" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")\n";
        }
    }

    return 0;
}
