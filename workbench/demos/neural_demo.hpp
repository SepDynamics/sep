#pragma once

#include <vector>

#include "demo_manager.hpp"
#include "sep_engine_wrapper.h"

namespace sep
{
    namespace workbench
    {

        class NeuralDemo : public Demo
        {
        public:
            void init() override;
            void update(float dt) override;
            void render() override;
            void cleanup() override;
            void handleKeyboard(unsigned char key) override;
            void handleMouse(int x, int y, int button) override;

        private:
            struct Neuron
            {
                uint64_t id;
                float potential{0.f};
            };

            std::vector<Neuron> neurons_;
            dag::DagGraph graph_;
            float threshold_{1.0f};
            float decay_{0.1f};
            float weight_{0.5f};
        };

    }  // namespace workbench
}  // namespace sep
