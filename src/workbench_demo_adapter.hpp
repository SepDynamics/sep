#pragma once

#include <memory>
#include <string>
#include <vector>

// Include full class definitions instead of forward declarations
#include "quantum_pattern.hpp"
#include "renderer.h"
#include "sep_engine_wrapper.h"
#include "ui_manager.h"
#include "window.h"

namespace sep
{
    namespace workbench
    {
        // Simplified Demo interface for our workbench
        class Demo
        {
        public:
            virtual ~Demo() = default;
            void initialize(sep::Engine* engine, sep::CyclesRenderer* renderer) {
                engine_ = engine;
                renderer_ = renderer;
            }
            virtual void on_load() = 0;
            virtual void on_unload() = 0;
            virtual void on_update(float dt) = 0;
            virtual void on_render() = 0;
            virtual void on_key_press(int key) = 0;
            virtual void on_mouse(int x, int y, int button) = 0;
        
        protected:
            sep::Engine* engine_ = nullptr;
            sep::CyclesRenderer* renderer_ = nullptr;
        };

        // Adapter class to connect our GenesisPatternDemo to the demo system
        class GenesisPatternAdapter : public Demo
        {
        public:
            GenesisPatternAdapter(Window* window, Renderer* renderer, UIManager* uiManager);
            ~GenesisPatternAdapter() override;

            void on_load() override;
            void on_update(float dt) override;
            void on_render() override;
            void on_unload() override;
            void on_key_press(int key) override;
            void on_mouse(int x, int y, int button) override;

        private:
            void generatePatterns(int count);
            void evolvePatterns(float dt);

            Window* window_;
            Renderer* renderer_;
            UIManager* uiManager_;
            std::vector<workbench::Pattern> patterns_;
            std::unique_ptr<sep::Engine> engine_;

            bool auto_evolve_ = true;
            float evolution_rate_ = 0.2f;
            float coherence_threshold_ = 0.5f;
            float timeSinceLastEvolution_ = 0.0f;
            uint64_t tick_ = 0;
        };

        // Factory function to create our demo adapter
        std::unique_ptr<Demo> createGenesisPatternDemo(Window* window, Renderer* renderer,
                                                       UIManager* uiManager);

    }  // namespace workbench
}  // namespace sep