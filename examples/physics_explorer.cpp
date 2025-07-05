#include <iostream>
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include "sep_engine_wrapper.h"
#include "quantum/evolution.h"
#include "quantum/types.h"
#include "quantum/data.hpp"
#include "memory/types.h"

using namespace sep;

static quantum::Pattern toPattern(const pattern::PatternData& d) {
    quantum::Pattern p;
    p.id = d.id;
    p.position = d.position;
    p.quantum_state.coherence = d.coherence;
    p.quantum_state.stability = d.stability;
    p.quantum_state.entropy = d.entropy;
    p.quantum_state.phase = d.phase;
    p.quantum_state.mutation_rate = d.mutation_rate;
    p.quantum_state.generation = d.generation;
    p.quantum_state.mutation_count = d.mutation_count;
    p.quantum_state.memory_tier = d.memory_tier;
    p.quantum_state.state = d.state;
    return p;
}

static void fromPattern(const quantum::Pattern& p, pattern::PatternData& d) {
    d.id = p.id;
    d.position = p.position;
    d.coherence = p.quantum_state.coherence;
    d.stability = p.quantum_state.stability;
    d.entropy = p.quantum_state.entropy;
    d.phase = p.quantum_state.phase;
    d.mutation_rate = p.quantum_state.mutation_rate;
    d.generation = p.quantum_state.generation;
    d.mutation_count = p.quantum_state.mutation_count;
    d.memory_tier = p.quantum_state.memory_tier;
    d.state = p.quantum_state.state;
}

int main() {
    const int width = 20;
    const int height = 20;
    std::vector<pattern::PatternData> grid(width * height);

    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto& cell = grid[y * width + x];
            cell.id = std::to_string(y * width + x);
            cell.position = glm::vec4(x, 0.0f, y, 1.0f);
            cell.coherence = dist(rng);
            cell.stability = dist(rng);
            cell.entropy = dist(rng);
            cell.mutation_rate = 0.05f;
            cell.memory_tier = memory::MemoryTierEnum::STM;
        }
    }

    const float spike_decay = 0.01f;
    const float spike_threshold = 1.0f;
    const float learning_rate = 0.05f;

    for (int iter = 0; iter < 100; ++iter) {
        std::vector<quantum::Pattern> patterns(grid.size());
        for (size_t i = 0; i < grid.size(); ++i) patterns[i] = toPattern(grid[i]);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t idx = y * width + x;
                float input = 0.0f;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = (x + dx + width) % width;
                        int ny = (y + dy + height) % height;
                        input += patterns[ny * width + nx].quantum_state.coherence * 0.2f;
                    }
                }
                quantum::evolution::applySpike(patterns[idx], input, spike_decay, spike_threshold);
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                size_t idx = y * width + x;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        int nx = (x + dx + width) % width;
                        int ny = (y + dy + height) % height;
                        quantum::evolution::hebbianUpdate(patterns[idx], patterns[ny * width + nx], learning_rate);
                    }
                }
                patterns[idx] = quantum::evolution::gaussianMutation(patterns[idx], 0.02f);
            }
        }

        for (size_t i = 0; i < grid.size(); ++i) fromPattern(patterns[i], grid[i]);
    }

    std::vector<glm::ivec2> stable;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const auto& cell = grid[y * width + x];
            if (cell.coherence > 0.9f && cell.stability > 0.7f) {
                stable.emplace_back(x, y);
            }
        }
    }

    std::cout << "Stable LTM patterns:\n";
    for (const auto& s : stable) {
        std::cout << "(" << s.x << "," << s.y << ")\n";
    }
    return 0;
}

