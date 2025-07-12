#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "core/logging.h"             // For our logger
#include "cycles_renderer_adapter.h"  // Cycles renderer
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "sep_engine_wrapper.h"              // SEP Engine wrapper
#include "workbench/demos/demo_manager.hpp"  // Demo manager

// Forward declaration for demo registration function
namespace sep
{
    namespace workbench
    {
        void registerDemos();
    }
}  // namespace sep

// Key callback function for GLFW
void key_callback(GLFWwindow* /*window*/, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS)
    {
        auto& demoManager = sep::workbench::DemoManager::getInstance();

        // Handle demo selection keys (1-9)
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_9)
        {
            const std::map<int, std::string> keyToDemoMap = {
                {GLFW_KEY_1, "genesis"},  {GLFW_KEY_2, "neural"}, {GLFW_KEY_3, "memory"},
                {GLFW_KEY_4, "flocking"}, {GLFW_KEY_5, "cosmo"},  {GLFW_KEY_6, "cosmo_sim"},
                {GLFW_KEY_7, "physics"},  {GLFW_KEY_8, "drug"},   {GLFW_KEY_9, "audio"}};

            auto it = keyToDemoMap.find(key);
            if (it != keyToDemoMap.end())
            {
                demoManager.switchToDemo(it->second);
            }
        }
        else
        {
            // Pass other keys to the current demo
            demoManager.on_key(key);
        }
    }
}

int main()
{
    // Set environment variables to avoid libdecor-gtk issues
    // This tells libdecor to use the Cairo backend instead of GTK
    putenv((char*)"LIBDECOR_BACKEND=cairo");
    // Disable GTK decorations completely as a fallback
    putenv((char*)"LIBDECOR_NOT_GTK=1");
    // Force X11 backend to avoid Wayland compatibility issues with OpenGL
    putenv((char*)"GDK_BACKEND=x11");
    putenv((char*)"GLFW_PLATFORM=x11");

    // 1. Initialize GLFW and Create a Window
    // Set error callback to get more detailed error messages
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error " << error << ": " << description << std::endl;
    });

    std::cout << "Initializing GLFW..." << std::endl;
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Print GLFW version and runtime info
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    std::cout << "GLFW Version: " << major << "." << minor << "." << rev << std::endl;
    std::cout << "GLFW Compiled Version: " << glfwGetVersionString() << std::endl;

    // First try with compatibility profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Add hints to help with Wayland compatibility
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    std::cout << "Creating GLFW window..." << std::endl;
    std::cout << "Creating window with compatibility profile..." << std::endl;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "SEP Workbench", nullptr, nullptr);
    std::cout << "Window creation result: " << (window ? "Success" : "Failed") << std::endl;

    // If that fails, try with no specific profile (more compatible)
    if (!window)
    {
        std::cout << "Failed to create window with compatibility profile, trying fallback..."
                  << std::endl;
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        window = glfwCreateWindow(1280, 720, "SEP Workbench (Fallback)", nullptr, nullptr);

        // If that still fails, try with even more basic settings
        if (!window)
        {
            std::cout << "Failed with default hints, trying minimal configuration..." << std::endl;
            glfwDefaultWindowHints();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            window = glfwCreateWindow(1280, 720, "SEP Workbench (Legacy)", nullptr, nullptr);

            if (!window)
            {
                std::cerr << "Failed to create GLFW window with all configurations" << std::endl;
                glfwTerminate();
                return -1;
            }
        }
    }
    std::cout << "Window created. Making context current..." << std::endl;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Set key callback
    glfwSetKeyCallback(window, key_callback);

    // 2. Initialize GLEW

    // CRITICAL: Check for an OpenGL context *before* GLEW
    if (glGetString(GL_VERSION) == NULL)
    {
        std::cerr << "CRITICAL ERROR: No OpenGL context found before glewInit()!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Check OpenGL version directly
    const GLubyte* version = glGetString(GL_VERSION);
    if (version)
    {
        std::cout << "OpenGL Context OK. Version: " << version << std::endl;
    }
    else
    {
        std::cerr << "WARNING: Failed to get OpenGL version string, but context exists"
                  << std::endl;
    }

    // Set GLEW to use modern OpenGL
    std::cout << "Initializing GLEW..." << std::endl;
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
    }
    else
    {
        std::cout << "GLEW Initialized Successfully." << std::endl;
    }

    // Clear any errors that might have been generated by glewInit
    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        std::cout << "OpenGL error after GLEW init: " << glErr << " (cleared)" << std::endl;
    }

    // Set up a simple rendering state
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // 3. Initialize SEP Core Logger
    sep::logging::Manager::initialize();
    sep::logging::LoggerConfig log_config;
    log_config.name = "workbench";
    auto logger = sep::logging::Manager::getInstance().createLogger("workbench", log_config);
    logger->info("Workbench shell initialized.");

    // 4. Initialize SEP Engine and Renderer
    std::unique_ptr<sep::Engine> engine = sep::createEngine();
    std::unique_ptr<sep::CyclesRenderer> renderer = sep::createRenderer();

    engine->initialize();
    renderer->initialize();

    // 5. Initialize the demo manager
    auto& demoManager = sep::workbench::DemoManager::getInstance();
    demoManager.initialize(engine.get(), renderer.get());

    // 6. Register available demos
    sep::workbench::registerDemos();
    logger->info("Demos registered and ready.");

    // 7. Initialize ImGui with a simpler backend
    std::cout << "Initializing ImGui..." << std::endl;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize ImGui for GLFW
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    // Use a simpler OpenGL3 initialization with a basic GLSL version
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    std::cout << "ImGui initialized successfully" << std::endl;

    // 8. Initialize the default demo
    std::cout << "Initializing default demo..." << std::endl;
    try
    {
        demoManager.switchToDemo("genesis");
        std::cout << "Default demo loaded successfully" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading default demo: " << e.what() << std::endl;
        std::cerr << "Continuing with UI only mode" << std::endl;
    }

    // 9. Main Loop
    double last_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double current_time = glfwGetTime();
        float dt = static_cast<float>(current_time - last_time);
        last_time = current_time;

        glfwPollEvents();

        demoManager.on_update(dt);
        renderer->render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Clear the screen with our simple color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Check for OpenGL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error in render loop: " << err << std::endl;
        }

        // Skip demo UI rendering
        // if (demoManager.getCurrentDemo() != "")
        // {
        //     demoManager.on_ui_render();
        // }

        // Render the main UI with a more interactive interface
        ImGui::Begin(
            "SEP Workbench", nullptr,
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Set window to fill the entire screen
        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y));

        // Title and info
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f), "SEP Engine Workbench");
        ImGui::PopFont();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator();
        ImGui::Text("Select a demo to run:");
        ImGui::Spacing();

        // Define demo information
        struct DemoInfo
        {
            const char* name;
            const char* description;
            const char* key;
        };

        DemoInfo demos[] = {
            {"Genesis", "Explore the fundamental pattern generation algorithms", "1"},
            {"Neural", "Neural network visualization and training", "2"},
            {"Memory Garden", "Visualize the tiered memory system", "3"},
            {"Flocking", "Emergent behavior simulation", "4"},
            {"Cosmo", "Cosmic pattern visualization", "5"},
            {"Cosmo Sim", "Interactive cosmic simulation", "6"},
            {"Digital Physics", "Quantum-inspired physics simulation", "7"},
            {"Drug Discovery", "Molecular pattern matching demo", "8"},
            {"Audio Visualizer", "Real-time audio processing and visualization", "9"}};

        // Calculate button size and layout
        float windowWidth = ImGui::GetContentRegionAvail().x;
        float buttonWidth = windowWidth / 3.0f - 20.0f;
        float buttonHeight = 100.0f;

        // Display demo buttons in a grid
        for (int i = 0; i < IM_ARRAYSIZE(demos); i++)
        {
            // Create a new row every 3 buttons
            if (i % 3 != 0)
            {
                ImGui::SameLine();
            }

            // Create a colored button with demo name
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.2f, 0.3f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.3f, 0.4f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.4f, 0.5f, 1.0f));

            // Create a unique ID for each button
            ImGui::PushID(i);

            if (ImGui::Button(demos[i].name, ImVec2(buttonWidth, buttonHeight)))
            {
                // This would normally launch the demo, but we're skipping for now
                std::cout << "Selected demo: " << demos[i].name << std::endl;
                // demoManager.switchToDemo(demos[i].name);
            }

            ImGui::PopID();
            ImGui::PopStyleColor(3);

            // Show tooltip with description and keyboard shortcut
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s", demos[i].description);
                ImGui::Separator();
                ImGui::Text("Press %s to launch", demos[i].key);
                ImGui::EndTooltip();
            }
        }

        ImGui::Separator();
        ImGui::Text(
            "Note: Demo functionality is currently disabled due to GLEW initialization issues.");

        ImGui::End();

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 10. Cleanup
    logger->info("Shutting down workbench shell.");

    // Unload the current demo
    demoManager.on_unload();

    // Cleanup engine and renderer
    engine->shutdown();

    // Cleanup ImGui and GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
