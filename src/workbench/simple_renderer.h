#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace sep {

// Simple OpenGL-based renderer to replace CyclesRenderer for workbench demos
class SimpleRenderer {
public:
    SimpleRenderer();
    ~SimpleRenderer();

    // Initialize OpenGL context and resources
    bool initialize();
    
    // Cleanup resources
    void cleanup();

    // Rendering methods
    void renderPatternState(const std::vector<glm::vec3>& positions);
    void renderParticles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors);
    void renderLines(const std::vector<glm::vec3>& points);
    void renderMesh(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices);

    // Visualization settings
    void setColorMode(const std::string& mode);
    void setEmissionMode(const std::string& mode);
    void setRotation(float rotation);
    void setZoom(float zoom);
    void setWireframe(bool enabled);
    void cycleColorMode();

    // Camera controls
    void setCamera(const glm::vec3& position, const glm::vec3& target);
    void updateProjection(int width, int height);

private:
    struct RenderState {
        std::string color_mode = "rainbow";
        std::string emission_mode = "normal";
        float rotation = 0.0f;
        float zoom = 1.0f;
        bool wireframe = false;
        glm::vec3 camera_pos = glm::vec3(0, 0, 5);
        glm::vec3 camera_target = glm::vec3(0, 0, 0);
        glm::mat4 projection;
        glm::mat4 view;
    } state_;

    // OpenGL resources
    uint32_t shader_program_;
    uint32_t vao_;
    uint32_t vbo_;
    uint32_t color_vbo_;

    bool initialized_ = false;

    // Shader compilation helpers
    uint32_t compileShader(const std::string& source, uint32_t type);
    uint32_t createShaderProgram();
    glm::vec3 getColorForMode(const std::string& mode, int index, float value = 0.0f);
};



} // namespace sep
