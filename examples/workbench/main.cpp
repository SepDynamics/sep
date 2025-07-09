#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../../src/window.h"

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        return -1;
    }

    // Create window using sep::workbench::Window
    sep::workbench::Window window(1280, 720, "SEP Workbench");
    if (!window.getGLFWWindow())
    {
        glfwTerminate();
        return -1;
    }
    window.makeContextCurrent();

    // Initialize GLEW after making context current
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    // Main loop
    while (!window.shouldClose())
    {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.swapBuffers();
    }

    glfwTerminate();
    return 0;
}
