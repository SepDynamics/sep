#ifndef SEP_CONFIG_TYPES_H
#define SEP_CONFIG_TYPES_H

// Standard C headers
#include <cstddef>
#include <cstdint>

// Standard C++ headers
#include <map>
#include <string>
#include <vector>

namespace sep {

// Memory tier types that are used across multiple modules
enum class MemoryTierEnum {
  // Logical memory tiers
  STM,
  MTM,
  LTM,
  // Physical memory locations
  HOST = 100,   // Host memory (CPU)
  DEVICE = 101, // Device memory (GPU)
  UNIFIED = 102 // Unified memory (accessible by both CPU and GPU)
};

namespace config {

// Default configuration values
constexpr const char *DEFAULT_LOG_LEVEL = "info";
constexpr const char *DEFAULT_LOG_FILE = "sep.log";
constexpr const char *DEFAULT_LOG_DIR = "logs";

struct MemoryThresholdConfig {
  float promote_stm_to_mtm{0.7f};
  float promote_mtm_to_ltm{0.9f};
  float demote_threshold{0.3f};
  float fragmentation_threshold{0.3f};
  std::size_t stm_size{1 << 20};
  std::size_t mtm_size{4 << 20};
  std::size_t ltm_size{16 << 20};
  uint32_t stm_to_mtm_min_gen{5};
  uint32_t mtm_to_ltm_min_gen{100};
  bool use_unified_memory{true};
  bool enable_compression{true};
};

// Minimal CUDA configuration used by unit tests. These fields are
// sufficient for the parts of the engine compiled in this repository.
struct CudaConfig {
  bool use_gpu{true};
  std::size_t max_memory_mb{8192};
  std::size_t batch_size{1024};
  float gpu_memory_limit{0.9f};
  bool enable_profiling{false};
};

// API server configuration. Only a subset of fields is required for
// compiling the memory manager tests.
struct APIConfig {
    std::size_t max_connections{1000};
    std::size_t timeout_ms{5000};
    std::string host{"127.0.0.1"};
    uint16_t port{8080};
    std::size_t threads{4};
    std::size_t keep_alive_timeout_ms{15000};
    std::string log_level{"info"};
    bool enable_metrics{true};
    std::size_t max_batch_size{1024};
};

// Logging configuration placeholder
struct LogConfig {
  std::string level{"info"};
  std::string file{};
  std::string dir{"logs"};
};

// Quantum memory/coherence thresholds used by quantum processing components
struct QuantumThresholdConfig {
    float ltm_coherence_threshold{0.9f};
    float mtm_coherence_threshold{0.6f};
    float stability_threshold{0.8f};
};

// Basic analytics configuration used by quantum components
struct AnalyticsConfig {
  bool enable{false};
};

// Aggregate system configuration stub. Only the pieces used by tests
// are represented here.
struct SystemConfig {};

} // namespace config

enum class PatternStateEnum {
  UNINITIALIZED = 0,
  INITIALIZING,
  ACTIVE,
  STOPPED,
  ERROR
};

enum class StreamFlags { Default = 0, NonBlocking = 1 };

// Compression methods used across modules
enum class CompressionMethod { None, LZ4, ZSTD, Custom };

} // namespace sep

#endif // SEP_CONFIG_TYPES_H
