#pragma once

#include <glm/vec3.hpp>
#include <vector>

#include "../demo_manager.hpp"
#include "sep_engine_wrapper.h"

namespace sep
{
    namespace workbench
    {

        struct Pose
        {
            glm::vec3 position{0.0f};
            glm::vec3 orientation{0.0f};
            float binding_affinity{0.0f};
        };

        class DrugDiscoveryDemo : public Demo
        {
        public:
            void init() override;
            void update(float dt) override;
            void render() override;
            void cleanup() override;
            void handleKeyboard(unsigned char key) override;
            void handleMouse(int x, int y, int button) override;

        private:
            void optimizePoses();

            std::vector<Pose> poses_;
            int iterations_{10};
            float mutation_rate_{0.05f};
        };

    }  // namespace workbench
}  // namespace sep
