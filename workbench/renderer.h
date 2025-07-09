#pragma once

// Include GLEW before any other GL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <vector>

// Include our Pattern definition
#include "pattern.hpp"

namespace sep {
namespace workbench {

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

    // Initialize the renderer with the given width and height
    void init(int width, int height);

    // Just render a blank screen for Phase 1
    void render();
    void render(std::vector<Pattern>& patterns);
    float calculateAvgCoherence(const std::vector<Pattern>& patterns);

    // Render patterns (implementation for main.cpp)
    void render(const std::vector<Pattern>& patterns);

private:
    // Setup shader program
    void setupShaders();

    // Shader program ID
    GLuint shaderProgram = 0;

    // Vertex Array Object and Vertex Buffer Object
    GLuint vao = 0;
    GLuint vbo = 0;

    // Window dimensions
    int width = 3440;
    int height = 1440;

    // Flag to determine if OpenGL rendering should be used
    bool useOpenGL = true;
};

}  // namespace workbench
}  // namespace sep