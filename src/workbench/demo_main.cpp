#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "core/engine.h"
#include "demo_manager.hpp"
#include "renderer.h"
#include "sep_engine_wrapper.h"
#include "ui_manager.h"
#include "window.h"

// Forward declaration for demo registration
namespace sep
{
    namespace workbench
    {
        void registerDemos();
        extern const std::map<char, std::string> demo_keys;
    }  // namespace workbench
}  // namespace sep

// Keyboard callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        char key_char = static_cast<char>(key);
        if (sep::workbench::demo_keys.find(key_char) != sep::workbench::demo_keys.end())
        {
            std::string demo_name = sep::workbench::demo_keys.at(key_char);
            std::cout << "Switching to demo: " << demo_name << std::endl;
            sep::workbench::DemoManager::getInstance().setActiveDemo(demo_name);
        }
    }
}

int main(int argc, char** argv)
{
    std::cout << "SEP Workbench Demo\n=================\n";

// Initialize SEP Engine
#include "core/manager.h"  // For APIConfig

    sep::config::APIConfig api_config;  // Create a config object
    sep::core::Engine engine;           // Create an instance on the stack
    if (!engine.init(api_config))
    {  // Pass the config to init()
        std::cerr << "Failed to initialize SEP engine\n";
        return 1;
    }

    // Create engine wrapper for demos
    sep::SepEngineWrapper& engine_wrapper = sep::getEngineWrapper();
    engine_wrapper.initialize();

    // Initialize demo manager with engine
    sep::workbench::DemoManager::getInstance().initialize(&engine, nullptr);

    // Register demos
    sep::workbench::registerDemos();

    // Create window
    sep::workbench::Window window("SEP Workbench", 1280, 720);
    if (!window.initialize())
    {
        std::cerr << "Failed to initialize window\n";
        return 1;
    }

    // Set key callback
    glfwSetKeyCallback(window.getGLFWWindow(), key_callback);

    // Initialize UI
    sep::workbench::UIManager ui_manager;
    ui_manager.initialize();

    // Initialize renderer
    sep::workbench::Renderer renderer(1280, 720);
    renderer.initialize();

    // Get demo name from command line if provided
    std::string demo_name = "genesis";
    if (argc > 1)
    {
        demo_name = argv[1];
        std::cout << "Using demo from command line: " << demo_name << std::endl;
    }

    // Set active demo
    sep::workbench::DemoManager::getInstance().setActiveDemo(demo_name);

    // Main loop
    double last_time = glfwGetTime();
    while (!window.shouldClose())
    {
        // Calculate delta time
        double current_time = glfwGetTime();
        float delta_time = float(current_time - last_time);
        last_time = current_time;

        // Update demo
        sep::workbench::DemoManager::getInstance().update(delta_time);

        // Render
        window.beginFrame();
        ui_manager.beginFrame();

        sep::workbench::DemoManager::getInstance().render();

        ui_manager.endFrame();
        window.endFrame();

        // Process events
        window.pollEvents();
    }

    // Cleanup
    sep::workbench::DemoManager::getInstance().cleanup();
    renderer.cleanup();
    ui_manager.cleanup();
    window.cleanup();

    return 0;
}
