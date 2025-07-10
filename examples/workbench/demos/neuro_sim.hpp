#pragma once

#include <array>
#include <memory>
#include <random>
#include <vector>

#include "demo_manager.hpp"
#include "memory/memory_tier_manager.hpp"
#include "quantum/types.h"
#include "sep_engine_wrapper.h"

namespace sep
{
    namespace workbench
    {

        class NeuroSimDemo : public Demo
        {
        public:
            NeuroSimDemo() : rd_{}, seed_{rd_()}, rng_{seed_} {}

            void on_load() override;
            void on_update(float dt) override;
            void on_render() override;
            void on_unload() override;
            void on_key_press(int key) override;
            void on_mouse(int x, int y, int button) override;

        private:
            struct Neuron
            {
                sep::quantum::Pattern pattern;
                float potential{0.f};
                uint64_t node_id{0};
            };

            std::unique_ptr<sep::memory::MemoryTierManager> memory_manager_;

            std::vector<Neuron> neurons_;
            std::random_device rd_;
            std::seed_seq seed_;
            std::mt19937 rng_;

            float threshold_{1.f};
            float decay_{0.1f};
            float input_strength_{0.5f};
            float learning_rate_{0.05f};
            float connection_prob_{0.2f};
        };

    }  // namespace workbench
}  // namespace sep
