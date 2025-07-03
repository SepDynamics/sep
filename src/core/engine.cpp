#include "compat/macros.h"
#if defined(__CUDACC__)
#include <cuda_runtime.h> // real CUDA header when available
#endif
#include "api/types.h"
#include "compat/core.h"
#include "compat/cuda_api.hpp"
#include "compat/cuda_common.h"
#include "compat/memory.h"
#include "compat/shim.h"
#include "compat/stream.h"
#include "core/common.h" // defines sep::SEPResult
#include "core/engine.h"
#include "core/error_handler.h"
#include "core/logging.h" // This is actually the logging manager
#include "core/types.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/qbsa.h"
#include <vector>
#ifdef SEP_HAS_BLENDER
#include "blender/pattern_bridge.h"
#include "blender/types.h" // For SEPBlenderBridge definition
#endif
#include "audio/capture.h"
#include "audio/factory.h"

#include <cstdint>
#include <cstdio>
#include <exception>
#include <numeric>

// Define namespace alias for clarity
namespace logging = sep::logging;

namespace sep {
namespace core {
#if defined(__CUDACC__)
using namespace ::sep::cuda;
#endif

struct Engine::Impl {
  // CPU fallback buffers
  std::vector<std::uint32_t> d_bitfield_;
  std::vector<std::uint32_t> d_probe_indices_;
  std::vector<std::uint32_t> d_expectations_;
  std::vector<std::uint32_t> d_corrections_;
  std::vector<std::uint32_t> d_correction_count_;
  std::vector<std::uint64_t> d_chunks_;
  std::vector<std::uint32_t> d_collapse_indices_;
  std::vector<std::uint32_t> d_collapse_counts_;
  std::vector<StateNode> state_history_;
  ::sep::config::APIConfig config;
  bool initialized{false};
};

Engine::Engine() noexcept(false) : impl_(std::make_unique<Impl>()) {}

bool Engine::init(const sep::config::APIConfig &config) {
  impl_->config = config;
  impl_->d_bitfield_.resize(DEFAULT_SIZE);
  impl_->d_probe_indices_.resize(DEFAULT_SIZE);
  impl_->d_expectations_.resize(DEFAULT_SIZE);
  impl_->d_corrections_.resize(DEFAULT_SIZE);
  impl_->d_correction_count_.resize(1);
  impl_->d_chunks_.resize(DEFAULT_SIZE);
  impl_->d_collapse_indices_.resize(DEFAULT_SIZE * PAIRS_PER_CHUNK);
  impl_->d_collapse_counts_.resize(DEFAULT_SIZE);

  printf("DEBUG: Engine::init - Initializing audio capture\n");
  fflush(stdout);

  // Core initialization only - specialized components are initialized in main
  printf("DEBUG: Engine::init - Setting initialized flag\n");
  fflush(stdout);

  impl_->initialized = true;
  return true;
}

void Engine::run() {
  if (!impl_->initialized) {
    if (!init(impl_->config))
      return;
  }

  if (audio_capture_) {
    auto result = audio_capture_->start();
    if (result != audio::AudioError::NONE) {
      spdlog::error("Failed to start audio capture: {}",
                    static_cast<int>(result));
    }
  }
}

void Engine::shutdown() {
  if (audio_capture_) {
    auto result = audio_capture_->stop();
    if (result != audio::AudioError::NONE) {
      spdlog::error("Failed to stop audio capture: {}",
                    static_cast<int>(result));
    }
  }
}

namespace {
#if SEP_HAS_EXCEPTIONS
void log_cleanup_exception(const std::exception *ex) noexcept {
  try {
    if (ex) {
      (void)fprintf(stderr, "Warning: Exception during Engine cleanup: %s\n",
                    ex->what());
    } else {
      (void)fprintf(stderr, "%s\n",
                    "Warning: Unknown exception during Engine cleanup");
    }
  } catch (...) {
    std::terminate();
  }
}
#endif
} // namespace

Engine::~Engine() {
#if SEP_HAS_EXCEPTIONS
  try {
#endif
    (void)impl_; // nothing to clean up in CPU-only mode
#if SEP_HAS_EXCEPTIONS
  } catch (const std::exception &e) {
    log_cleanup_exception(&e);
  } catch (...) {
    log_cleanup_exception(nullptr);
  }
#endif
}

void Engine::generate_probes(const std::vector<::sep::PinState> &inputs,
                             std::vector<std::uint32_t> &probe_indices,
                             std::vector<std::uint32_t> &expectations,
                             std::uint64_t tick) {
  if (inputs.empty()) {
    ::sep::core::ErrorHandler::instance().reportError(
        {sep::SEPResult::INVALID_ARGUMENT, "No input states",
         "Engine::generate_probes"});
    return;
  }

  probe_indices.clear();
  expectations.clear();
  probe_indices.reserve(inputs.size());
  expectations.reserve(inputs.size());

  // Convert each input state to probe indices and expectations
  for (size_t i = 0; i < inputs.size(); ++i) {
    const auto &pin_state = inputs[i];

    // Generate probe index based on pin state and current tick
    std::uint32_t probe_idx =
        static_cast<std::uint32_t>((pin_state.pin_id + tick) % DEFAULT_SIZE);
    probe_indices.push_back(probe_idx);

    // Calculate expected value based on pin state and coherence
    std::uint32_t expected = static_cast<std::uint32_t>(
        pin_state.value * pin_state.coherence * 1000.f);
    expectations.push_back(expected);
  }

  // Ensure device buffers are properly sized
  if (impl_->d_bitfield_.size() < inputs.size()) {
    impl_->d_bitfield_.resize(inputs.size());
  }
  if (impl_->d_corrections_.size() < inputs.size()) {
    impl_->d_corrections_.resize(inputs.size());
  }
  if (impl_->d_correction_count_.size() < 1) {
    impl_->d_correction_count_.resize(1);
  }
  if (impl_->d_collapse_indices_.size() < inputs.size()) {
    impl_->d_collapse_indices_.resize(inputs.size());
  }
  if (impl_->d_collapse_counts_.size() < inputs.size()) {
    impl_->d_collapse_counts_.resize(inputs.size());
  }
  if (impl_->d_chunks_.size() < inputs.size()) {
    impl_->d_chunks_.resize(inputs.size());
  }

  // Initialize device buffers
  std::fill(impl_->d_bitfield_.begin(), impl_->d_bitfield_.end(), 0);
  std::fill(impl_->d_corrections_.begin(), impl_->d_corrections_.end(), 0);
  impl_->d_correction_count_[0] = 0;
  std::fill(impl_->d_collapse_indices_.begin(),
            impl_->d_collapse_indices_.end(), 0);
  std::fill(impl_->d_collapse_counts_.begin(), impl_->d_collapse_counts_.end(),
            0);
  std::fill(impl_->d_chunks_.begin(), impl_->d_chunks_.end(), 0);
}

void Engine::process_batch(const std::vector<::sep::PinState> &inputs,
                           std::uint64_t tick,
                           ::sep::quantum::QBSAResult &qbsa_result,
                           ::sep::cuda::QSHResult &qsh_result) {
  // Input validation
  if (inputs.empty()) {
    ::sep::core::ErrorHandler::instance().reportError(
        {sep::SEPResult::INVALID_ARGUMENT, "No input states",
         "Engine::process_batch"});
    return;
  }

  if (inputs.size() > DEFAULT_SIZE) {
    ::sep::core::ErrorHandler::instance().reportError(
        {sep::SEPResult::INVALID_ARGUMENT, "Batch too large",
         "Engine::process_batch"});
    return;
  }

  // Initialize result structures
  qbsa_result.corrections.clear();
  qbsa_result.correction_ratio = 0.0f;
  qbsa_result.collapse_detected = false;

  qsh_result.collapse_indices.assign(inputs.size(), {});
  qsh_result.collapse_counts.assign(inputs.size(), 0);
  qsh_result.total_collapses = 0;
  qsh_result.total_states = inputs.size();

  try {
    // Generate probes from inputs
    std::vector<std::uint32_t> probe_indices;
    std::vector<std::uint32_t> expectations;
    generate_probes(inputs, probe_indices, expectations, tick);

    // CPU fallback when CUDA is unavailable
    sep::quantum::QBSAProcessor cpu_proc;
    qbsa_result = cpu_proc.analyze(probe_indices, expectations);
    qbsa_result.collapse_detected =
        cpu_proc.detectCollapse(qbsa_result, inputs.size());

    // No CUDA results to copy when using CPU path

    // Update state history
    StateNode node;
    node.tick = tick;
    node.coherence = 1.0f - qbsa_result.correction_ratio;
    node.rupture = qbsa_result.collapse_detected;
    if (!impl_->state_history_.empty()) {
      node.parents.push_back(impl_->state_history_.size() - 1);
    }
    impl_->state_history_.push_back(node);

  } catch (const std::exception &e) {
    ::sep::core::ErrorHandler::instance().reportError(
        {sep::SEPResult::PROCESSING_ERROR, e.what(), "Engine::process_batch"});
    return;
  }
}

const std::vector<Engine::StateNode> &Engine::getStateHistory() const noexcept {
  return impl_->state_history_;
}

std::vector<float> Engine::getCoherenceHistory() const {
  std::vector<float> history;
  history.reserve(impl_->state_history_.size());
  for (const auto &n : impl_->state_history_) {
    history.push_back(n.coherence);
  }
  return history;
}

} // namespace core
} // namespace sep
