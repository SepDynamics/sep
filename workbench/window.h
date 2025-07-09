#pragma once

#include <string>
struct GLFWwindow;

namespace sep
{
    namespace workbench
    {

        class Window
        {
        public:
            // Update constructor to match the implementation in window.cpp
            Window(int width, int height, const std::string& title);
            ~Window();

            bool initialize();
            void beginFrame();
            void endFrame();
            void pollEvents();
            bool shouldClose() const;  // Make it const to match implementation
            void cleanup();

            // Add missing functions that are used in pattern_main.cpp
            void makeContextCurrent();
            void swapBuffers();
            GLFWwindow* getHandle() const { return window; }
            int getWidth() const { return width; }
            int getHeight() const { return height; }

            // Get the GLFWwindow pointer for callbacks
            GLFWwindow* getGLFWWindow() { return window; }

        private:
            std::string title;
            int width;
            int height;
            GLFWwindow* window;
        };

    }  // namespace workbench
}  // namespace sep
