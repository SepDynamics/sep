#include <glad/glad.h>  // GLAD first
#include <GLFW/glfw3.h>  // Then GLFW

#include <fstream>
#include <iostream>
#include "workbench/config.hpp"
#include <memory>  // For std::unique_ptr

#include "core/logging.h"
#include "cycles_renderer_adapter.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "renderer.h"
#include "sep_engine_wrapper.h"
#include "workbench/demos/demo_manager.hpp"

// Forward declaration for demo registration
namespace sep {
namespace workbench {
void registerDemos();
}
}  // namespace sep

// Key callback
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)window;
    (void)scancode;
    (void)mods;
    if (action == GLFW_PRESS) {
        auto& demoManager = sep::workbench::DemoManager::getInstance();

        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9) {
            const std::map<int, std::string> keyToDemoMap = {
                {GLFW_KEY_1, "genesis"}, {GLFW_KEY_2, "neural"}, {GLFW_KEY_3, "memory"},
                {GLFW_KEY_4, "flocking"}, {GLFW_KEY_5, "cosmo"}, {GLFW_KEY_6, "cosmo_sim"},
                {GLFW_KEY_7, "physics"}, {GLFW_KEY_8, "drug"}, {GLFW_KEY_9, "audio"}
            };
            auto it = keyToDemoMap.find(key);
            if (it != keyToDemoMap.end()) {
                demoManager.switchToDemo(it->second);
            }
        } else {
            demoManager.on_key(key);
        }
    }
}

int main() {
    // Force X11 backend and avoid GTK decorations which can cause segfaults
    setenv("LIBDECOR_BACKEND", "cairo", 1);
    setenv("LIBDECOR_NOT_GTK", "1", 1);
    setenv("GDK_BACKEND", "x11", 1);
    setenv("GLFW_PLATFORM", "x11", 1);
    setenv("GLFW_USE_WAYLAND", "0", 1);
    
    // Additional variables to help avoid GTK issues
    setenv("SDL_VIDEODRIVER", "x11", 1);
    setenv("XDG_SESSION_TYPE", "x11", 1);

    // GLFW error callback
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    });

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Print GLFW info
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    std::cout << "GLFW Version: " << major << "." << minor << "." << rev << std::endl;
    std::cout << "GLFW Compiled: " << glfwGetVersionString() << std::endl;

    // Start with legacy/basic OpenGL profile for maximum compatibility
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    std::cout << "Attempting to create window with OpenGL 2.1..." << std::endl;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "SEP Workbench", nullptr, nullptr);
    if (!window) {
        // Fallback to default hints
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        window = glfwCreateWindow(1280, 720, "SEP Workbench (Fallback)", nullptr, nullptr);
    }

    if (!window) {
        // Ultimate fallback to legacy
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        window = glfwCreateWindow(1280, 720, "SEP Workbench (Legacy)", nullptr, nullptr);
    }

    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // VSync

    glfwSetKeyCallback(window, key_callback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Print GL info
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // Initialize with safer error handling
    std::unique_ptr<sep::Engine> engine;
    std::unique_ptr<sep::CyclesRenderer> renderer;
    
    // Get reference to demo manager (singleton) outside try block for correct scope
    auto& demoManager = sep::workbench::DemoManager::getInstance();
    
    try {
        // Initialize logger
        std::cout << "Initializing logging system..." << std::endl;
        sep::logging::Manager::initialize();
        auto logger = sep::logging::Manager::getInstance().createLogger("workbench", {});
        
        // Initialize engine and renderer with error handling
        std::cout << "Creating engine..." << std::endl;
        engine = sep::createEngine();
        if (!engine) {
            std::cerr << "Failed to create engine!" << std::endl;
            return -1;
        }
        
        std::cout << "Creating renderer..." << std::endl;
        renderer = sep::createRenderer();
        if (!renderer) {
            std::cerr << "Failed to create renderer!" << std::endl;
            return -1;
        }
        
        std::cout << "Initializing engine..." << std::endl;
        engine->initialize();
        
        std::cout << "Initializing renderer..." << std::endl;
        renderer->initialize();
        
        // Initialize demo manager
        std::cout << "Initializing demo manager..." << std::endl;
        demoManager.initialize(engine.get(), renderer.get());

        // Load workbench configuration
        auto& config = sep::workbench::Config::getInstance();
        if (!config.load("src/workbench/config.json")) {
            std::cout << "Using default configuration..." << std::endl;
        }
        
        
        // Register demos
        std::cout << "Registering demos..." << std::endl;
        sep::workbench::registerDemos();
        
        // Select a default demo to start with
        std::cout << "Selecting default demo..." << std::endl;
        if (!demoManager.switchToDemo("genesis")) {
            std::cerr << "Failed to select default demo!" << std::endl;
            return -1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception during initialization: " << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    } catch (...) {
        std::cerr << "Unknown exception during initialization!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Initialize ImGui (with safeguards)
    bool imgui_available = true;
    try {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        const char* glsl_version = "#version 330";  // Use 330 for compat
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    } catch (const std::exception& e) {
        std::cerr << "ImGui init failed: " << e.what() << std::endl;
        imgui_available = false;
    }

    // Main loop
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double current_time = glfwGetTime();
        float dt = static_cast<float>(current_time - last_time);
        last_time = current_time;

        demoManager.on_update(dt);
        demoManager.on_render();

        if (imgui_available) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Render global demo selection menu
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("Demos")) {
                    for (const auto& name : demoManager.getRegisteredDemos()) {
                        bool selected = (demoManager.getCurrentDemo() == name);
                        if (ImGui::MenuItem(name.c_str(), nullptr, selected)) {
                            demoManager.switchToDemo(name);
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }

            demoManager.on_ui_render();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    demoManager.on_unload();
    engine->shutdown();

    if (imgui_available) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}