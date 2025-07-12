#include "workbench/demos/demo_base.hpp"
#pragma once
#include "workbench/demos/demo_base.hpp"

#include <array>
#include <memory>
#include <random>
#include <vector>

#include "core/types.h"
#include "demo_manager.hpp"
#include "imgui.h"
#include "memory/memory_tier_manager.hpp"
namespace sep { namespace quantum { using Pattern = ::sep::Pattern; } }
#include "sep_engine_wrapper.h"

namespace sep
{
    namespace workbench
    {

        class NeuroSimDemo : public Demo
        {
        public:
            NeuroSimDemo() : rd_{}, seed_{rd_()}, rng_{seed_} {}

            void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
            void on_ui_render() override;
            void on_update(float dt) override;
            void on_render() override;
            void on_unload() override;
            void on_key_press(int key) override;
            void on_mouse(int x, int y, int button) override;

        private:
            struct Neuron
            {
                sep::Pattern pattern;
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
