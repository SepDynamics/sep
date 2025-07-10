#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "quantum/data.hpp"
#include "quantum/evolution.h"
#include "memory/types.h"

using namespace sep;
using namespace sep::pattern;
using namespace sep::quantum;

int main() {
    const int grid = 10;
    std::vector<PatternData> patterns;
    patterns.reserve(grid * grid);

    for (int x = 0; x < grid; ++x) {
        for (int y = 0; y < grid; ++y) {
            PatternData p;
            p.id = "p_" + std::to_string(x) + "_" + std::to_string(y);
            p.position = glm::vec4(static_cast<float>(x), static_cast<float>(y), 0.0f, 1.0f);
            p.coherence = 0.5f;
            p.stability = 0.5f;
            p.memory_tier = memory::MemoryTierEnum::STM;
            patterns.push_back(p);
        }
    }

    glm::vec3 center(grid / 2.0f, grid / 2.0f, 0.0f);

    for (int step = 0; step < 50; ++step) {
        for (auto &p : patterns) {
            evolution::applyGravity(p, center, 0.02f);
            evolution::randomPerturbation(p, 0.01f);

            if (glm::length(glm::vec3(p.position) - center) < 0.5f) {
                p.coherence = std::min(1.0f, p.coherence + 0.05f);
                p.stability = std::min(1.0f, p.stability + 0.05f);
                if (p.coherence > 0.9f && p.stability > 0.9f) {
                    p.memory_tier = memory::MemoryTierEnum::LTM;
                }
            }
        }
    }

    std::cout << "Stable LTM patterns:\n";
    for (const auto &p : patterns) {
        if (p.memory_tier == memory::MemoryTierEnum::LTM) {
            std::cout << p.id << " at (" << p.position.x << ", " << p.position.y << ", " << p.position.z << ")\n";
        }
    }

    return 0;
}
