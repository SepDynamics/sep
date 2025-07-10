#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

// Pattern structure for rendering
struct Pattern {
    glm::vec3 position;
    float intensity;
    glm::vec3 color;
    
    Pattern() : position(0.0f), intensity(1.0f), color(1.0f) {}
    
    Pattern(const glm::vec3& pos, float intens = 1.0f, const glm::vec3& col = glm::vec3(1.0f))
        : position(pos), intensity(intens), color(col) {}
};

// Base renderer class
class Renderer {
public:
    Renderer();
    virtual ~Renderer();
    
    // Singleton pattern
    static Renderer* getInstance();
    
    // Initialize the renderer with window dimensions
    virtual void init(int width, int height);
    
    // Basic render method
    virtual void render();
    
    // Render a collection of patterns
    virtual void render(std::vector<Pattern>& patterns);
    
protected:
    int m_width;
    int m_height;
    bool m_initialized;
};

} // namespace workbench
} // namespace sep