#include "window.h"

#include <GLFW/glfw3.h>

#include <iostream>

namespace sep
{
    namespace workbench
    {

        Window::Window(int width, int height, const std::string& title)
            : width(width), height(height), title(title), window(nullptr)
        {
            std::cout << "Creating window: " << title << " (" << width << "x" << height << ")"
                      << std::endl;

            // Initialize GLFW
            if (!glfwInit())
            {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                return;
            }

            // Use the simplest possible OpenGL configuration
            glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

            // Create window
            window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            if (!window)
            {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return;
            }

            // Make the window's context current
            glfwMakeContextCurrent(window);

            // Skip GLEW initialization for now - we'll use basic OpenGL commands only
            std::cout << "Using basic OpenGL mode (no GLEW)" << std::endl;

            // Enable vsync
            glfwSwapInterval(1);

            std::cout << "Window created successfully" << std::endl;
        }

        Window::~Window()
        {
            if (window)
            {
                glfwDestroyWindow(window);
                glfwTerminate();
            }
        }

        void Window::makeContextCurrent()
        {
            if (window)
            {
                glfwMakeContextCurrent(window);
            }
        }

        bool Window::shouldClose() const { return window ? glfwWindowShouldClose(window) : true; }

        void Window::pollEvents() { glfwPollEvents(); }

        void Window::swapBuffers()
        {
            if (window)
            {
                glfwSwapBuffers(window);
            }
        }

    }  // namespace workbench
}  // namespace sep