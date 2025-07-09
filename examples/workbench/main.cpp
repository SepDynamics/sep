#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "demos/audio_visualizer.hpp"
#include "demos/demo_manager.hpp"
#include "demos/digital_physics_demo.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/memory_garden.hpp"
#include "imgui.h"

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "SEP Workbench", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

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

    auto& manager = sep::workbench::DemoManager::getInstance();
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
