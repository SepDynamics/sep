#include "workbench_demo_adapter.hpp"

#include "renderer.h"
#include "sep_engine_wrapper.h"
#include "ui_manager.h"
#include "window.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace sep
{
    namespace workbench
    {
        GenesisPatternAdapter::GenesisPatternAdapter(Window* window, Renderer* renderer,
                                                              UIManager* uiManager)
                     : window_(window), renderer_(renderer), uiManager_(uiManager), tick_(0)
                 {
                     // Initialize the SEP engine
                     engine_ = createEngine();
                    if (engine_)
                    {
                        engine_->initialize();
                        engine_->setCudaEnabled(true);
                        engine_->setMetricsEnabled(true);
                        std::cout << "SEP Engine initialized successfully." << std::endl;
                    }
                    else
                    {
                        std::cerr << "Failed to create SEP Engine!" << std::endl;
                    }
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

                // Initialize with deterministic values instead of random ones
                patterns_.back().values.resize(16);
                for (int j = 0; j < 16; j++)
                {
                    // Use a deterministic pattern based on indices
                    float val = (float)((i + j) % 10) / 10.0f;
                    patterns_.back().values[j] = val;
                }

                // Initialize quantum state with deterministic values
                patterns_.back().quantum_state.coherence = 0.5f;
                patterns_.back().quantum_state.stability = 0.5f + (float)(i % 5) / 10.0f;
            }

            std::cout << "Generated " << count << " patterns" << std::endl;
        }

        void GenesisPatternAdapter::evolvePatterns(float dt)
        {
            if (!engine_ || patterns_.empty()) {
                return;
            }

            tick_++;

            // This is a more advanced evolution algorithm that simulates quantum-inspired
            // behavior. In a future update, this will be connected directly to the SEP
            // core engine for real quantum processing.

            // First, calculate pattern interactions to simulate quantum entanglement
            std::vector<float> coherence_changes(patterns_.size(), 0.0f);
            std::vector<float> stability_changes(patterns_.size(), 0.0f);

            // Phase 1: Calculate inter-pattern influences (simulating entanglement)
            for (size_t i = 0; i < patterns_.size(); ++i) {
                for (size_t j = i + 1; j < patterns_.size(); ++j)
                {
                    // Calculate "distance" between patterns based on their values
                    float similarity = 0.0f;
                    if (!patterns_[i].values.empty() && !patterns_[j].values.empty())
                    {
                        size_t min_size =
                            std::min(patterns_[i].values.size(), patterns_[j].values.size());
                        for (size_t k = 0; k < min_size; ++k)
                        {
                            similarity += std::abs(patterns_[i].values[k] - patterns_[j].values[k]);
                        }
                        similarity /= min_size;
                        similarity = 1.0f - similarity;  // Convert to similarity (0-1)
                    }

                    // Calculate mutual influence based on similarity and current states
                    float influence = similarity * evolution_rate_ * 0.1f;

                    // Coherence exchange - more coherent patterns influence less coherent ones
                    float coherence_diff =
                        patterns_[i].quantum_state.coherence - patterns_[j].quantum_state.coherence;
                    coherence_changes[i] -= coherence_diff * influence;
                    coherence_changes[j] += coherence_diff * influence;

                    // Stability reinforcement - similar patterns reinforce each other's stability
                    stability_changes[i] += similarity * influence;
                    stability_changes[j] += similarity * influence;
                }

                // Add a wave function oscillation component based on tick
                float oscillation = std::sin(tick_ * 0.1f + i * 0.3f) * evolution_rate_ * 0.03f;
                coherence_changes[i] += oscillation;
            }

            // Phase 2: Apply changes and use sep::quantum namespace functions when available
            for (size_t i = 0; i < patterns_.size(); ++i)
            {
                // Apply coherence changes with bounds checking
                patterns_[i].quantum_state.coherence = std::clamp(
                    patterns_[i].quantum_state.coherence + coherence_changes[i], 0.0f, 1.0f);

                // Apply stability changes with bounds checking
                patterns_[i].quantum_state.stability = std::clamp(
                    patterns_[i].quantum_state.stability + stability_changes[i], 0.0f, 1.0f);

                // Apply Hebbian-inspired updates between adjacent patterns
                if (i > 0)
                {
                    // Direct implementation of Hebbian update (instead of calling
                    // sep::quantum::hebbianUpdate)
                    patterns_[i].quantum_state.stability +=
                        evolution_rate_ * 0.01f * patterns_[i - 1].quantum_state.coherence;
                    patterns_[i].quantum_state.stability =
                        std::min(1.0f, patterns_[i].quantum_state.stability);
                }

                // Simulate spike activity
                if (i % 3 == tick_ % 3)
                {  // Periodic activity
                    // Direct implementation of spike function (instead of calling
                    // sep::quantum::applySpike)
                    float input = 0.1f * evolution_rate_;
                    float decay = 0.02f;
                    patterns_[i].quantum_state.coherence =
                        std::min(1.0f, patterns_[i].quantum_state.coherence + input - decay);
                }

                // Update generation counter
                patterns_[i].quantum_state.generation++;
            }

            // Occasionally add a new pattern (every 10 ticks if under limit)
            if (tick_ % 10 == 0 && patterns_.size() < 200) {
                patterns_.emplace_back();
                
                // Set unique ID for the new pattern
                patterns_.back().id = "Pattern_" + std::to_string(patterns_.size() - 1);

                // Initialize with deterministic values
                patterns_.back().values.resize(16);
                for (int j = 0; j < 16; j++) {
                    float val = (float)((patterns_.size() + j) % 10) / 10.0f;
                    patterns_.back().values[j] = val;
                }

                // Initialize quantum state with deterministic values
                patterns_.back().quantum_state.coherence = 0.5f;
                patterns_.back().quantum_state.stability = 0.5f;

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
}  // namespace sep

