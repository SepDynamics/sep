#ifndef SEP_CORE_ENGINE_H
#define SEP_CORE_ENGINE_H

#include <cstdint>
#include <memory>
#include "compat/shim.h"

#include "core/common.h"

#include "core/types.h"
#include "blender/types.h"  // SEPBlenderBridge definition
#include "compat/types.h"  // for QSHResult
#include "quantum/qbsa.h"

namespace sep {
namespace cuda {
class Stream;
template <typename T>
class DeviceMemory;
using StreamPtr = std::shared_ptr<Stream>;
struct QSHResult;
}  // namespace cuda
namespace audio {
class AudioCapture;
}  // namespace audio
namespace pattern {
class BlenderBridge;
}  // namespace pattern
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

  // Delete copy operations
  Engine(const Engine &) = delete;
  Engine &operator=(const Engine &) = delete;

  // Allow move operations
  Engine(Engine &&) noexcept = default;
  Engine &operator=(Engine &&) noexcept = default;

  // Explicit initialization and lifecycle management
  bool init(const ::sep::config::APIConfig& config);
  void run();
  void shutdown();

  void generate_probes(const ::sep::shim::vector<::sep::PinState> &inputs,
                       ::sep::shim::vector<std::uint32_t> &indices,
                       ::sep::shim::vector<std::uint32_t> &expectations,
                       std::uint64_t tick);

  void process_batch(const ::sep::shim::vector<::sep::PinState> &inputs,
                     std::uint64_t tick,
                     ::sep::quantum::QBSAResult &qbsa_result,
                     ::sep::cuda::QSHResult &qsh_result);

  // DAG accessors
  struct StateNode {
    std::uint64_t tick{0};
    float coherence{0.0f};
    bool rupture{false};
    ::sep::shim::vector<std::size_t> parents;
  };

  const ::sep::shim::vector<StateNode> &getStateHistory() const noexcept;

  ::sep::shim::vector<float> getCoherenceHistory() const;

 private:
  static constexpr size_t DEFAULT_SIZE = 1024;
  static constexpr size_t PAIRS_PER_CHUNK = 32;  // WARP_SIZE

  struct Impl;
  std::unique_ptr<Impl> impl_;

  // Managed components
  std::unique_ptr<::sep::audio::AudioCapture> audio_capture_;
#ifdef SEP_HAS_BLENDER
  std::unique_ptr<SEPBlenderBridge> blender_bridge_;
#endif
};

}  // namespace core
}  // namespace sep

#endif  // SEP_CORE_ENGINE_H
