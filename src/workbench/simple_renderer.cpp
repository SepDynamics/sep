#include "simple_renderer.h"
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sep {

// Simple vertex shader
const char* vertex_shader_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = aColor;
    gl_PointSize = 4.0;
}
)";

// Simple fragment shader
const char* fragment_shader_source = R"(
#version 330 core
in vec3 color;
out vec4 FragColor;

void main() {
    FragColor = vec4(color, 1.0);
}
)";

SimpleRenderer::SimpleRenderer() {
    state_.projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
    state_.view = glm::lookAt(state_.camera_pos, state_.camera_target, glm::vec3(0, 1, 0));
}

SimpleRenderer::~SimpleRenderer() {
    cleanup();
}

bool SimpleRenderer::initialize() {
    if (initialized_) return true;

    // Create shader program
    shader_program_ = createShaderProgram();
    if (shader_program_ == 0) {
        std::cerr << "Failed to create shader program\n";
        return false;
    }

    // Generate buffers
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &color_vbo_);

    initialized_ = true;
    return true;
}

void SimpleRenderer::cleanup() {
    if (!initialized_) return;
    
    if (vao_ != 0) glDeleteVertexArrays(1, &vao_);
    if (vbo_ != 0) glDeleteBuffers(1, &vbo_);
    if (color_vbo_ != 0) glDeleteBuffers(1, &color_vbo_);
    if (shader_program_ != 0) glDeleteProgram(shader_program_);
    
    initialized_ = false;
}

uint32_t SimpleRenderer::compileShader(const std::string& source, uint32_t type) {
    uint32_t shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cerr << "Shader compilation failed: " << info << std::endl;
        return 0;
    }
    return shader;
}

uint32_t SimpleRenderer::createShaderProgram() {
    uint32_t vertex_shader = compileShader(vertex_shader_source, GL_VERTEX_SHADER);
    uint32_t fragment_shader = compileShader(fragment_shader_source, GL_FRAGMENT_SHADER);
    
    if (vertex_shader == 0 || fragment_shader == 0) return 0;

    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, nullptr, info);
        std::cerr << "Shader program linking failed: " << info << std::endl;
        return 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program;
}

glm::vec3 SimpleRenderer::getColorForMode(const std::string& mode, int index, float value) {
    if (mode == "rainbow") {
        float hue = fmod(index * 0.618f, 1.0f); // Golden ratio for nice distribution
        float r = abs(6.0f * hue - 3.0f) - 1.0f;
        float g = 2.0f - abs(6.0f * hue - 2.0f);
        float b = 2.0f - abs(6.0f * hue - 4.0f);
        return glm::clamp(glm::vec3(r, g, b), 0.0f, 1.0f);
    } else if (mode == "temperature") {
        float t = glm::clamp(value, 0.0f, 1.0f);
        return glm::vec3(t, t * 0.5f, 1.0f - t);
    } else if (mode == "density") {
        float d = glm::clamp(value, 0.0f, 1.0f);
        return glm::vec3(d, d, d);
    }
    return glm::vec3(0.5f, 0.7f, 1.0f); // Default blue
}

void SimpleRenderer::renderPatternState(const std::vector<glm::vec3>& positions) {
    std::vector<glm::vec3> colors;
    for (size_t i = 0; i < positions.size(); ++i) {
        colors.push_back(getColorForMode(state_.color_mode, i));
    }
    renderParticles(positions, colors);
}

void SimpleRenderer::renderParticles(const std::vector<glm::vec3>& positions, const std::vector<glm::vec3>& colors) {
    if (!initialized_ || positions.empty()) return;

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);

    // Upload position data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    // Upload color data
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo_);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    // Set uniforms
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(state_.rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(state_.zoom));
    
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_FALSE, glm::value_ptr(state_.view));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "projection"), 1, GL_FALSE, glm::value_ptr(state_.projection));

    // Render
    if (state_.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    glDrawArrays(GL_POINTS, 0, positions.size());
    
    glBindVertexArray(0);
}

void SimpleRenderer::renderLines(const std::vector<glm::vec3>& points) {
    if (!initialized_ || points.empty()) return;

    std::vector<glm::vec3> colors;
    for (size_t i = 0; i < points.size(); ++i) {
        colors.push_back(getColorForMode(state_.color_mode, i));
    }

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec3), points.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo_);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(state_.rotation), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(state_.zoom));
    
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "view"), 1, GL_FALSE, glm::value_ptr(state_.view));
    glUniformMatrix4fv(glGetUniformLocation(shader_program_, "projection"), 1, GL_FALSE, glm::value_ptr(state_.projection));

    glDrawArrays(GL_LINES, 0, points.size());
    glBindVertexArray(0);
}

void SimpleRenderer::renderMesh(const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices) {
    if (!initialized_ || vertices.empty()) return;
    
    std::vector<glm::vec3> colors;
    for (size_t i = 0; i < vertices.size(); ++i) {
        colors.push_back(getColorForMode(state_.color_mode, i));
    }
    
    renderParticles(vertices, colors);
}

void SimpleRenderer::setColorMode(const std::string& mode) {
    state_.color_mode = mode;
}

void SimpleRenderer::setEmissionMode(const std::string& mode) {
    state_.emission_mode = mode;
}

void SimpleRenderer::setRotation(float rotation) {
    state_.rotation = rotation;
}

void SimpleRenderer::setZoom(float zoom) {
    state_.zoom = zoom;
}

void SimpleRenderer::setWireframe(bool enabled) {
    state_.wireframe = enabled;
}

void SimpleRenderer::cycleColorMode() {
    static const std::vector<std::string> modes = {"rainbow", "temperature", "density", "normal"};
    auto it = std::find(modes.begin(), modes.end(), state_.color_mode);
    if (it != modes.end()) {
        ++it;
        if (it == modes.end()) it = modes.begin();
        state_.color_mode = *it;
    } else {
        state_.color_mode = modes[0];
    }
}

void SimpleRenderer::setCamera(const glm::vec3& position, const glm::vec3& target) {
    state_.camera_pos = position;
    state_.camera_target = target;
    state_.view = glm::lookAt(state_.camera_pos, state_.camera_target, glm::vec3(0, 1, 0));
}

void SimpleRenderer::updateProjection(int width, int height) {
    float aspect = width > 0 && height > 0 ? (float)width / (float)height : 1.0f;
    state_.projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

} // namespace sep
