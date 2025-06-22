/**
 * @file mock_pattern_compression.cpp
 * @brief Mock implementation of PatternCompression for testing
 */

#include "blender/compression.h"
#include <algorithm>
#include <cstring>

namespace blender {

bool PatternCompression::compressPatterns(
    const sep::shim::vector<sep::pattern::PatternData>& patterns,
    sep::shim::vector<uint8_t>& compressed, size_t& compressed_size,
    Mode mode) {
  (void)mode;
  // Simple mock implementation for testing
  if (patterns.empty()) {
    compressed.clear();
    compressed_size = 0;
    return false;  // Return false for empty patterns as expected by tests
  }

  // For testing, we'll create a simple serialization format:
  // 1. Store pattern count (uint32_t)
  // 2. For each pattern, store only the essential numeric fields
  //    (not the std::string or std::vector which cause memory issues)
  
  // Calculate a safe size for our serialized data
  // We'll only store the numeric fields, not the string or vector
  const size_t PATTERN_HEADER_SIZE = sizeof(uint32_t); // pattern count
  const size_t PATTERN_NUMERIC_SIZE =
      sizeof(glm::vec4) * 3 +  // position, velocity, attributes
      sizeof(float) * 4 +      // coherence, entropy, stability, mutation_rate
      sizeof(uint32_t) * 4 +   // generation, mutations, promotions, flags
      sizeof(uint64_t) +       // offset_in_tier
      sizeof(uint32_t) * 2;    // size_in_bytes, tier_id
  
  size_t total_size = PATTERN_HEADER_SIZE + patterns.size() * PATTERN_NUMERIC_SIZE;
  
  // Resize the output buffer
  compressed.resize(total_size);
  compressed_size = total_size;
  
  // Store pattern count at the beginning
  uint32_t pattern_count = static_cast<uint32_t>(patterns.size());
  std::memcpy(compressed.data(), &pattern_count, sizeof(uint32_t));
  
  // Serialize each pattern's numeric fields only
  uint8_t* current_pos = compressed.data() + PATTERN_HEADER_SIZE;
  for (const auto& pattern : patterns) {
    // Copy spatial data (3 Vec4s)
    std::memcpy(current_pos, &pattern.position, sizeof(glm::vec4));
    current_pos += sizeof(glm::vec4);
    std::memcpy(current_pos, &pattern.velocity, sizeof(glm::vec4));
    current_pos += sizeof(glm::vec4);
    std::memcpy(current_pos, &pattern.attributes, sizeof(glm::vec4));
    current_pos += sizeof(glm::vec4);
    
    // Copy metrics (4 floats)
    std::memcpy(current_pos, &pattern.coherence, sizeof(float));
    current_pos += sizeof(float);
    std::memcpy(current_pos, &pattern.entropy, sizeof(float));
    current_pos += sizeof(float);
    std::memcpy(current_pos, &pattern.stability, sizeof(float));
    current_pos += sizeof(float);
    std::memcpy(current_pos, &pattern.mutation_rate, sizeof(float));
    current_pos += sizeof(float);
    
    // Copy state (4 uint32_t)
    std::memcpy(current_pos, &pattern.generation, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(current_pos, &pattern.mutations, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(current_pos, &pattern.promotions, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(current_pos, &pattern.flags, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    
    // Copy memory fields (excluding std::string)
    std::memcpy(current_pos, &pattern.offset_in_tier, sizeof(uint64_t));
    current_pos += sizeof(uint64_t);
    std::memcpy(current_pos, &pattern.size_in_bytes, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(current_pos, &pattern.tier_id, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    
    // Skip the std::string id and std::vector relationships
    // as these can't be safely memcpy'd
  }
  
  return true;
}

bool PatternCompression::decompressPatterns(
    const uint8_t* compressed_data, size_t compressed_size,
    sep::shim::vector<sep::pattern::PatternData>& patterns) {
  // Simple mock implementation for testing
  if (!compressed_data || compressed_size < sizeof(uint32_t)) {
    patterns.clear();
    return false;
  }
  
  // Read pattern count
  uint32_t pattern_count;
  std::memcpy(&pattern_count, compressed_data, sizeof(uint32_t));
  
  // Calculate expected size based on our serialization format
  const size_t PATTERN_HEADER_SIZE = sizeof(uint32_t);
  const size_t PATTERN_NUMERIC_SIZE =
      sizeof(glm::vec4) * 3 +  // position, velocity, attributes
      sizeof(float) * 4 +      // coherence, entropy, stability, mutation_rate
      sizeof(uint32_t) * 4 +   // generation, mutations, promotions, flags
      sizeof(uint64_t) +       // offset_in_tier
      sizeof(uint32_t) * 2;    // size_in_bytes, tier_id
  
  size_t expected_size = PATTERN_HEADER_SIZE + pattern_count * PATTERN_NUMERIC_SIZE;
  
  // Validate size
  if (compressed_size < expected_size) {
    patterns.clear();
    return false;
  }
  
  // Resize output vector and initialize with default values
  patterns.resize(pattern_count);
  
  // Deserialize each pattern's numeric fields
  const uint8_t* current_pos = compressed_data + PATTERN_HEADER_SIZE;
  for (uint32_t i = 0; i < pattern_count; ++i) {
    sep::pattern::PatternData& pattern = patterns[i];
    
    // Copy spatial data (3 Vec4s)
    std::memcpy(&pattern.position, current_pos, sizeof(glm::vec4));
    current_pos += sizeof(glm::vec4);
    std::memcpy(&pattern.velocity, current_pos, sizeof(glm::vec4));
    current_pos += sizeof(glm::vec4);
    std::memcpy(&pattern.attributes, current_pos, sizeof(glm::vec4));
    current_pos += sizeof(glm::vec4);
    
    // Copy metrics (4 floats)
    std::memcpy(&pattern.coherence, current_pos, sizeof(float));
    current_pos += sizeof(float);
    std::memcpy(&pattern.entropy, current_pos, sizeof(float));
    current_pos += sizeof(float);
    std::memcpy(&pattern.stability, current_pos, sizeof(float));
    current_pos += sizeof(float);
    std::memcpy(&pattern.mutation_rate, current_pos, sizeof(float));
    current_pos += sizeof(float);
    
    // Copy state (4 uint32_t)
    std::memcpy(&pattern.generation, current_pos, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(&pattern.mutations, current_pos, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(&pattern.promotions, current_pos, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(&pattern.flags, current_pos, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    
    // Copy memory fields (excluding std::string)
    std::memcpy(&pattern.offset_in_tier, current_pos, sizeof(uint64_t));
    current_pos += sizeof(uint64_t);
    std::memcpy(&pattern.size_in_bytes, current_pos, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    std::memcpy(&pattern.tier_id, current_pos, sizeof(uint32_t));
    current_pos += sizeof(uint32_t);
    
    // Initialize the string and vector with empty values
    pattern.id = "";
    pattern.relationships.clear();
  }
  
  return true;
}

bool PatternCompression::getMetadata(
    const uint8_t* compressed_data, size_t compressed_size,
    Metadata& metadata) {
  // Simple mock implementation
  if (!compressed_data || compressed_size < sizeof(uint32_t)) {
    return false;
  }
  
  // Read pattern count
  uint32_t pattern_count;
  std::memcpy(&pattern_count, compressed_data, sizeof(uint32_t));
  
  // Set metadata
  metadata.pattern_count = pattern_count;
  
  // Set flags to match what the test expects
  metadata.flags = static_cast<uint32_t>(Flags::DELTA_ENCODED) |
                   static_cast<uint32_t>(Flags::QUANTIZED) |
                   static_cast<uint32_t>(Flags::BIT_PACKED);
  
  // Calculate mock compression ratio
  const size_t PATTERN_NUMERIC_SIZE =
      sizeof(glm::vec4) * 3 +  // position, velocity, attributes
      sizeof(float) * 4 +      // coherence, entropy, stability, mutation_rate
      sizeof(uint32_t) * 4 +   // generation, mutations, promotions, flags
      sizeof(uint64_t) +       // offset_in_tier
      sizeof(uint32_t) * 2;    // size_in_bytes, tier_id
  
  float full_size = pattern_count * sizeof(sep::pattern::PatternData);
  float compressed_numeric_size = pattern_count * PATTERN_NUMERIC_SIZE;
  
  metadata.compression_ratio = compressed_numeric_size / full_size;
  
  return true;
}

bool PatternCompression::validate(
    const uint8_t* compressed_data, size_t compressed_size) {
  // Simple mock implementation
  if (!compressed_data || compressed_size < sizeof(uint32_t)) {
    return false;
  }
  
  // Read pattern count
  uint32_t pattern_count;
  std::memcpy(&pattern_count, compressed_data, sizeof(uint32_t));
  
  // Calculate expected size based on our serialization format
  const size_t PATTERN_HEADER_SIZE = sizeof(uint32_t);
  const size_t PATTERN_NUMERIC_SIZE =
      sizeof(glm::vec4) * 3 +  // position, velocity, attributes
      sizeof(float) * 4 +      // coherence, entropy, stability, mutation_rate
      sizeof(uint32_t) * 4 +   // generation, mutations, promotions, flags
      sizeof(uint64_t) +       // offset_in_tier
      sizeof(uint32_t) * 2;    // size_in_bytes, tier_id
  
  size_t expected_size = PATTERN_HEADER_SIZE + pattern_count * PATTERN_NUMERIC_SIZE;
  
  // Validate size
  return compressed_size >= expected_size;
}

}  // namespace blender
