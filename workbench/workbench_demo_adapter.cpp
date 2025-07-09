#include "workbench_demo_adapter.hpp"

#include "renderer.h"
#include "ui_manager.h"
#include "window.h"
// These includes are necessary to fix the incomplete type errors
#include <cstdlib>
#include <ctime>
#include <iostream>


namespace workbench
{

    using sep::workbench;
    
    GenesisPatternAdapter::GenesisPatternAdapter(Window* window, Renderer* renderer,
                                                 UIManager* uiManager)
        window_(window),
        renderer_(renderer),
        uiManager_(uiManager)
    {
        // Seed the random number generator
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    GenesisPatternAdapter::~GenesisPatternAdapter() = default;

    void GenesisPatternAdapter::init()
    {
        std::cout << "Initializing Genesis Pattern Demo..." << std::endl;

        // Generate initial patterns
        generatePatterns(50);  // Start with 50 patterns

        std::cout << "Genesis Pattern Demo initialized successfully." << std::endl;
    }

    void GenesisPatternAdapter::update(float dt)
    {
        // Poll window events
        if (window_)
        {
            window_->pollEvents();
        }

        // Every N seconds, evolve all patterns
        timeSinceLastEvolution_ += dt;

        // UI state handling
        if (uiManager_)
        {
            uiManager_->beginFrame();

            // Pattern control UI
            uiManager_->showPatternControls(evolution_rate_, auto_evolve_);

            // Pattern metrics UI
            float avgCoherence = 0.0f;
            for (const auto& pattern : patterns_)
            {
            avgCoherence += pattern.quantum_state.coherence;
            }
            avgCoherence = patterns_.empty() ? 0.0f : avgCoherence / patterns_.size();
            uiManager_->showPatternMetrics(patterns_.size(), avgCoherence);

            uiManager_->endFrame();
        }

        if (auto_evolve_ && timeSinceLastEvolution_ >= 0.5f)
        {
            evolvePatterns(dt);
            timeSinceLastEvolution_ = 0.0f;
        }
    }

    void GenesisPatternAdapter::render()
    {
        // Render the patterns
        if (renderer_)
        {
            renderer_->render(patterns_);
        }

        // Render UI
        if (uiManager_)
        {
            uiManager_->render();
        }

        // Swap buffers
        if (window_)
        {
            window_->swapBuffers();
        }
    }

    void GenesisPatternAdapter::cleanup() { patterns_.clear(); }

    void GenesisPatternAdapter::handleKeyboard(unsigned char key)
    {
        switch (key)
        {
            case ' ':  // Space - toggle auto evolution
                auto_evolve_ = !auto_evolve_;
                break;
            case '+':  // Increase evolution rate
                evolution_rate_ *= 1.1f;
                evolution_rate_ = std::min(evolution_rate_, 1.0f);
                break;
            case '-':  // Decrease evolution rate
                evolution_rate_ *= 0.9f;
                evolution_rate_ = std::max(evolution_rate_, 0.01f);
                break;
        }
    }

    void GenesisPatternAdapter::handleMouse(int x, int y, int button)
    {
        // Not used in this simple demo
    }

    void GenesisPatternAdapter::generatePatterns(int count)
    {
        patterns_.clear();
        patterns_.reserve(count);

        for (int i = 0; i < count; i++)
        {
            patterns_.emplace_back();
            // Set unique IDs for the patterns
            patterns_.back().id = "Pattern_" + std::to_string(i);

            // Initialize with random values
            patterns_.back().values.resize(16);
            for (auto& val : patterns_.back().values)
            {
                val = ((float)rand() / RAND_MAX);
            }

            // Initialize quantum state
            patterns_.back().quantum_state.coherence = 0.5f;
            patterns_.back().quantum_state.stability = ((float)rand() / RAND_MAX);
            }

        std::cout << "Generated " << count << " patterns" << std::endl;
    }

    void GenesisPatternAdapter::evolvePatterns(float dt)
    {
        for (auto& pattern : patterns_)
        {
            // Evolve each pattern
            pattern.evolve(evolution_rate_);
        }

        // Occasionally add a new pattern (1% chance each evolution step)
        if (rand() % 100 < 1 && patterns_.size() < 200)
        {

            patterns_.emplace_back();
            // Set unique ID for the new pattern
            patterns_.back().id = "Pattern_" + std::to_string(patterns_.size() - 1);

            // Initialize with random values
            patterns_.back().values.resize(16);
            for (auto& val : patterns_.back().values)
            {
                val = ((float)rand() / RAND_MAX);
            }

            // Initialize quantum state
            patterns_.back().quantum_state.coherence = 0.5f;
            patterns_.back().quantum_state.stability = ((float)rand() / RAND_MAX);

            std::cout << "Added new pattern, total: " << patterns_.size() << std::endl;
        }
    }

    // Factory function implementation - fixing the conversion issue
    std::unique_ptr<Demo> createGenesisPatternDemo(Window* window, Renderer* renderer,
                                                       UIManager* uiManager)
    {
        std::unique_ptr<Demo> demo(new GenesisPatternAdapter(window, renderer, uiManager));
        return demo;
    }
}  // namespace workbench

