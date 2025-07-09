#pragma once

#include <array>
#include <memory>
#include <random>
#include <vector>

#include "demo_manager.hpp"
#include "../sep_engine_wrapper.h"

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
            void on_key_press(unsigned char key) override;
            void on_mouse(int x, int y, int button);

        private:
            struct Neuron
            {
#ifdef SEP_WORKBENCH_DEMO
                sep::Pattern pattern;  // In demo mode, Pattern is directly in sep namespace
#else
                sep::quantum::Pattern pattern;  // In non-demo mode, Pattern is in quantum namespace
#endif
                float potential{0.f};
                uint64_t node_id{0};
            };

            // Memory manager type depends on demo mode
#ifdef SEP_WORKBENCH_DEMO
            std::unique_ptr<sep::MemoryTierManager> memory_manager_;  // In demo mode
#else
            std::unique_ptr<sep::memory::MemoryTierManager> memory_manager_;  // In non-demo mode
#endif

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
