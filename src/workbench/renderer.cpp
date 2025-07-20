#include "renderer.h"

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

// Static member
bool sep::workbench::Renderer::startButtonClicked = false;

void sep::workbench::Renderer::init(int width, int height)
{
    this->width = width;
    this->height = height;

    // Check if GL context is valid
    if (glGetString(GL_VERSION) == nullptr)
    {
        std::cerr << "No valid OpenGL context" << std::endl;
        useOpenGL = false;
        return;
    }

    try
    {
        // Print GL info
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, width, height);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Simple triangle vertices
        float vertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        setupShaders();

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glClear(GL_COLOR_BUFFER_BIT);
        if (glGetError() != GL_NO_ERROR)
        {
            throw std::runtime_error("OpenGL error during init");
        }

        useOpenGL = true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Renderer init failed: " << e.what() << " - Using fallback" << std::endl;
        useOpenGL = false;
    }
}

void sep::workbench::Renderer::setupShaders()
{
    std::cout << "Setting up shaders..." << std::endl;

    // Vertex shader source code
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        void main()
        {
            gl_Position = vec4(aPos, 1.0);
        }
    )";

    // Fragment shader source code
    const char* fragmentShaderSource = R"(
        #version 330 core
        uniform vec4 uColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = uColor;
        }
    )";

    // Create and compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);  // FIXED: Declare here
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        throw std::runtime_error("Vertex shader compilation failed");
    }

    // Create and compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        throw std::runtime_error("Fragment shader compilation failed");
    }

    // Create shader program and link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        throw std::runtime_error("Shader program linking failed");
    }

    // Delete shaders as they're linked into the program and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    std::cout << "Shader program created with ID: " << shaderProgram << std::endl;
}

void sep::workbench::Renderer::render()
{
    if (!useOpenGL)
    {
        return;  // Fallback: do nothing
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
    if (glGetError() != GL_NO_ERROR)
    {
        useOpenGL = false;  // Disable if error
    }
}

// [Rest of your code remains similar, but fix integer division:]
// In isPointInStartButton:
bool sep::workbench::Renderer::isPointInStartButton(double x, double y) const
{
    // Complete the condition based on your original code
    return (x >= static_cast<double>(width) / 2.0 - 100.0 &&
            x <= static_cast<double>(width) / 2.0 + 100.0 &&
            y >= static_cast<double>(height) / 2.0 + 170.0 &&
            y <= static_cast<double>(height) / 2.0 +
                     220.0);  // FIXED: Complete expression, no ellipsis
}

// ... [rest of file unchanged]