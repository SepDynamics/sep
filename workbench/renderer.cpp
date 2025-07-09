#include "renderer.h"

#include <cmath>
#include <iostream>

void Renderer::init(int width, int height)
{
    this->width = width;
    this->height = height;

    std::cout << "Renderer initialized with dimensions: " << width << "x" << height << std::endl;

    // Set clear color (background) in a try-catch to handle potential errors
    try
    {
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

        // Enable blending for transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set up viewport
        glViewport(0, 0, width, height);

        // Use orthographic projection for 2D rendering
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

        // Switch back to modelview matrix for rendering
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Setup shaders in a safe way (for OpenGL 2.1 we can use fixed function pipeline)
        setupShaders();
    }
    catch (...)
    {
        std::cerr << "Error during renderer initialization - fallback mode enabled" << std::endl;
        useOpenGL = false;
    }
}

void Renderer::setupShaders()
{
    // Simple shader setup for now
    std::cout << "Setting up shaders..." << std::endl;

    // Using fixed function pipeline for OpenGL 2.1
    // No need for shaders
    shaderProgram = 1;  // Dummy value to indicate initialization
}

void Renderer::render()
{
    if (!useOpenGL)
    {
        // Skip rendering in fallback mode
        return;
    }

    try
    {
        // Basic rendering functionality - clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw a demo pattern in the center
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Draw a simple blue square in the center
        glColor4f(0.0f, 0.5f, 1.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2f(width / 2 - 100, height / 2 - 100);
        glVertex2f(width / 2 + 100, height / 2 - 100);
        glVertex2f(width / 2 + 100, height / 2 + 100);
        glVertex2f(width / 2 - 100, height / 2 + 100);
        glEnd();
    }
    catch (...)
    {
        std::cerr << "Error during rendering - falling back to console mode" << std::endl;
        useOpenGL = false;
    }
}

void Renderer::render(const std::vector<sep::quantum::Pattern>& patterns)
{
    if (!useOpenGL)
    {
        // Fallback to console rendering for patterns
        static bool first_time = true;
        if (first_time)
        {
            std::cout << "Rendering " << patterns.size() << " patterns (console mode)" << std::endl;
            first_time = false;
        }
        return;
    }

    try
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up orthographic projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, width, 0.0, height, -1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Calculate pattern grid layout
        int patternSize = 80;  // Size of each pattern visualization
        int padding = 10;      // Padding between patterns
        int patternsPerRow = (width - padding) / (patternSize + padding);
        if (patternsPerRow < 1) patternsPerRow = 1;  // Safety check

        // Draw patterns as colorful squares with simplified logic
        for (size_t i = 0; i < patterns.size() && i < 100; i++)
        {  // Limit to 100 patterns max for safety
            const auto& pattern = patterns[i];

            // Calculate position in grid
            int row = i / patternsPerRow;
            int col = i % patternsPerRow;

            float x = padding + col * (patternSize + padding);
            float y = height - (padding + (row + 1) * (patternSize + padding));

            // Get coherence value (between 0 and 1)
            float coherence = 0.5f;  // Default
            if (!pattern.values.empty())
            {
                // Use first value as coherence for visualization
                coherence = pattern.values[0];
                // Clamp to [0, 1]
                coherence = std::max(0.0f, std::min(1.0f, coherence));
            }

            // Generate simple color based on pattern values
            float r = 0.2f + coherence * 0.8f;
            float g = 0.1f + (1.0f - coherence) * 0.7f;
            float b = 0.3f + std::sin(coherence * 3.14159f) * 0.7f;

            // Draw square - simple version
            glColor4f(r, g, b, 0.8f);
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + patternSize, y);
            glVertex2f(x + patternSize, y + patternSize);
            glVertex2f(x, y + patternSize);
            glEnd();

            // Draw simple border
            glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y);
            glVertex2f(x + patternSize, y);
            glVertex2f(x + patternSize, y + patternSize);
            glVertex2f(x, y + patternSize);
            glEnd();
        }
    }
    catch (...)
    {
        std::cerr << "Error during pattern rendering - falling back to console mode" << std::endl;
        useOpenGL = false;
    }
}

// Fix the non-const version of render to match the header file
void Renderer::render(std::vector<sep::quantum::Pattern>& patterns)
{
    // Simple implementation to visualize patterns
    std::cout << "Patterns: " << patterns.size()
              << " | Avg Coherence: " << calculateAvgCoherence(patterns) << std::endl;
}

float Renderer::calculateAvgCoherence(const std::vector<sep::quantum::Pattern>& patterns)
{
    if (patterns.empty()) return 0.0f;

    float total = 0.0f;
    for (const auto& pattern : patterns)
    {
        total += pattern.quantum_state.coherence;
    }
    return total / patterns.size();
}
