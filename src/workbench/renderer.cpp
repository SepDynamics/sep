#include "renderer.h"

#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

// Initialize static member variable
bool sep::workbench::Renderer::startButtonClicked = false;

void sep::workbench::Renderer::init(int width, int height)
{
    this->width = width;
    this->height = height;

    std::cout << "Renderer initialized with dimensions: " << width << "x" << height << std::endl;

    // Default to fallback mode until we confirm OpenGL works
    useOpenGL = false;

    try
    {
        // Get OpenGL version info for debugging
        const GLubyte* version = glGetString(GL_VERSION);
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);

        if (version && vendor && renderer)
        {
            std::cout << "OpenGL Version: " << version << std::endl;
            std::cout << "OpenGL Vendor: " << vendor << std::endl;
            std::cout << "OpenGL Renderer: " << renderer << std::endl;
        }
        else
        {
            std::cerr << "Warning: Unable to get OpenGL version info" << std::endl;
        }

        // Set a bright clear color to make it obvious if rendering is working
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set up viewport
        glViewport(0, 0, width, height);

        // Create and bind a vertex array object (VAO)
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Create and bind a vertex buffer object (VBO)
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Define the vertices for a simple triangle
        float vertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};

        // Upload the vertex data to the GPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Setup shaders
        setupShaders();

        // Enable vertex attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Unbind VAO
        glBindVertexArray(0);

        // Test if basic OpenGL functions work by clearing the screen
        glClear(GL_COLOR_BUFFER_BIT);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error during initialization: " << err << std::endl;
            throw std::runtime_error("OpenGL error during initialization");
        }

        // Set flag to indicate successful initialization
        useOpenGL = true;
        std::cout << "OpenGL renderer initialized successfully" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during renderer initialization: " << e.what() << std::endl;
        std::cerr << "Fallback mode enabled - will attempt basic rendering" << std::endl;
        useOpenGL = false;
    }
    catch (...)
    {
        std::cerr << "Unknown error during renderer initialization - fallback mode enabled"
                  << std::endl;
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
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
    }

    // Delete shaders as they're linked into the program and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    std::cout << "Shader program created with ID: " << shaderProgram << std::endl;
}

void sep::workbench::Renderer::render()
{
    try
    {
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (useOpenGL && shaderProgram != 0 && vao != 0)
        {
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            glUseProgram(0);
            static bool success_logged = false;
            if (!success_logged)
            {
                std::cout << "Successfully rendered triangle with modern OpenGL!" << std::endl;
                success_logged = true;
            }
        }
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            std::cerr << "OpenGL error during rendering: " << err << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
        useOpenGL = false;
    }
    catch (...)
    {
        std::cerr << "Unknown error during rendering" << std::endl;
        useOpenGL = false;
    }
}

void sep::workbench::Renderer::render(const std::vector<sep::workbench::Pattern>& patterns)
{
    try
    {
        glClearColor(0.7f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        if (useOpenGL && shaderProgram != 0 && vao != 0)
        {
            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            float spacing = 1.5f;
            int perRow = static_cast<int>(std::sqrt(patterns.size())) + 1;
            float startX = -((perRow - 1) * spacing) / 2.0f;
            float startZ = -((perRow - 1) * spacing) / 2.0f;
            for (size_t i = 0; i < patterns.size() && i < 100; ++i)
            {
                const auto& p = patterns[i];
                int row = i / perRow;
                int col = i % perRow;
                glm::vec3 pos(p.position.x != 0.0f ? p.position.x : (startX + col * spacing),
                              p.position.y != 0.0f ? p.position.y : 0.0f,
                              p.position.z != 0.0f ? p.position.z : (startZ + row * spacing));
                float s = 1.0f;
                glm::vec4 c(1.0f);
                glUniform4f(colorLoc, c.r, c.g, c.b, c.a);
                if (p.quantum_state.memory_tier == sep::memory::MemoryTierEnum::SHORT_TERM)
                    renderSphere(8, 8, pos, s, c);
                else
                    renderCube(pos, s, c);
            }
            glBindVertexArray(0);
            glUseProgram(0);
        }
        glDisable(GL_DEPTH_TEST);
    }
    catch (...)
    {
        useOpenGL = false;
    }
}
// Helper method to render a sphere
void sep::workbench::Renderer::renderSphere(int latitudes, int longitudes, const glm::vec3& pos,
                                            float scale, const glm::vec4& color)
{
    std::vector<float> verts;
    float r = 0.5f * scale;
    for (int i = 0; i < latitudes; ++i)
    {
        float lat0 = M_PI * (-0.5f + (float)i / latitudes);
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / latitudes);
        for (int j = 0; j <= longitudes; ++j)
        {
            float lng0 = 2 * M_PI * (float)j / longitudes;
            float lng1 = 2 * M_PI * (float)(j + 1) / longitudes;
            glm::vec3 v0 =
                pos + r * glm::vec3(cos(lng0) * cos(lat0), sin(lat0), sin(lng0) * cos(lat0));
            glm::vec3 v1 =
                pos + r * glm::vec3(cos(lng0) * cos(lat1), sin(lat1), sin(lng0) * cos(lat1));
            glm::vec3 v2 =
                pos + r * glm::vec3(cos(lng1) * cos(lat1), sin(lat1), sin(lng1) * cos(lat1));
            glm::vec3 v3 =
                pos + r * glm::vec3(cos(lng1) * cos(lat0), sin(lat0), sin(lng1) * cos(lat0));
            verts.insert(verts.end(), {v0.x, v0.y, v0.z, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z});
            verts.insert(verts.end(), {v0.x, v0.y, v0.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z});
        }
    }
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    drawVertices(verts, GL_TRIANGLES);
}

void sep::workbench::Renderer::renderCube(const glm::vec3& pos, float scale, const glm::vec4& color)
{
    float s = 0.5f * scale;
    std::vector<float> verts = {// front
                                pos.x - s, pos.y - s, pos.z + s, pos.x + s, pos.y - s, pos.z + s,
                                pos.x + s, pos.y + s, pos.z + s, pos.x - s, pos.y - s, pos.z + s,
                                pos.x + s, pos.y + s, pos.z + s, pos.x - s, pos.y + s, pos.z + s,
                                // back
                                pos.x - s, pos.y - s, pos.z - s, pos.x - s, pos.y + s, pos.z - s,
                                pos.x + s, pos.y + s, pos.z - s, pos.x - s, pos.y - s, pos.z - s,
                                pos.x + s, pos.y + s, pos.z - s, pos.x + s, pos.y - s, pos.z - s,
                                // left
                                pos.x - s, pos.y - s, pos.z - s, pos.x - s, pos.y - s, pos.z + s,
                                pos.x - s, pos.y + s, pos.z + s, pos.x - s, pos.y - s, pos.z - s,
                                pos.x - s, pos.y + s, pos.z + s, pos.x - s, pos.y + s, pos.z - s,
                                // right
                                pos.x + s, pos.y - s, pos.z - s, pos.x + s, pos.y + s, pos.z - s,
                                pos.x + s, pos.y + s, pos.z + s, pos.x + s, pos.y - s, pos.z - s,
                                pos.x + s, pos.y + s, pos.z + s, pos.x + s, pos.y - s, pos.z + s,
                                // top
                                pos.x - s, pos.y + s, pos.z - s, pos.x - s, pos.y + s, pos.z + s,
                                pos.x + s, pos.y + s, pos.z + s, pos.x - s, pos.y + s, pos.z - s,
                                pos.x + s, pos.y + s, pos.z + s, pos.x + s, pos.y + s, pos.z - s,
                                // bottom
                                pos.x - s, pos.y - s, pos.z - s, pos.x + s, pos.y - s, pos.z - s,
                                pos.x + s, pos.y - s, pos.z + s, pos.x - s, pos.y - s, pos.z - s,
                                pos.x + s, pos.y - s, pos.z + s, pos.x - s, pos.y - s, pos.z + s};
    glUniform4f(colorLoc, color.r, color.g, color.b, color.a);
    drawVertices(verts, GL_TRIANGLES);
}

// Non-const version of render that delegates to the const version
sep::workbench::Renderer::~Renderer()
{
    // Call cleanup to release OpenGL resources
    cleanup();
};

void sep::workbench::Renderer::cleanup()
{
    // Only cleanup if OpenGL was successfully initialized
    if (useOpenGL)
    {
        std::cout << "Cleaning up OpenGL resources..." << std::endl;

        // Delete shader program
        if (shaderProgram != 0)
        {
            glDeleteProgram(shaderProgram);
            shaderProgram = 0;
        }

        // Delete VAO
        if (vao != 0)
        {
            glDeleteVertexArrays(1, &vao);
            vao = 0;
        }

        // Delete VBO
        if (vbo != 0)
        {
            glDeleteBuffers(1, &vbo);
            vbo = 0;
        }

        std::cout << "OpenGL resources cleaned up successfully." << std::endl;
    }
}

void sep::workbench::Renderer::drawVertices(const std::vector<float>& vertices, GLenum mode)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glDrawArrays(mode, 0, vertices.size() / 3);
}
// Check if a point is inside the START button area
bool sep::workbench::Renderer::isPointInStartButton(double x, double y) const
{
    // The START text is drawn above the square
    // The square is at (width/2 - 150, height/2 - 150) to (width/2 + 150, height/2 + 150)
    // The START text is at (width/2 - 100, height/2 + 170) to (width/2 + 100, height/2 + 220)

    // Convert screen coordinates to our coordinate system
    // In OpenGL, y=0 is at the bottom, but in screen coordinates, y=0 is at the top
    y = height - y;

    // Check if the point is within the START text area
    return (x >= width / 2 - 100 && x <= width / 2 + 100 && y >= height / 2 + 170 &&
            y <= height / 2 + 220);
}

// Handle mouse click
void sep::workbench::Renderer::handleMouseClick(double x, double y)
{
    if (isPointInStartButton(x, y) && demoManager != nullptr)
    {
        // Instead of casting to a specific type, we'll use a simpler approach
        // The main.cpp file will handle the actual switching to the genesis demo
        std::cout << "START button clicked, requesting switch to genesis demo" << std::endl;

        // Signal that the START button was clicked
        // The main application can check this and take appropriate action
        startButtonClicked = true;
    }
}

// Static method implementations
bool sep::workbench::Renderer::wasStartButtonClicked() { return startButtonClicked; }

void sep::workbench::Renderer::resetStartButtonClicked() { startButtonClicked = false; }

void sep::workbench::Renderer::render(std::vector<sep::workbench::Pattern>& patterns)
{
    // Call the const version to avoid code duplication
    render(static_cast<const std::vector<sep::workbench::Pattern>&>(patterns));

    // Additional logging for debugging
    static bool first_time = true;
    if (first_time)
    {
        std::cout << "Patterns: " << patterns.size()
                  << " | Avg Coherence: " << calculateAvgCoherence(patterns) << std::endl;
        first_time = false;
    }
}

float sep::workbench::Renderer::calculateAvgCoherence(
    const std::vector<sep::workbench::Pattern>& patterns)
{
    if (patterns.empty()) return 0.0f;

    float total = 0.0f;
    for (const auto& pattern : patterns)
    {
        total += pattern.quantum_state.coherence;
    }
    return total / patterns.size();
}
