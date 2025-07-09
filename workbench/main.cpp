#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// Use our mock types for development
#include "window.h"
#include "ui_manager.h"
#include "renderer.h"

// Our workbench components
#include "renderer.h"
#include "ui_manager.h"
#include "window.h"

// Forward declarations for the demo system
class Demo;
class GenesisPatternDemo;

// Base Demo class
class Demo
{
public:
    virtual ~Demo() = default;
    virtual bool init() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
};

// Genesis Pattern Demo implementation
class GenesisPatternDemo : public Demo
{
public:
    GenesisPatternDemo() = default;
    ~GenesisPatternDemo() override = default;

    bool init() override
    {
        std::cout << "Initializing Genesis Pattern Demo..." << std::endl;

        // Initialize memory manager
        auto& memory_mgr = sep::memory::MemoryTierManager::getInstance();
        sep::memory::MemoryTierManager::Config memory_config;
        memory_mgr.init(memory_config);

        // Initialize pattern processor with a default config
        sep::quantum::ProcessingConfig config;
        pattern_processor_ = sep::quantum::createProcessor(config);

        if (!pattern_processor_)
        {
            std::cerr << "Failed to create Pattern Processor!" << std::endl;
            return false;
        }

        auto result = pattern_processor_->init(nullptr);
        if (result != sep::SEPResult::SUCCESS)
        {
            std::cerr << "Failed to initialize Pattern Processor!" << std::endl;
            return false;
        }

        // Create some initial patterns
        for (int i = 0; i < 5; i++)
        {
            sep::quantum::Pattern pattern;
            pattern.id = "pattern_" + std::to_string(i);
            pattern.quantum_state.coherence = 0.5f;
            pattern.quantum_state.stability = 0.5f;
            pattern.quantum_state.generation = 1;
            pattern_processor_->addPattern(pattern);
        }

        // Initialize renderer
        renderer_ = std::make_unique<Renderer>();
        renderer_->init(window_->getWidth(), window_->getHeight());

        // Initialize UI
        ui_manager_ = std::make_unique<UIManager>();
        ui_manager_->init(*window_);

        std::cout << "Genesis Pattern Demo initialized successfully." << std::endl;
        return true;
    }

    void update(float deltaTime) override
    {
        // Poll events
        window_->pollEvents();

        // Process all patterns
        auto result = pattern_processor_->processAll();

        // Evolve patterns if auto-evolve is enabled
        if (auto_evolve_)
        {
            auto patterns = pattern_processor_->getPatterns();
            if (!patterns.empty())
            {
                auto& pattern = patterns[0];
                pattern_processor_->evolvePattern(pattern.id);
            }
        }

        // Start ImGui frame
        ui_manager_->beginFrame();

        // Show UI windows
        ui_manager_->showPatternControls(evolution_rate_, auto_evolve_);

        // Calculate average coherence for metrics display
        auto patterns = pattern_processor_->getPatterns();
        float avg_coherence = 0.0f;
        for (const auto& pattern : patterns)
        {
            avg_coherence += pattern.quantum_state.coherence;
        }
        avg_coherence = patterns.empty() ? 0.0f : avg_coherence / patterns.size();

        ui_manager_->showPatternMetrics(patterns.size(), avg_coherence);

        // End ImGui frame
        ui_manager_->endFrame();
    }

    void render() override
    {
        // Get the current patterns to render
        auto patterns = pattern_processor_->getPatterns();

        // Render the patterns
        renderer_->render(patterns);

        // Render ImGui
        ui_manager_->render();

        // Swap buffers
        window_->swapBuffers();
    }

    void setWindow(std::shared_ptr<Window> window) { window_ = window; }

private:
    std::unique_ptr<sep::quantum::Processor> pattern_processor_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<UIManager> ui_manager_;
    std::shared_ptr<Window> window_;

    // Demo parameters
    float evolution_rate_ = 1.0f;
    bool auto_evolve_ = true;
};

int main(int argc, char** argv)
{
    std::cout << "SEP Workbench Demo" << std::endl;
    std::cout << "=================" << std::endl;

    // Create the window
    auto window = std::make_shared<Window>(1280, 720, "SEP Workbench");

    // Create the demo
    auto demo = std::make_unique<GenesisPatternDemo>();

    // Set the window for the demo
    static_cast<GenesisPatternDemo*>(demo.get())->setWindow(window);

    // Initialize
    if (!demo->init())
    {
        std::cerr << "Failed to initialize demo!" << std::endl;
        return 1;
    }

    // Main loop
    bool running = true;
    float deltaTime = 0.016f;  // ~60 FPS

    while (!window->shouldClose())
    {
        // Update
        demo->update(deltaTime);

        // Render
        demo->render();

        // Sleep to cap the frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "Demo completed successfully." << std::endl;
    return 0;
}
