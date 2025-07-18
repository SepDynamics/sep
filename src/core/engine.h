#ifndef SEP_CORE_ENGINE_H
#define SEP_CORE_ENGINE_H

#include <cstdint>
#include <memory>
#include <vector>

#include "compat/cuda_unified.h"
#include "compat/types.h"  // For QSHResult definition
#include "core/common.h"
#include "core/config.h"
#include "core/types.h"
#include "quantum/qbsa.h"

namespace sep {
namespace cuda {
class Stream;
template <typename T>
class DeviceMemory;
using StreamPtr = std::shared_ptr<Stream>;
struct QSHResult;
}  // namespace cuda
// Removed audio and blender forward declarations - not needed for quant processing
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

  bool init(const sep::config::CudaConfig &config);

  // Delete copy operations
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  // Allow move operations
  Engine(Engine &&) noexcept = default;
  Engine &operator=(Engine &&) noexcept = default;

  // Explicit initialization and lifecycle management
  void run();
  void shutdown();

  void generate_probes(const std::vector<::sep::PinState> &inputs,
                       std::vector<std::uint32_t> &indices,
                       std::vector<std::uint32_t> &expectations,
                       std::uint64_t tick);

  void process_batch(const std::vector<::sep::PinState> &inputs,
                     std::uint64_t tick,
                      sep::quantum::QBSAResult &qbsa_result,
                      sep::cuda::QSHResult &qsh_result);

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

 private:
  static constexpr size_t DEFAULT_SIZE = 1024;
  static constexpr size_t PAIRS_PER_CHUNK = 32;  // WARP_SIZE

  struct Impl;
  std::unique_ptr<Impl> impl_;

  // Managed components
  // (Removed audio and blender - not needed for quant processing)
};

}  // namespace core
}  // namespace sep

#endif  // SEP_CORE_ENGINE_H
