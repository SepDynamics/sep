#include <GL/glew.h>  // MUST be included before any other GL headers
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

// Define the sep::workbench namespace and classes here directly
namespace sep {
    // Basic result enum
    enum class SEPResult {
        SUCCESS,
        ERROR
    };

    namespace quantum {
        // Simple Pattern representation
        struct Pattern {
            struct QuantumState {
                float coherence = 0.5f;
                float stability = 0.5f;
                int generation = 1;
            };

            std::string id;
            QuantumState quantum_state;
            std::vector<float> values;

            // Simple evolution method
            void evolve(float rate) {
                // Simple evolution simulation
                quantum_state.coherence = std::clamp(
                    quantum_state.coherence + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
                    0.0f, 1.0f);

                quantum_state.stability = std::clamp(
                    quantum_state.stability + (((float)rand() / RAND_MAX) * 0.2f - 0.1f) * rate,
                    0.0f, 1.0f);

                quantum_state.generation++;
            }
        };
    }  // namespace quantum

    namespace workbench {
        // Simple window wrapper
        class Window {
        public:
            Window(int width, int height, const std::string& title)
                : width_(width), height_(height), title_(title) {
                // Create GLFW window
                window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            }

            ~Window() {
                if (window_) {
                    glfwDestroyWindow(window_);
                }
            }

            bool initialize() {
                if (!window_) {
                    return false;
                }

                // Initialize GLFW
                if (!glfwInit()) {
                    return false;
                }

                makeContextCurrent();

                // Initialize GLEW
                GLenum err = glewInit();
                if (err != GLEW_OK) {
                    std::cerr << "GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
                    return false;
                }

                return true;
            }

            void makeContextCurrent() {
                glfwMakeContextCurrent(window_);
            }

            void pollEvents() {
                glfwPollEvents();
            }

            bool shouldClose() {
                return glfwWindowShouldClose(window_);
            }

            void swapBuffers() {
                glfwSwapBuffers(window_);
            }

            GLFWwindow* getHandle() { return window_; }
            int getWidth() const { return width_; }
            int getHeight() const { return height_; }

        private:
            GLFWwindow* window_ = nullptr;
            int width_ = 0;
            int height_ = 0;
            std::string title_;
        };

        // Simple renderer
        class Renderer {
        public:
            void init(int width, int height) {
                width_ = width;
                height_ = height;
                
                // Set up OpenGL settings
                glViewport(0, 0, width, height);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }

            void render(const std::vector<sep::quantum::Pattern>& patterns) {
                // Very simple rendering - just draws circles for each pattern
                for (const auto& pattern : patterns) {
                    float x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                    float y = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                    float size = 0.05f + pattern.quantum_state.coherence * 0.1f;
                    float r = pattern.quantum_state.coherence;
                    float g = pattern.quantum_state.stability;
                    float b = 1.0f - pattern.quantum_state.coherence;
                    
                    drawCircle(x, y, size, r, g, b);
                }
            }

        private:
            void drawCircle(float x, float y, float radius, float r, float g, float b) {
                const int segments = 36;
                glBegin(GL_TRIANGLE_FAN);
                glColor4f(r, g, b, 0.7f);
                glVertex2f(x, y);  // Center
                for (int i = 0; i <= segments; i++) {
                    float angle = 2.0f * 3.14159f * i / segments;
                    glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
                }
                glEnd();
            }

            int width_ = 0;
            int height_ = 0;
        };
    }  // namespace workbench
}  // namespace sep

class GenesisPatternDemo
{
public:
    GenesisPatternDemo() = default;
    ~GenesisPatternDemo() = default;

    bool init()
    {
        std::cout << "Initializing Genesis Pattern Demo..." << std::endl;

        // Create window
        window = new sep::workbench::Window(1280, 720, "SEP Workbench");
        if (!window->getHandle())
        {
            std::cerr << "Failed to create window" << std::endl;
            return false;
        }

        window->makeContextCurrent();

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
    sep::workbench::Renderer renderer;
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
