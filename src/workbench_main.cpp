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
    // These environment variables must be set before any libraries are loaded
    // Set environment variables to avoid libdecor-gtk issues
    setenv("LIBDECOR_BACKEND", "cairo", 1);
    setenv("LIBDECOR_NOT_GTK", "1", 1);
    // Force X11 backend to avoid Wayland compatibility issues with OpenGL
    setenv("GDK_BACKEND", "x11", 1);
    setenv("GLFW_PLATFORM", "x11", 1);
    // Tell GLFW to use X11 explicitly
    setenv("GLFW_USE_WAYLAND", "0", 1);

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

    // Set up a simple rendering state - do this before GLEW
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Set GLEW to use modern OpenGL - try with and without experimental flag
    std::cout << "Initializing GLEW..." << std::endl;
    
    // First try with experimental flag
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    // If that fails, try without experimental flag
    if (err != GLEW_OK) {
        std::cerr << "GLEW initialization with experimental flag failed, trying without..." << std::endl;
        glewExperimental = GL_FALSE;
        err = glewInit();
    }
    
    if (err != GLEW_OK)
    {
        std::cerr << "GLEW Error: " << glewGetErrorString(err) << std::endl;
        // Continue anyway - some platforms report errors but still work
    }
    else
    {
        std::cout << "GLEW Initialized Successfully." << std::endl;
    }

    // Clear any errors that might have been generated by glewInit
    // This is normal and can be ignored
    while (glGetError() != GL_NO_ERROR) {
        // Just clear the error queue
    }

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

    // 7. Create a global flag to track if ImGui is available
    bool imgui_available = false;
    
    // Skip ImGui if GLEW had errors - this prevents the segfault
    if (err != GLEW_OK) {
        std::cerr << "GLEW initialization failed, skipping ImGui..." << std::endl;
        logger->warn("GLEW initialization failed, operating in minimal mode");
    } else {
        std::cout << "Initializing ImGui..." << std::endl;
        
        // Try to initialize ImGui with minimal dependencies
        try {
            // Create ImGui context first
            ImGui::CreateContext();
            
            // Only proceed if context creation succeeded
            if (ImGui::GetCurrentContext() != nullptr) {
                // Use very basic styling
                ImGui::StyleColorsDark();
                
                // Use the most basic GLSL version possible
                const char* glsl_version = "#version 110";
                
                // Initialize GLFW backend with immediate error check
                if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
                    throw std::runtime_error("ImGui GLFW backend init failed");
                }
                
                // Initialize OpenGL backend with immediate error check
                if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
                    // Clean up GLFW backend if OpenGL init fails
                    ImGui_ImplGlfw_Shutdown();
                    throw std::runtime_error("ImGui OpenGL backend init failed");
                }
                
                imgui_available = true;
                std::cout << "ImGui initialized successfully" << std::endl;
            } else {
                throw std::runtime_error("ImGui context creation failed");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "ImGui initialization failed: " << e.what() << std::endl;
            logger->error("ImGui initialization failed: {}", e.what());
            
            // Make sure ImGui is fully shut down if partial initialization occurred
            if (ImGui::GetCurrentContext() != nullptr) {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
            }
        }
        catch (...) {
            std::cerr << "Unknown exception during ImGui initialization" << std::endl;
            logger->error("Unknown exception during ImGui initialization");
            
            // Make sure ImGui is fully shut down if partial initialization occurred
            if (ImGui::GetCurrentContext() != nullptr) {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
            }
        }
    }

    // 8. Connect to the SEP API server instead of loading demos directly
    std::cout << "Connecting to SEP API server..." << std::endl;
    
    try {
        // For now, just log that we're connecting - we'll implement actual API client later
        logger->info("Connected to SEP API server on port 3000");
        
        // Skip demo loading for now as it's causing segfaults
        // We'll implement demo loading through the API server later
        logger->info("Demos will be loaded through API server");
    }
    catch (const std::exception& e) {
        std::cerr << "Error connecting to API server: " << e.what() << std::endl;
        std::cerr << "Continuing with UI only mode" << std::endl;
    }

    // 9. Main Loop
    double last_time = glfwGetTime();
    bool rendering_error = false;

    while (!glfwWindowShouldClose(window))
    {
        double current_time = glfwGetTime();
        float dt = static_cast<float>(current_time - last_time);
        last_time = current_time;

        glfwPollEvents();

        // Try to update demo manager, but catch any exceptions
        try {
            if (!demoManager.getCurrentDemo().empty()) {
                demoManager.on_update(dt);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error in demo update: " << e.what() << std::endl;
            logger->error("Demo update error: {}", e.what());
        }

        // Try to render, but if it fails, just continue
        try {
            if (!rendering_error) {
                renderer->render();
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Renderer error: " << e.what() << std::endl;
            logger->error("Renderer error: {}", e.what());
            rendering_error = true;
        }

        // Clear the screen with our simple color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Clear any OpenGL errors that might have accumulated
        while (glGetError() != GL_NO_ERROR) {
            // Just clear the error queue
        }

        // Only use ImGui if it was successfully initialized
        if (imgui_available) {
            try {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
            }
            catch (const std::exception& e) {
                std::cerr << "ImGui error: " << e.what() << std::endl;
                logger->error("ImGui error: {}", e.what());
                imgui_available = false;  // Disable ImGui if there's an error
            }
        }

        if (!imgui_available) {
            // Just clear the screen with a color to show we're alive
            glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

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
