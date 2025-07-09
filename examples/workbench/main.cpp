#include "window.h"
#include "demos/demo_manager.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/digital_physics_demo.hpp"
#include "demos/memory_garden.hpp"
#include "demos/audio_visualizer.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <chrono>
#include <thread>

int main() {
    sep::workbench::Window window(1280, 720, "SEP Workbench");
    window.makeContextCurrent();

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
    ImGui_ImplOpenGL3_Init();

    auto &manager = sep::workbench::DemoManager::getInstance();
    manager.registerDemo("genesis", []{ return std::make_unique<sep::workbench::GenesisPatternDemo>(); });
    manager.registerDemo("digital", []{ return std::make_unique<sep::workbench::DigitalPhysicsDemo>(); });
    manager.registerDemo("memory", []{ return std::make_unique<sep::workbench::MemoryGardenDemo>(); });
    manager.registerDemo("audio", []{ return std::make_unique<sep::workbench::AudioVisualizerDemo>(); });
    manager.switchToDemo("genesis");

    while (!window.shouldClose()) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Begin("Demos");
        if (ImGui::Button("Genesis")) manager.switchToDemo("genesis");
        if (ImGui::Button("Digital")) manager.switchToDemo("digital");
        if (ImGui::Button("Memory")) manager.switchToDemo("memory");
        if (ImGui::Button("Audio")) manager.switchToDemo("audio");
        ImGui::End();

        manager.update(0.016f);
        manager.render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swapBuffers();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
