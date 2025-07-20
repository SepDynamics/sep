#include <string.h>
#include <cstring>
#include <ctime>
#include <time.h>
#include <unistd.h>
#include <cstdlib>

#include "macros.h"
#if defined(__CUDACC__)
#  include <cuda_runtime.h> // real CUDA header when available
#endif

// Determine if real CUDA support is present at compile time. This mirrors the
// logic used in other components so the engine can compile cleanly even when
// CUDA sources are not built with NVCC.
#if defined(__CUDACC__)
#  define SEP_ENGINE_HAS_CUDA 1
#else
#  define SEP_ENGINE_HAS_CUDA 0
#endif
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstring>  // For std::memcpy, std::memcmp if used in headers
#include <exception>
#include <nlohmann/json.hpp>
#include <numeric>
#include <sstream>
#include <vector>
#include <filesystem>
#include <iostream>

#include "api/types.h"
#include "core.h"
#include "cuda_api.hpp"
#include "cuda.h"
#include "memory.h"
#include "shim.h"
#include "stream.h"
#include "common.h"  // defines sep::SEPResult
#include "config.h"
#include "dag_graph.h"
#include "data_parser.h"
#include ".h"
#include "error_handler.h"
#include "logging.h"  // This is actually the logging manager
#include "types.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/qbsa.h"

// Define namespace alias for clarity
namespace logging = sep::logging;

namespace sep {
namespace core {
#if SEP_ENGINE_HAS_CUDA
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
  ::sep::config::CudaConfig config;
  bool initialized{false};
};

Engine::Engine() noexcept(false) : impl_(std::make_unique<Impl>()) {
    sep::quantum::QuantumProcessor::Config config;
    quantum_processor_ = sep::quantum::createQuantumProcessor(config);
}

bool Engine::init(const sep::config::CudaConfig &config)
{
    impl_->config = config;
    impl_->d_bitfield_.resize(DEFAULT_SIZE);
    impl_->d_probe_indices_.resize(DEFAULT_SIZE);
    impl_->d_expectations_.resize(DEFAULT_SIZE);
    impl_->d_corrections_.resize(DEFAULT_SIZE);
    impl_->d_correction_count_.resize(1);
    impl_->d_chunks_.resize(DEFAULT_SIZE);
    impl_->d_collapse_indices_.resize(DEFAULT_SIZE * PAIRS_PER_CHUNK);
    impl_->d_collapse_counts_.resize(DEFAULT_SIZE);

#ifdef SEP_HAS_AUDIO
  printf("DEBUG: Engine::init - Initializing audio capture\n");
  (void)fflush(stdout);
#endif

  // Core initialization only - specialized components are initialized in main
  printf("DEBUG: Engine::init - Setting initialized flag\n");
  (void)fflush(stdout);

  impl_->initialized = true;
  return true;
}

void Engine::run() {
  if (!impl_->initialized) {
    if (!init(impl_->config))
      return;
  }

  // (Removed audio capture start - not needed for quant processing)
}

void Engine::shutdown() {
  // (Removed audio capture stop - not needed for quant processing)
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

void Engine::ingestFile(const std::string& dataPath, bool legacy) {
    metrics_collector_.increment("files_ingested");
    if (legacy) {
        DataParser parser;
        auto patterns = parser.parseFile(dataPath);
        auto pinStates = parser.toPinStates(patterns);
        metrics_collector_.increment("patterns_converted_to_pin_states", patterns.size());
        // Assuming process_batch is the intended consumer for PinStates
        // process_batch(pinStates, ...);
    } else {
        pattern_metric_engine_.ingestFile(dataPath);
        pattern_metric_engine_.evolvePatterns();
        auto metrics = pattern_metric_engine_.computeMetrics();
        metrics_collector_.increment("patterns_processed", metrics.size());
        for (size_t i = 0; i < metrics.size(); ++i) {
            // This is a placeholder for a more sophisticated mapping
            // between PatternMetrics and the QuantumProcessor's representation.
            quantum_processor_->processPattern(glm::vec3(metrics[i].coherence, metrics[i].stability, metrics[i].entropy), i);
        }
    }
}

void Engine::ingestFromDirectory(const std::string& dirPath, bool recursive) {
    std::vector<std::string> filePaths;
    if (recursive) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                filePaths.push_back(entry.path().string());
            }
        }
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                filePaths.push_back(entry.path().string());
            }
        }
    }

    const size_t batch_size = 16;
    for (size_t i = 0; i < filePaths.size(); i += batch_size) {
        // This is a placeholder for parallel processing
        for (size_t j = i; j < std::min(i + batch_size, filePaths.size()); ++j) {
            ingestFile(filePaths[j], false);
        }
    }
}

void Engine::ingestFromSocket(int socket_fd) {
    // This is a placeholder implementation.
    // A real implementation would use a library like Asio or Boost.Asio
    // to wrap the socket file descriptor in a stream object.
    // For now, we'll just log a message.
    std::cout << "Ingesting from socket: " << socket_fd << std::endl;
}

void Engine::ingestFromStream(std::istream& stream) {
    pattern_metric_engine_.ingestData(stream);
    pattern_metric_engine_.evolvePatterns();
}

std::string Engine::processQuantData(const std::string &dataPath, bool useGPU)
{
    try
    {
        // Create data parser
        DataParser parser;

        // Parse the data file (auto-detects format)
        auto patterns = parser.parseFile(dataPath);

        if (patterns.empty())
        {
            nlohmann::json error_json;
            error_json["error"] = "No patterns parsed from file";
            error_json["file"] = dataPath;
            return error_json.dump();
        }

        // Process patterns to calculate basic metrics
        // The quantum algorithms would normally calculate coherence
        // For now, we'll use simple heuristics based on price volatility
        
        for (auto &pattern : patterns)
        {
            // Calculate simple volatility metric from OHLC
            float range = pattern.position.y - pattern.position.z; // high - low
            float avg_price = (pattern.position.x + pattern.position.w) / 2.0f; // (open + close) / 2
            float volatility = (range / avg_price) * 100.0f; // percentage
            
            // Simple coherence calculation (inverse of volatility)
            pattern.coherence = 1.0f / (1.0f + volatility * 0.01f);
            pattern.quantum_state.coherence = pattern.coherence;
            pattern.quantum_state.stability = pattern.coherence;
            pattern.quantum_state.energy = volatility;
        }

        // Build DAG for correlations
        ::sep::dag::DagGraph dag;

        // Add patterns to DAG
        for (const auto &pattern : patterns)
        {
            std::vector<uint64_t> parents;  // No parents for initial patterns

            // Extract position as vec3 for DAG
            glm::vec3 pos(pattern.position.x, pattern.position.y, pattern.position.z);

            // Add to DAG with market data if available
            if (!pattern.data.empty())
            {
                float volume = pattern.data[0];
                dag.addMarketDataNode(pos, pattern.coherence, pattern.position.w, 0.0f, volume,
                                      parents);
            }
            else
            {
                dag.addNode(pos, pattern.coherence, parents);
            }
        }

        // Calculate correlations and metrics
        dag.calculateNodeCorrelations();
        dag.calculateTailRisk();
        dag.calculateAlpha();

        // Export results as JSON
        std::string result = dag.exportAsJson();

        // Add processing metadata
        nlohmann::json metadata;
        metadata["patterns_processed"] = patterns.size();
        metadata["gpu_enabled"] = useGPU;
        metadata["file"] = dataPath;

        // Parse existing result and add metadata
        nlohmann::json final_json = nlohmann::json::parse(result);
        final_json["metadata"] = metadata;

        return final_json.dump(2);  // Pretty print with 2-space indent
    }
    catch (const std::exception &e)
    {
        nlohmann::json error_json;
        error_json["error"] = e.what();
        error_json["file"] = dataPath;
        return error_json.dump();
    }
}

} // namespace core
} // namespace sep
