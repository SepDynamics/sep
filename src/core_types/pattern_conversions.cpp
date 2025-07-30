#include "pattern.h"
#include <cstring>
#include <algorithm>

// Implementation of conversion functions between canonical and POD types

namespace sep::core {

void convertToPOD(const QuantumState& src, QuantumStatePOD& dst) {
    dst.coherence = src.coherence;
    dst.stability = src.stability;
    dst.entropy = src.entropy;
    dst.mutation_rate = src.mutation_rate;
    dst.evolution_rate = src.evolution_rate;
    dst.energy = src.energy;
    dst.coupling_strength = src.coupling_strength;
    dst.phase = src.phase;
    dst.generation = src.generation;
    dst.mutation_count = src.mutation_count;
    dst.access_frequency = src.access_frequency;
    dst.status = static_cast<int>(src.status);
}

void convertFromPOD(const QuantumStatePOD& src, QuantumState& dst) {
    dst.coherence = src.coherence;
    dst.stability = src.stability;
    dst.entropy = src.entropy;
    dst.mutation_rate = src.mutation_rate;
    dst.evolution_rate = src.evolution_rate;
    dst.energy = src.energy;
    dst.coupling_strength = src.coupling_strength;
    dst.phase = src.phase;
    dst.generation = src.generation;
    dst.mutation_count = src.mutation_count;
    dst.access_frequency = src.access_frequency;
    dst.status = static_cast<QuantumState::Status>(src.status);
}

void convertToPOD(const PatternRelationship& src, PatternRelationshipPOD& dst) {
    // Copy string with bounds checking
    strncpy(dst.target_id, src.target_id.c_str(), sizeof(dst.target_id) - 1);
    dst.target_id[sizeof(dst.target_id) - 1] = '\0'; // Ensure null termination
    
    dst.strength = src.strength;
    dst.type = static_cast<int>(src.type);
}

void convertFromPOD(const PatternRelationshipPOD& src, PatternRelationship& dst) {
    dst.target_id = std::string(src.target_id);
    dst.strength = src.strength;
    dst.type = static_cast<RelationshipType>(src.type);
}

void convertToPOD(const Pattern& src, PatternPOD& dst) {
    // Copy ID with bounds checking
    strncpy(dst.id, src.id.c_str(), sizeof(dst.id) - 1);
    dst.id[sizeof(dst.id) - 1] = '\0';
    
    // Convert vectors
    convertToPOD(src.position, dst.position);
    convertToPOD(src.momentum, dst.momentum);
    convertToPOD(src.velocity, dst.velocity);
    convertToPOD(src.attributes, dst.attributes);
    
    // Convert quantum state
    convertToPOD(src.quantum_state, dst.quantum_state);
    
    // Convert relationships (limited to array size)
    dst.relationship_count = std::min(static_cast<int>(src.relationships.size()), 16);
    for (int i = 0; i < dst.relationship_count; ++i) {
        convertToPOD(src.relationships[i], dst.relationships[i]);
    }
    
    // Convert parent IDs (limited to array size)
    dst.parent_count = std::min(static_cast<int>(src.parent_ids.size()), 8);
    for (int i = 0; i < dst.parent_count; ++i) {
        strncpy(dst.parent_ids[i], src.parent_ids[i].c_str(), sizeof(dst.parent_ids[i]) - 1);
        dst.parent_ids[i][sizeof(dst.parent_ids[i]) - 1] = '\0';
    }
    
    // Convert complex amplitude to real/imaginary parts
    dst.amplitude_real = src.amplitude.real();
    dst.amplitude_imag = src.amplitude.imag();
    
    // Copy scalar fields
    dst.timestamp = src.timestamp;
    dst.last_accessed = src.last_accessed;
    dst.last_modified = src.last_modified;
    dst.last_updated = src.last_updated;
    dst.generation = src.generation;
    dst.coherence = src.coherence;
}

void convertFromPOD(const PatternPOD& src, Pattern& dst) {
    // Copy ID
    dst.id = std::string(src.id);
    
    // Convert vectors
    convertFromPOD(src.position, dst.position);
    convertFromPOD(src.momentum, dst.momentum);
    convertFromPOD(src.velocity, dst.velocity);
    convertFromPOD(src.attributes, dst.attributes);
    
    // Convert quantum state
    convertFromPOD(src.quantum_state, dst.quantum_state);
    
    // Convert relationships
    dst.relationships.clear();
    dst.relationships.reserve(src.relationship_count);
    for (int i = 0; i < src.relationship_count; ++i) {
        PatternRelationship rel;
        convertFromPOD(src.relationships[i], rel);
        dst.relationships.push_back(rel);
    }
    
    // Convert parent IDs
    dst.parent_ids.clear();
    dst.parent_ids.reserve(src.parent_count);
    for (int i = 0; i < src.parent_count; ++i) {
        dst.parent_ids.emplace_back(src.parent_ids[i]);
    }
    
    // Convert complex amplitude from real/imaginary parts
    dst.amplitude = std::complex<float>(src.amplitude_real, src.amplitude_imag);
    
    // Copy scalar fields
    dst.timestamp = src.timestamp;
    dst.last_accessed = src.last_accessed;
    dst.last_modified = src.last_modified;
    dst.last_updated = src.last_updated;
    dst.generation = src.generation;
    dst.coherence = src.coherence;
}

} // namespace sep::core
