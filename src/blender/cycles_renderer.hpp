#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace sep {
namespace blender {

enum class ColorMode {
    Temperature = 0,
    Frequency,
    Coherence
};

enum class EmissionMode {
    Density = 0,
    Amplitude,
    Stability
};

class CyclesRenderer {
public:
    CyclesRenderer() = default;
    ~CyclesRenderer() = default;

    // Configuration methods
    void setRotation(float rotation) { rotation_ = rotation; }
    void setZoom(float zoom) { zoom_ = zoom; }
    void setWireframe(bool wireframe) { wireframe_ = wireframe; }
    void setColorMode(ColorMode mode) { color_mode_ = static_cast<int>(mode); }
    void setEmissionMode(EmissionMode mode) { emission_mode_ = static_cast<int>(mode); }
    void setRoughnessMode(int mode) { roughness_mode_ = mode; }
    
    // Method to cycle through available color modes
    void cycleColorMode() { color_mode_ = (color_mode_ + 1) % num_color_modes_; }
    
    // Rendering method for pattern visualization
    void renderPatternState(const std::vector<glm::vec3>& pattern_state);

private:
    float rotation_ = 0.0f;
    float zoom_ = 1.0f;
    bool wireframe_ = false;
    int color_mode_ = 0;
    int emission_mode_ = 0;
    int roughness_mode_ = 0;
    int num_color_modes_ = 3;
};

} // namespace blender
} // namespace sep