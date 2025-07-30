#pragma once
#include <cuda_runtime.h>

#ifdef __CUDACC__
#include <cuda_runtime.h>
#endif

#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include "glm_cuda_compat.h"
#include <glm/glm.hpp>

namespace sep {
namespace compat {

// CUDA-compatible data structures
struct PatternRelationship {
    static constexpr int MAX_ID_LENGTH = 64;
    char target_id[MAX_ID_LENGTH];
    float strength = 0.0f;
    
    // Additional compatibility fields
#ifndef __CUDA_ARCH__
    std::string targetId;  // Legacy field name
    std::string type = "default";  // Relationship type
    
    // Helper constructors for host code
    PatternRelationship() { 
        target_id[0] = '\0'; 
        targetId = "";
    }
    PatternRelationship(const std::string& id, float str = 0.0f) : strength(str), targetId(id) {
        std::strncpy(target_id, id.c_str(), MAX_ID_LENGTH - 1);
        target_id[MAX_ID_LENGTH - 1] = '\0';
    }
#endif
};

struct PatternConfig {
    float min_coherence = 0.0f;
    float min_stability = 0.0f;
    int max_patterns = 100;
};

struct PatternResult {
    int processed_count = 0;
    bool success = true;
};

// CUDA-compatible pattern data with fixed-size arrays
struct PatternData {
    static constexpr int MAX_ATTRIBUTES = 16;
    static constexpr int MAX_ID_LENGTH = 64;
    static constexpr int MAX_RELATIONSHIPS = 8;
    
    char id[MAX_ID_LENGTH];
    int generation = 0;
    float attributes[MAX_ATTRIBUTES];
    int size = 0;
    
    // Position for pattern evolution
    glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Velocity for movement simulations
    glm::vec4 velocity = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Coherence value for pattern stability
    float coherence = 0.0f;
    
    // Fixed-size array for relationships
    PatternRelationship relationships[MAX_RELATIONSHIPS];
    int relationship_count = 0;

#ifndef __CUDA_ARCH__
    // Host-only compatibility members
    std::vector<float> data;
    
    // Host-side relationship structure for easier manipulation
    struct HostRelationship {
        std::string target_id;
        float strength = 0.0f;
    };
    std::vector<HostRelationship> host_relationships;
    
    // Constructor for host code
    PatternData() { 
        id[0] = '\0'; 
        std::fill(attributes, attributes + MAX_ATTRIBUTES, 0.0f);
    }
    
    PatternData(const std::string& pattern_id) : PatternData() {
        std::strncpy(id, pattern_id.c_str(), MAX_ID_LENGTH - 1);
        id[MAX_ID_LENGTH - 1] = '\0';
    }
    
    // Sync methods to keep compatibility data updated
    void sync_to_host() {
        data.clear();
        data.insert(data.end(), attributes, attributes + size);
        
        host_relationships.clear();
        for (int i = 0; i < relationship_count; ++i) {
            HostRelationship hr;
            hr.target_id = relationships[i].target_id;
            hr.strength = relationships[i].strength;
            host_relationships.push_back(hr);
        }
    }
    
    void sync_from_host() {
        size = std::min(static_cast<int>(data.size()), MAX_ATTRIBUTES);
        std::copy(data.begin(), data.begin() + size, attributes);
        
        relationship_count = std::min(static_cast<int>(host_relationships.size()), MAX_RELATIONSHIPS);
        for (int i = 0; i < relationship_count; ++i) {
            const auto& host_rel = host_relationships[i];
            std::strncpy(relationships[i].target_id, host_rel.target_id.c_str(), MAX_ID_LENGTH - 1);
            relationships[i].target_id[MAX_ID_LENGTH - 1] = '\0';
            relationships[i].strength = host_rel.strength;
        }
    }
#endif

    // Host-only methods for compatibility with existing code
#ifndef __CUDA_ARCH__
    void push_back(float value) {
        if (size < MAX_ATTRIBUTES) {
            attributes[size++] = value;
        }
    }

    // Basic iterator support to satisfy range-based for loops if needed elsewhere.
    float* begin() { return attributes; }
    float* end() { return attributes + size; }
    const float* begin() const { return attributes; }
    const float* end() const { return attributes + size; }
#endif

    // Device-accessible methods
    __host__ __device__ float& operator[](int idx) { return attributes[idx]; }
    __host__ __device__ const float& operator[](int idx) const { return attributes[idx]; }
    __host__ __device__ int get_size() const { return size; }

    // Additional compatibility methods
#ifndef __CUDA_ARCH__
    bool empty() const { return size == 0; }
    void resize(int new_size) { 
        size = std::min(new_size, MAX_ATTRIBUTES); 
        if (data.size() != static_cast<size_t>(size)) {
            data.resize(size);
        }
    }
    
    // String-compatible ID access
    std::string get_id() const { return std::string(id); }
    void set_id(const std::string& new_id) {
        std::strncpy(id, new_id.c_str(), MAX_ID_LENGTH - 1);
        id[MAX_ID_LENGTH - 1] = '\0';
    }
    
    // Quantum state placeholder for compatibility
    struct {
        float coherence = 0.0f;
        float phase = 0.0f;
        float amplitude = 1.0f;
        float entanglement = 0.0f;
        float stability = 0.0f;
        float entropy = 0.0f;
        float mutation_rate = 0.0f;
    } quantum_state;
#endif
};

} // namespace compat
} // namespace sep