#include <GL/glew.h>  // MUST be included before any other GL headers
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "renderer.h"
#include "ui_manager.h"
#include "window.h"

class GenesisPatternDemo
{
public:
    GenesisPatternDemo() = default;
    ~GenesisPatternDemo() = default;

    bool init()
    {
        std::cout << "Initializing Genesis Pattern Demo..." << std::endl;

        // Create window - update to use proper namespace
        window = new sep::workbench::Window(1280, 720, "SEP Workbench");
        if (!window->getGLFWWindow())
        {
            std::cerr << "Failed to create window" << std::endl;
            return false;
        }

        window->makeContextCurrent();

        // Initialize UI after window creation and GL context is current
        uiManager.init(*window);

        // Initialize renderer
        renderer.init(window->getWidth(), window->getHeight());

        // Generate initial patterns
        generatePatterns(50);  // Start with 50 patterns

        std::cout << "Genesis Pattern Demo initialized successfully." << std::endl;
        return true;
    }

    void run()
    {
        auto lastTime = std::chrono::high_resolution_clock::now();
        float deltaTime = 0.0f;

        // Main loop
        while (!window->shouldClose())
        {
            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            deltaTime =
                std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime)
                    .count();
            lastTime = currentTime;

            // Update
            update(deltaTime);

            // Render
            render();

            // Limit to ~60fps
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }

    void update(float deltaTime)
    {
        // Poll window events
        window->pollEvents();

        // Every N seconds, evolve all patterns
        static float timeSinceLastEvolution = 0.0f;
        timeSinceLastEvolution += deltaTime;

        // UI state
        static float evolutionRate = 0.2f;
        static bool autoEvolve = true;

        // Start UI frame
        uiManager.beginFrame();

        // Pattern control UI
        uiManager.showPatternControls(evolutionRate, autoEvolve);

        // Pattern metrics UI
        float avgCoherence = 0.0f;
        for (const auto& pattern : patterns)
        {
            avgCoherence += pattern.quantum_state.coherence;
        }
        avgCoherence = patterns.empty() ? 0.0f : avgCoherence / patterns.size();
        uiManager.showPatternMetrics(patterns.size(), avgCoherence);

        // End UI frame
        uiManager.endFrame();

        if (autoEvolve && timeSinceLastEvolution >= 0.5f)
        {  // Evolve every 0.5 seconds
            for (auto& pattern : patterns)
            {
                pattern.evolve(evolutionRate);  // Use UI-controlled evolution rate
            }
            timeSinceLastEvolution = 0.0f;

            // Occasionally add a new pattern (1% chance each evolution step)
            if (rand() % 100 < 1 && patterns.size() < 200)
            {
                patterns.emplace_back();
                std::cout << "Added new pattern, total: " << patterns.size() << std::endl;
            }
        }
    }

    void render()
    {
        // Clear the screen
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render patterns
        renderer.render(patterns);

        // Render UI
        uiManager.render();

        // Swap buffers
        window->swapBuffers();
    }

    void cleanup()
    {
        if (window)
        {
            delete window;
            window = nullptr;
        }
    }

private:
    void generatePatterns(int count)
    {
        patterns.clear();
        patterns.reserve(count);

        for (int i = 0; i < count; i++)
        {
            patterns.emplace_back();
            // Set unique IDs for the patterns
            patterns.back().id = "Pattern_" + std::to_string(i);

            // Initialize with random values
            patterns.back().values.resize(16);
            for (auto& val : patterns.back().values)
            {
                val = ((float)rand() / RAND_MAX);
            }
        }

        std::cout << "Generated " << count << " patterns" << std::endl;
    }

    sep::workbench::Window* window = nullptr;
    UIManager uiManager;
    Renderer renderer;
    std::vector<sep::quantum::Pattern> patterns;
};

int main(int argc, char** argv)
{
    std::cout << "SEP Workbench Demo" << std::endl;
    std::cout << "=================" << std::endl;

    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    GenesisPatternDemo demo;

    if (!demo.init())
    {
        std::cerr << "Failed to initialize demo" << std::endl;
        return 1;
    }

    demo.run();
    demo.cleanup();

    std::cout << "Demo completed successfully" << std::endl;
    return 0;
}