#pragma once

#include <memory>

#include "demo_base.hpp"
#include "stubs/engine_stubs.h"
#include "stubs/imgui.h"

// Forward declarations
namespace sep {
namespace core {
class Engine;
}
class SimpleRenderer;
}

namespace sep
{
    namespace workbench
    {

        class GenesisPatternDemo : public Demo
        {
        public:
            void on_load(sep::core::Engine* engine, sep::SimpleRenderer* renderer) override;
            void on_update(float dt) override;
            void on_render() override;
            void on_ui_render() override;
            void on_unload() override;
            void on_key_press(int key) override;
            void on_mouse(int x, int y, int button);

        private:
            void initializePatterns();
            void evolvePatterns(float dt);
            void updateVisualization();

            struct ViewSettings
            {
                float rotation{0.0f};
                float zoom{1.0f};
                bool wireframe{false};
            } view_;

            struct Metrics
            {
                float coherence{0.f};
                std::size_t pattern_count{0};
                float evolution_rate{0.f};
                std::size_t iterations{0};
            } metrics_;

            std::unique_ptr<sep::quantum::Processor> pattern_processor_;
            std::unique_ptr<sep::memory::QuantumCoherenceManager> coherence_manager_;
            sep::SimpleRenderer* renderer_{nullptr};

            sep::core::Engine* engine_{nullptr};
        };

    }  // namespace workbench
}  // namespace sep
