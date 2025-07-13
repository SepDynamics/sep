#include <glad/glad.h>  // GLAD first
#include <GLFW/glfw3.h>  // Then GLFW

#include <fstream>
#include <iostream>
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
    // Environment variables (already good, but confirm they're set early)
    setenv("LIBDECOR_BACKEND", "cairo", 1);
    setenv("LIBDECOR_NOT_GTK", "1", 1);
    setenv("GDK_BACKEND", "x11", 1);
    setenv("GLFW_PLATFORM", "x11", 1);
    setenv("GLFW_USE_WAYLAND", "0", 1);

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

    // Window hints (start with compat profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

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

    // Initialize logger
    sep::logging::Manager::initialize();
    auto logger = sep::logging::Manager::getInstance().createLogger("workbench", {});

    // Initialize engine and renderer
    std::unique_ptr<sep::Engine> engine = sep::createEngine();
    std::unique_ptr<sep::CyclesRenderer> renderer = sep::createRenderer();
    engine->initialize();
    renderer->initialize();

    // Initialize demo manager
    auto& demoManager = sep::workbench::DemoManager::getInstance();
    demoManager.initialize(engine.get(), renderer.get());

    // Register demos
    sep::workbench::registerDemos();

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