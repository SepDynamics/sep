#pragma once

// Include GLEW before any other GL headers
#include <glad/glad.h>

#include <cmath>
#include <iostream>
#include <vector>

// Include canonical Pattern definition
#include "core/types.h"

namespace sep
{
    namespace workbench
    {
        using Pattern = ::sep::Pattern;

        class Renderer
        {
        public:
            Renderer() = default;
            ~Renderer();

            // Initialize the renderer with the given width and height
            void init(int width, int height);

            // Cleanup OpenGL resources
            void cleanup();

            // Just render a blank screen for Phase 1
            void render();
            void render(std::vector<Pattern>& patterns);
            float calculateAvgCoherence(const std::vector<Pattern>& patterns);

            // Render patterns (implementation for main.cpp)
            void render(const std::vector<Pattern>& patterns);

            // Mouse interaction handling
            bool isPointInStartButton(double x, double y) const;
            void handleMouseClick(double x, double y);

            // Check if the START button was clicked
            static bool wasStartButtonClicked();
            static void resetStartButtonClicked();

            // Set the demo manager for interaction
            void setDemoManager(void* manager) { demoManager = manager; }

        private:
            // Static variable to track if START button was clicked
            static bool startButtonClicked;

        private:
            // Setup shader program
            void setupShaders();

            // 3D shape rendering helpers
            void renderSphere(int latitudes, int longitudes, const glm::vec3& pos = glm::vec3(0.0f),
                              float scale = 1.0f, const glm::vec4& color = glm::vec4(1.0f));
            void renderCube(const glm::vec3& pos = glm::vec3(0.0f), float scale = 1.0f,
                            const glm::vec4& color = glm::vec4(1.0f));
            void drawVertices(const std::vector<float>& vertices, GLenum mode);

            // Shader program ID
            GLuint shaderProgram = 0;
            GLint colorLoc = -1;

            // Vertex Array Object and Vertex Buffer Object
            GLuint vao = 0;
            GLuint vbo = 0;

            // Window dimensions
            int width = 3440;
            int height = 1440;

            // Flag to determine if OpenGL rendering should be used
            bool useOpenGL = true;

            // Pointer to the demo manager for callbacks
            void* demoManager = nullptr;
        };

    }  // namespace workbench
}  // namespace sep