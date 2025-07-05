#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <glm/vec4.hpp>
#include "quantum/evolution.h"
#include "quantum/types.h"
#include "quantum/data.hpp"
#include "memory/types.h"

using namespace sep;

int main() {
    const int width = 10;
    const int height = 10;

    std::vector<pattern::PatternData> grid(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto& cell = grid[y * width + x];
            cell.id = shim::string(("cell_" + std::to_string(x) + "_" + std::to_string(y)).c_str());
            cell.position = glm::vec4(static_cast<float>(x) / width,
                                      0.0f,
                                      static_cast<float>(y) / height,
                                      1.0f);
            cell.coherence = static_cast<float>(std::rand()) / RAND_MAX;
            cell.stability = static_cast<float>(std::rand()) / RAND_MAX;
        }
    }

    // Convert to quantum::Pattern for rule application
    std::vector<quantum::Pattern> patterns(grid.size());
    for (size_t i = 0; i < grid.size(); ++i) {
        auto& src = grid[i];
        auto& dst = patterns[i];
        dst.id = src.id.c_str();
        dst.position = src.position;
        dst.quantum_state.coherence = src.coherence;
        dst.quantum_state.stability = src.stability;
        dst.quantum_state.memory_tier = src.memory_tier;
    }

    const int iterations = 10;
    for (int iter = 0; iter < iterations; ++iter) {
        // Apply gaussian mutation to explore new states
        for (auto& p : patterns) {
            p = quantum::evolution::gaussianMutation(p, 0.05f);
        }

        // Hebbian-like update among neighbors
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = y * width + x;
                auto& cell = patterns[idx];
                if (x + 1 < width) {
                    quantum::evolution::hebbianUpdate(cell, patterns[y * width + x + 1], 0.01f);
                }
                if (y + 1 < height) {
                    quantum::evolution::hebbianUpdate(cell, patterns[(y + 1) * width + x], 0.01f);
                }
            }
        }

        // Promote stable patterns to LTM
        for (auto& p : patterns) {
            float c = p.quantum_state.coherence;
            float s = p.quantum_state.stability;
            if (c > 0.7f && s > 0.7f) {
                p.quantum_state.memory_tier = memory::MemoryTierEnum::LTM;
            }
        }
    }

    std::cout << "Stable LTM patterns:\n";
    for (const auto& p : patterns) {
        if (p.quantum_state.memory_tier == memory::MemoryTierEnum::LTM) {
            std::cout << p.id << " coherence:" << p.quantum_state.coherence
                      << " stability:" << p.quantum_state.stability << "\n";
        }
    }
    return 0;
}
