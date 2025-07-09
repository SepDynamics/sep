#include "window.h"
#include "demo_manager.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/annealing_demo.hpp"
#include "demos/cosmo_demo.hpp"
#include "demos/flocking_demo.hpp"
#include "demos/neural_demo.hpp"
#include "demos/drug_discovery_demo.hpp"
#include "demos/digital_physics_demo.hpp"
#include "demos/memory_garden.hpp"
#include "sep_engine_wrapper.h"
#include "config.hpp"

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Callback function prototypes
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// Global references for callbacks
sep::workbench::DemoManager* g_demo_manager = nullptr;
int g_mouse_x = 0;
int g_mouse_y = 0;
int g_mouse_button = -1;

int main(int argc, char* argv[]) {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    glfwSetErrorCallback(error_callback);
    
    // Set up OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 2. Create Window
    auto window = std::make_unique<sep::workbench::Window>(1280, 720, "SEP Workbench");
    if (!window->initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    window->makeContextCurrent();
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    // 3. Initialize the SEP Engine
    sep::config::EngineConfig engine_config;
    engine_config.memory_limit_mb = 1024;
    engine_config.use_gpu = true;
    engine_config.log_level = "info";
    
    auto engine = std::make_unique<sep::EngineWrapper>(engine_config);
    if (!engine->initialize()) {
        std::cerr << "Failed to initialize SEP Engine" << std::endl;
        return -1;
    }
    
    // Initialize renderer
    sep::CyclesRenderer* renderer = nullptr;
    #ifdef SEP_HAS_CYCLES
    renderer = new sep::CyclesRenderer();
    #endif
    
    // 4. Initialize the DemoManager
    auto& demo_manager = sep::workbench::DemoManager::getInstance();
    demo_manager.initialize(engine.get(), renderer);
    g_demo_manager = &demo_manager;
    
    // Set up input callbacks
    GLFWwindow* glfw_window = window->getGLFWWindow();
    glfwSetKeyCallback(glfw_window, key_callback);
    glfwSetMouseButtonCallback(glfw_window, mouse_button_callback);
    glfwSetCursorPosCallback(glfw_window, cursor_position_callback);
    
    // 5. Register all demo classes
    demo_manager.registerDemo("genesis", []() {
        return std::make_unique<sep::workbench::GenesisPatternDemo>();
    });
    
    demo_manager.registerDemo("annealing", []() {
        return std::make_unique<sep::workbench::AnnealingDemo>();
    });
    
    demo_manager.registerDemo("cosmos", []() {
        return std::make_unique<sep::workbench::CosmoDemo>();
    });
    
    demo_manager.registerDemo("flocking", []() {
        return std::make_unique<sep::workbench::FlockingDemo>();
    });
    
    demo_manager.registerDemo("neural", []() {
        return std::make_unique<sep::workbench::NeuralDemo>();
    });
    
    demo_manager.registerDemo("drug_discovery", []() {
        return std::make_unique<sep::workbench::DrugDiscoveryDemo>();
    });
    
    demo_manager.registerDemo("digital_physics", []() {
        return std::make_unique<sep::workbench::DigitalPhysicsDemo>();
    });
    
    demo_manager.registerDemo("memory_garden", []() {
        return std::make_unique<sep::workbench::MemoryGardenDemo>();
    });
    
    // Start with Genesis Pattern demo by default
    demo_manager.switchToDemo("genesis");
    
    // 6. Set up the main application loop
    double last_time = glfwGetTime();
    
    while (!window->shouldClose()) {
        // Calculate delta time
        double current_time = glfwGetTime();
        float dt = static_cast<float>(current_time - last_time);
        last_time = current_time;
        
        // Begin frame
        window->beginFrame();
        
        // 7. Update and render the current demo
        demo_manager.update(dt);
        demo_manager.render();
        
        // End frame
        window->endFrame();
        
        // Poll events
        window->pollEvents();
        
        // Cap framerate to ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    // Clean up
    demo_manager.cleanup();
    window->cleanup();
    
    if (renderer) {
        delete renderer;
    }
    
    glfwTerminate();
    return 0;
}

// Callback implementations
void error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            return;
        }
        
        if (g_demo_manager) {
            g_demo_manager->handleKeyboard(static_cast<unsigned char>(key));
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        g_mouse_button = button;
        
        if (g_demo_manager) {
            g_demo_manager->handleMouse(g_mouse_x, g_mouse_y, button);
        }
    } else if (action == GLFW_RELEASE) {
        g_mouse_button = -1;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    g_mouse_x = static_cast<int>(xpos);
    g_mouse_y = static_cast<int>(ypos);
    
    if (g_mouse_button >= 0 && g_demo_manager) {
        g_demo_manager->handleMouse(g_mouse_x, g_mouse_y, g_mouse_button);
    }
}
