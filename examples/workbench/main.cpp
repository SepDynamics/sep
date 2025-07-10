// System includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

// ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// SEP Engine includes (need to be first due to Pattern definition)
#include "sep_engine_wrapper.h"
#include "cycles_renderer_adapter.h"

// Demo manager and demos
#include "demos/demo_manager.hpp"
#include "demos/digital_physics_demo.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/memory_garden.hpp"
#include "audio_visualizer.hpp"

// Error callback for GLFW
static void error_callback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main(int /* argc */, char * /* argv */[])
{
    // Set environment variables for display
    setenv("DISPLAY", ":0", 0);  // Only set if not already set
    
    // Force libdecor to use the xdg-shell backend instead of GTK
    setenv("LIBDECOR_PLUGIN", "xdg-shell", 1);
    
    // Set GLFW error callback
    glfwSetErrorCallback(error_callback);
    
    std::cout << "Initializing GLFW..." << std::endl;
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }
    std::cout << "GLFW initialized successfully." << std::endl;
    
    // Configure GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    
    // Disable window decoration if libdecor is causing issues
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    // Try creating window with decorations disabled to avoid libdecor issues
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    
    std::cout << "Creating GLFW window..." << std::endl;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "SEP Workbench", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "GLFW window created successfully." << std::endl;

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // Create our SEP engine and renderer
    auto engine = sep::createEngine();
    auto renderer = std::make_unique<sep::workbench::Renderer>();
    renderer->init(1280, 720);
    
    // Create the adapter to make our Renderer work with the CyclesRenderer interface
    auto cycles_adapter = std::make_unique<sep::workbench::CyclesRendererAdapter>(renderer.get());
    
    // Initialize the demo manager
    auto& manager = sep::workbench::DemoManager::getInstance();
    manager.initialize(engine.get(), cycles_adapter.get());
    manager.registerDemo("genesis",
                         [] { return std::make_unique<sep::workbench::GenesisPatternDemo>(); });
    manager.registerDemo("digital",
                         [] { return std::make_unique<sep::workbench::DigitalPhysicsDemo>(); });
    manager.registerDemo("memory",
                         [] { return std::make_unique<sep::workbench::MemoryGardenDemo>(); });
    manager.registerDemo("audio",
                         [] { return std::make_unique<sep::workbench::AudioVisualizerDemo>(); });
    manager.switchToDemo("genesis");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Begin("Demos");
        if (ImGui::Button("Genesis")) manager.switchToDemo("genesis");
        if (ImGui::Button("Digital")) manager.switchToDemo("digital");
        if (ImGui::Button("Memory")) manager.switchToDemo("memory");
        if (ImGui::Button("Audio")) manager.switchToDemo("audio");
        ImGui::End();

        manager.on_update(0.016f);
        manager.on_render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    manager.on_unload();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
