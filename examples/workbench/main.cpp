#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "window.h"

int main() {
    if (!glfwInit()) {
        return -1;
    }

    sep::workbench::Window window(800, 600, "SEP Example");
    window.makeContextCurrent();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return -1;
    }

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    while (!window.shouldClose()) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.swapBuffers();
    }
    return 0;
}
