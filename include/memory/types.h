#pragma once

#ifndef MEMORY_TYPES_H
#define MEMORY_TYPES_H

#include "core/types.h"
#include "compat/shim.h"

#include <cstdint>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <glm/glm.hpp>
#include <chrono>
#include <glm/vec3.hpp>
#include <vector>

namespace sep {
namespace persistence {

// Structure to store relationship metadata for persistence
struct RelationshipData {
    std::size_t id;       // Related pattern ID
    uint8_t type;         // Relationship type (from RelationshipType enum)
    float strength;       // Relationship strength (0.0-1.0)
};

// Structure to store pattern data for persistence
struct PatternData {
    glm::vec3 position;                    // Pattern's 3D position data
    float coherence;                       // Pattern's coherence score
    float stability;                       // Pattern's stability score
    int generation_count;                  // Pattern's generation count
    float access_frequency;                // Pattern's access frequency
    ::sep::shim::chrono::system_clock::time_point timestamp;  // Pattern's timestamp
    ::sep::shim::vector<std::size_t> relationships;          // Legacy relationships (just IDs)
    ::sep::shim::vector<RelationshipData> relationship_data; // Enhanced relationship data
    uint64_t dag_node_id = 0;              // DAG node ID for pattern
};

class RedisManager {
public:
    RedisManager(const std::string& host = "localhost", int port = 6379);
    ~RedisManager();

    // Store a pattern in Redis with its full metadata
    void storePattern(std::size_t id, const sep::persistence::PatternData& data, const std::string& tier);
    
    // Load a pattern from Redis
    std::optional<sep::persistence::PatternData> loadPattern(std::size_t id, const std::string& tier);
    
    // Get all pattern IDs for a given tier
    std::vector<std::size_t> getPatternIds(const std::string& tier);
    
    // Remove a pattern from Redis
    void removePattern(std::size_t id, const std::string& tier);
    
    // Batch operations for optimization
    void bulkStore(const std::vector<std::pair<std::size_t, sep::persistence::PatternData>>& patterns, const std::string& tier);
    std::vector<sep::persistence::PatternData> bulkLoad(const std::vector<std::size_t>& ids, const std::string& tier);
    
    // Status check
    bool isConnected() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace persistence
namespace logging {

// Log levels enum
enum class Level { TRACE = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4, CRITICAL = 5, OFF = 6 };

// Console sink configuration
struct ConsoleConfig {
  bool enabled = true;
};

// File sink configuration
struct FileConfig {
  std::string path;
  size_t max_size = 1048576;  // 1MB default
  size_t max_files = 3;       // 3 files default
};

// Logger configuration structure
struct LoggerConfig {
  std::string name;
  Level level = Level::INFO;
  std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v";
  std::vector<std::string> sinks;
  bool async = false;
  size_t queue_size = 8192;
  ConsoleConfig console;
  FileConfig file;

  LoggerConfig() = default;
  explicit LoggerConfig(const std::string& logger_name) : name(logger_name) {}
};

}  // namespace logging
}   // namespace sep

#endif  // MEMORY_TYPES_H
