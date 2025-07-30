#pragma once

#include <complex>
#include <string>
#include <vector>
#include <cstdint>

// The One True Normal Reference Frame for Pattern Types
// This is the canonical source of truth for all pattern-related data structures

namespace sep::core {

// Forward declarations
struct QuantumState;
struct PatternRelationship;

// =============================================================================
// CANONICAL C++ TYPES (The Normal Reference Frame)
// =============================================================================

/// The quantum state that governs pattern behavior
struct QuantumState {
    float coherence{0.0f};           // Coherence measure [0.0, 1.0]
    float stability{0.0f};           // Stability measure [0.0, 1.0]  
    float entropy{0.0f};             // Information entropy
    float mutation_rate{0.0f};       // Rate of change
    float evolution_rate{0.0f};      // Speed of evolution
    float energy{0.0f};              // Energy level
    float coupling_strength{0.0f};   // Interaction strength
    float phase{0.0f};               // Phase angle
    int generation{0};               // Generation number
    int mutation_count{0};           // Number of mutations
    int access_frequency{0};         // Access count
    
    enum class Status {
        STABLE,
        UNSTABLE, 
        COLLAPSED
    } status{Status::STABLE};
};

/// Types of relationships between patterns
enum class RelationshipType {
    Generic,
    Entanglement,
    Causality
};

/// A relationship between two patterns
struct PatternRelationship {
    std::string target_id;
    float strength{0.0f};
    RelationshipType type{RelationshipType::Generic};
};

/// 3D vector for positions, velocities, etc.
struct Vec3 {
    float x{0.0f};
    float y{0.0f}; 
    float z{0.0f};
};

/// 4D vector for extended attributes
struct Vec4 {
    float x{0.0f};
    float y{0.0f};
    float z{0.0f};
    float w{0.0f};
};

/// The fundamental pattern structure - the core entity of the system
struct Pattern {
    std::string id;                                    // Unique identifier
    Vec4 position{};                                   // 4D position
    Vec3 momentum{};                                   // 3D momentum
    Vec4 velocity{};                                   // 4D velocity  
    Vec4 attributes{};                                 // Custom attributes
    QuantumState quantum_state{};                      // Quantum properties
    std::vector<PatternRelationship> relationships{};  // Connections to other patterns
    std::vector<std::string> parent_ids{};            // Lineage tracking
    std::complex<float> amplitude{1.0f, 0.0f};        // Quantum amplitude
    uint64_t timestamp{0};                             // Creation time
    uint64_t last_accessed{0};                         // Last access time
    uint64_t last_modified{0};                         // Last modification time
    uint64_t last_updated{0};                          // Last update time
    int generation{0};                                 // Generation in evolution
    float coherence{0.0f};                            // Pattern coherence
};

// =============================================================================
// CUDA POD PROJECTIONS (For GPU Kernels)
// =============================================================================

/// POD version of Vec3 for CUDA kernels
struct Vec3POD {
    float x, y, z;
};

/// POD version of Vec4 for CUDA kernels  
struct Vec4POD {
    float x, y, z, w;
};

/// POD version of QuantumState for CUDA kernels
struct QuantumStatePOD {
    float coherence;
    float stability;
    float entropy;
    float mutation_rate;
    float evolution_rate;
    float energy;
    float coupling_strength;
    float phase;
    int generation;
    int mutation_count;
    int access_frequency;
    int status; // Cast to/from QuantumState::Status
};

/// POD version of PatternRelationship for CUDA kernels
struct PatternRelationshipPOD {
    char target_id[64];  // Fixed-size string for GPU
    float strength;
    int type; // Cast to/from RelationshipType
};

/// POD version of Pattern for CUDA kernels
struct PatternPOD {
    char id[64];                                      // Fixed-size string
    Vec4POD position;
    Vec3POD momentum;
    Vec4POD velocity;
    Vec4POD attributes;
    QuantumStatePOD quantum_state;
    PatternRelationshipPOD relationships[16];         // Fixed-size array
    char parent_ids[8][64];                          // Fixed-size parent tracking
    float amplitude_real;                            // Real part of amplitude
    float amplitude_imag;                            // Imaginary part of amplitude
    uint64_t timestamp;
    uint64_t last_accessed;
    uint64_t last_modified;
    uint64_t last_updated;
    int generation;
    float coherence;
    int relationship_count;                          // Number of valid relationships
    int parent_count;                                // Number of valid parents
};

// =============================================================================
// MEASUREMENT FUNCTIONS (Conversions Between Reference Frames)
// =============================================================================

/// Convert canonical Vec3 to POD for CUDA
inline void convertToPOD(const Vec3& src, Vec3POD& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

/// Convert POD Vec3 back to canonical
inline void convertFromPOD(const Vec3POD& src, Vec3& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
}

/// Convert canonical Vec4 to POD for CUDA
inline void convertToPOD(const Vec4& src, Vec4POD& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

/// Convert POD Vec4 back to canonical
inline void convertFromPOD(const Vec4POD& src, Vec4& dst) {
    dst.x = src.x;
    dst.y = src.y;
    dst.z = src.z;
    dst.w = src.w;
}

/// Convert canonical QuantumState to POD for CUDA
void convertToPOD(const QuantumState& src, QuantumStatePOD& dst);

/// Convert POD QuantumState back to canonical
void convertFromPOD(const QuantumStatePOD& src, QuantumState& dst);

/// Convert canonical PatternRelationship to POD for CUDA
void convertToPOD(const PatternRelationship& src, PatternRelationshipPOD& dst);

/// Convert POD PatternRelationship back to canonical
void convertFromPOD(const PatternRelationshipPOD& src, PatternRelationship& dst);

/// Convert canonical Pattern to POD for CUDA
void convertToPOD(const Pattern& src, PatternPOD& dst);

/// Convert POD Pattern back to canonical
void convertFromPOD(const PatternPOD& src, Pattern& dst);

} // namespace sep::core
