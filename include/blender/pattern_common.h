#pragma once

#include "compat/shim.h"
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

namespace sep {
namespace blender {

/**
 * Buffer for pattern data used in Blender visualization
 * Stores positions, colors, and scales for patterns
 */
class PatternBuffer {
public:
    PatternBuffer() : count(0) {}
    
    /**
     * Initialize buffer with specified pattern count
     * @param pattern_count Number of patterns to allocate space for
     */
    void initialize(uint32_t pattern_count) {
        count = pattern_count;
        // Allocate space for position, color, and scale (3 values each)
        data.resize(count * 9, 0.0f);
    }
    
    /**
     * Get position for pattern at specified index
     * @param index Pattern index
     * @return 3D position vector
     */
    glm::vec3 getPosition(uint32_t index) const {
        if (index >= count) return glm::vec3(0.0f);
        
        uint32_t offset = index * 9;
        return glm::vec3(
            data[offset + 0],
            data[offset + 1],
            data[offset + 2]
        );
    }
    
    /**
     * Set position for pattern at specified index
     * @param index Pattern index
     * @param position 3D position vector
     */
    void setPosition(uint32_t index, const glm::vec3& position) {
        if (index >= count) return;
        
        uint32_t offset = index * 9;
        data[offset + 0] = position.x;
        data[offset + 1] = position.y;
        data[offset + 2] = position.z;
    }
    
    /**
     * Get color for pattern at specified index
     * @param index Pattern index
     * @return RGB color vector
     */
    glm::vec3 getColor(uint32_t index) const {
        if (index >= count) return glm::vec3(0.0f);
        
        uint32_t offset = index * 9 + 3;
        return glm::vec3(
            data[offset + 0],
            data[offset + 1],
            data[offset + 2]
        );
    }
    
    /**
     * Set color for pattern at specified index
     * @param index Pattern index
     * @param color RGB color vector
     */
    void setColor(uint32_t index, const glm::vec3& color) {
        if (index >= count) return;
        
        uint32_t offset = index * 9 + 3;
        data[offset + 0] = color.x;
        data[offset + 1] = color.y;
        data[offset + 2] = color.z;
    }
    
    /**
     * Get scale for pattern at specified index
     * @param index Pattern index
     * @return 3D scale vector
     */
    glm::vec3 getScale(uint32_t index) const {
        if (index >= count) return glm::vec3(1.0f);
        
        uint32_t offset = index * 9 + 6;
        return glm::vec3(
            data[offset + 0],
            data[offset + 1],
            data[offset + 2]
        );
    }
    
    /**
     * Set scale for pattern at specified index
     * @param index Pattern index
     * @param scale 3D scale vector
     */
    void setScale(uint32_t index, const glm::vec3& scale) {
        if (index >= count) return;
        
        uint32_t offset = index * 9 + 6;
        data[offset + 0] = scale.x;
        data[offset + 1] = scale.y;
        data[offset + 2] = scale.z;
    }
    
    /**
     * Get raw data pointer for use with GPU buffers
     * @return Pointer to raw float data
     */
    const float* getRawData() const {
        return data.data();
    }
    
    /**
     * Get size of raw data in bytes
     * @return Size in bytes
     */
    size_t getSizeInBytes() const {
        return data.size() * sizeof(float);
    }
    
    uint32_t count;         // Number of patterns
    std::vector<float> data; // Raw pattern data (position, color, scale)
};

/**
 * Configuration parameters for pattern processing
 */
struct PatternProcessorConfig {
    uint32_t max_patterns{10000};      // Maximum number of patterns
    float coherence_threshold{0.7f};   // Coherence threshold for promotion
    float stability_threshold{0.85f};  // Stability threshold for promotion
    float evolution_rate{0.05f};       // Rate of pattern evolution
    float time_delta{0.016f};          // Time delta between updates (default 60 fps)
};

} // namespace blender
} // namespace sep
