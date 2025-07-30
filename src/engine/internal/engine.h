#ifndef SEP_CORE_ENGINE_H
#define SEP_CORE_ENGINE_H

#include "common.h"
#include "config.h"
#include "engine/internal/standard_includes.h"
#include "metrics_collector.h"
#include "quantum/pattern_metric_engine.h"
#include "quantum/bitspace/qbsa.h"
#include "quantum/quantum_processor.h"
#include "quantum/types.h"
#include "types.h"

namespace sep {
namespace cuda {
class Stream;
template <typename T>
class DeviceMemory;
using StreamPtr = std::shared_ptr<Stream>;

struct QSHResult {
    std::vector<std::vector<std::uint32_t>> collapse_indices;
    std::vector<std::uint32_t> collapse_counts;
    std::uint32_t total_collapses{0};
    std::uint32_t total_states{0};
};
}  // namespace cuda
}  // namespace sep

namespace sep {
namespace core {

/**
 * @brief Main quantum processing engine
 */
class Engine {
 public:
  Engine() noexcept(false);
  ~Engine();

  bool init(const config::CudaConfig &config);

  // Delete copy operations
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  // Explicitly implement move operations
  Engine(Engine &&) noexcept;
  Engine &operator=(Engine &&) noexcept;

  // Explicit initialization and lifecycle management
  void run();
  void shutdown();

  void generate_probes(const std::vector<::sep::PinState> &inputs,
                       std::vector<std::uint32_t> &indices,
                       std::vector<std::uint32_t> &expectations, std::uint64_t tick);

  void process_batch(const std::vector<::sep::PinState> &inputs, std::uint64_t tick,
                     quantum::QBSAResult &qbsa_result, cuda::QSHResult &qsh_result);

  // Process quantitative data (market data, etc.)
  std::string processQuantData(const std::string &dataPath, bool useGPU = true);

  // DAG accessors
  struct StateNode {
    std::uint64_t tick{0};
    float coherence{0.0f};
    bool rupture{false};
    std::vector<std::size_t> parents;
  };

  const std::vector<StateNode> &getStateHistory() const noexcept;

  std::vector<float> getCoherenceHistory() const;

  void ingestFile(const std::string &dataPath, bool legacy = false);
  void ingestFromDirectory(const std::string &dirPath, bool recursive = true);
  void ingestFromSocket(int socket_fd);
  void ingestFromStream(std::istream& stream);

  // Dashboard integration methods
  std::map<std::string, double> getMetrics() const;
  bool isProcessing() const;

  quantum::PatternMetricEngine* getPatternMetricEngine() { return &pattern_metric_engine_; }

 private:
  static constexpr size_t DEFAULT_SIZE = 1024;
  static constexpr size_t PAIRS_PER_CHUNK = 32;  // WARP_SIZE

  struct Impl;
  std::unique_ptr<Impl> impl_;
  quantum::PatternMetricEngine pattern_metric_engine_;
  std::unique_ptr<quantum::QuantumProcessor> quantum_processor_;
  MetricsCollector metrics_collector_;
};

}  // namespace core
}  // namespace sep

#endif  // SEP_CORE_ENGINE_H
