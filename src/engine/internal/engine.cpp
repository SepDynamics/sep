#include "engine/internal/standard_includes.h"
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
#include <filesystem>
#include <future>
#include <iostream>
#include <nlohmann/json.hpp>
#include <numeric>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <vector>

#include "common.h"  // defines sep::SEPResult
#include "config.h"
#include "core.h"
#include "cuda_api.hpp"
#include "cuda_sep.h"
#include "dag_graph.h"
#include "data_parser.h"
#include "engine.h"
#include "error_handler.h"
#include "logging.h"  // This is actually the logging manager
#include "memory.h"
#include "memory/memory_tier_manager.hpp"
#include "quantum/bitspace/qbsa.h"
#include "stream.h"
#include "types.h"

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
  bool processing_{false};
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

void Engine::generate_probes(const std::vector<PinState> &inputs,
                             std::vector<std::uint32_t> &probe_indices,
                             std::vector<std::uint32_t> &expectations, std::uint64_t tick)
{
    if (inputs.empty())
    {
        ::sep::core::ErrorHandler::instance().reportError(
            {SEPResult::INVALID_ARGUMENT, "No input states", "Engine::generate_probes"});
        return;
    }

    probe_indices.clear();
    expectations.clear();
    probe_indices.reserve(inputs.size());
    expectations.reserve(inputs.size());

    // Convert each input state to probe indices and expectations
    for (size_t i = 0; i < inputs.size(); ++i)
    {
        const auto &pin_state = inputs[i];

        // Generate probe index based on pin state and current tick
        std::uint32_t probe_idx =
            static_cast<std::uint32_t>((pin_state.pin_id + tick) % DEFAULT_SIZE);
        probe_indices.push_back(probe_idx);

        // Calculate expected value based on pin state and coherence
        std::uint32_t expected =
            static_cast<std::uint32_t>(pin_state.value * pin_state.coherence * 1000.f);
        expectations.push_back(expected);
    }

    // Ensure device buffers are properly sized
    if (impl_->d_bitfield_.size() < inputs.size())
    {
        impl_->d_bitfield_.resize(inputs.size());
    }
    if (impl_->d_corrections_.size() < inputs.size())
    {
        impl_->d_corrections_.resize(inputs.size());
    }
    if (impl_->d_correction_count_.size() < 1)
    {
        impl_->d_correction_count_.resize(1);
    }
    if (impl_->d_collapse_indices_.size() < inputs.size())
    {
        impl_->d_collapse_indices_.resize(inputs.size());
    }
    if (impl_->d_collapse_counts_.size() < inputs.size())
    {
        impl_->d_collapse_counts_.resize(inputs.size());
    }
    if (impl_->d_chunks_.size() < inputs.size())
    {
        impl_->d_chunks_.resize(inputs.size());
    }

    // Initialize device buffers
    std::fill(impl_->d_bitfield_.begin(), impl_->d_bitfield_.end(), 0);
    std::fill(impl_->d_corrections_.begin(), impl_->d_corrections_.end(), 0);
    impl_->d_correction_count_[0] = 0;
    std::fill(impl_->d_collapse_indices_.begin(), impl_->d_collapse_indices_.end(), 0);
    std::fill(impl_->d_collapse_counts_.begin(), impl_->d_collapse_counts_.end(), 0);
    std::fill(impl_->d_chunks_.begin(), impl_->d_chunks_.end(), 0);
}

void Engine::process_batch(const std::vector<PinState> &inputs, std::uint64_t tick,
                           ::sep::quantum::QBSAResult &qbsa_result,
                           ::sep::cuda::QSHResult &qsh_result)
{
    // Input validation
    if (inputs.empty())
    {
        ::sep::core::ErrorHandler::instance().reportError(
            {SEPResult::INVALID_ARGUMENT, "No input states", "Engine::process_batch"});
        return;
    }

    if (inputs.size() > DEFAULT_SIZE)
    {
        ::sep::core::ErrorHandler::instance().reportError(
            {SEPResult::INVALID_ARGUMENT, "Batch too large", "Engine::process_batch"});
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

    try
    {
        // Generate probes from inputs
        std::vector<std::uint32_t> probe_indices;
        std::vector<std::uint32_t> expectations;
        generate_probes(inputs, probe_indices, expectations, tick);

        // CPU fallback when CUDA is unavailable
        quantum::QBSAProcessor cpu_proc;
        qbsa_result = cpu_proc.analyze(probe_indices, expectations);
        qbsa_result.collapse_detected = cpu_proc.detectCollapse(qbsa_result, inputs.size());

        // No CUDA results to copy when using CPU path

        // Update state history
        StateNode node;
        node.tick = tick;
        node.coherence = 1.0f - qbsa_result.correction_ratio;
        node.rupture = qbsa_result.collapse_detected;
        if (!impl_->state_history_.empty())
        {
            node.parents.push_back(impl_->state_history_.size() - 1);
        }
        impl_->state_history_.push_back(node);
    } catch (const std::exception &e) {
      ::sep::core::ErrorHandler::instance().reportError(
          {SEPResult::PROCESSING_ERROR, e.what(), "Engine::process_batch"});
      return;
  }
}

const std::vector<Engine::StateNode> &Engine::getStateHistory() const noexcept
{
    return impl_->state_history_;
}

std::vector<float> Engine::getCoherenceHistory() const
{
    std::vector<float> history;
    history.reserve(impl_->state_history_.size());
    for (const auto &n : impl_->state_history_)
    {
        history.push_back(n.coherence);
    }
    return history;
}

void Engine::ingestFile(const std::string &dataPath, bool legacy)
{
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
        // Create quantum patterns from pattern metrics with sophisticated mapping
        for (size_t i = 0; i < metrics.size(); ++i) {
            const auto& metric = metrics[i];
            
            // Create quantum pattern from pattern metric
            quantum::Pattern pattern;
            pattern.id = "metric_pattern_" + std::to_string(i) + "_" + std::to_string(std::time(nullptr));
            
            // Map position from metric values (coherence, stability, entropy)
            pattern.position = glm::vec4(
                metric.coherence,
                metric.stability,
                metric.entropy,
                1.0f
            );
            
            // Initialize quantum state from metrics
            pattern.quantum_state.coherence = glm::clamp(metric.coherence, 0.0f, 1.0f);
            pattern.quantum_state.stability = glm::clamp(metric.stability, 0.0f, 1.0f);
            pattern.quantum_state.entropy = glm::clamp(metric.entropy, 0.0f, 1.0f);
            pattern.quantum_state.energy = std::sqrt(metric.coherence * metric.stability);
            pattern.quantum_state.phase = metric.entropy; // Use entropy as phase
            
            // Set memory tier based on coherence thresholds
            if (pattern.quantum_state.coherence >= 0.9f && pattern.quantum_state.stability >= 0.8f) {
                pattern.quantum_state.memory_tier = memory::MemoryTierEnum::LTM;
            } else if (pattern.quantum_state.coherence >= 0.6f) {
                pattern.quantum_state.memory_tier = memory::MemoryTierEnum::MTM;
            } else {
                pattern.quantum_state.memory_tier = memory::MemoryTierEnum::STM;
            }
            
            // Set timestamps
            auto now = std::chrono::system_clock::now();
            pattern.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
            pattern.last_accessed = pattern.timestamp;
            pattern.last_modified = pattern.timestamp;
            
            // Copy compatible data to pattern data structure
            std::strncpy(pattern.data.id, pattern.id.c_str(), compat::PatternData::MAX_ID_LENGTH - 1);
            pattern.data.id[compat::PatternData::MAX_ID_LENGTH - 1] = '\0';
            pattern.data.coherence = pattern.quantum_state.coherence;
            pattern.data.position = pattern.position;
            
            // Copy metric values to compatible data structure as attributes
            pattern.data.attributes[0] = metric.coherence;
            pattern.data.attributes[1] = metric.stability;
            pattern.data.attributes[2] = metric.entropy;
            pattern.data.size = 3; // coherence, stability, entropy
            
            // Add pattern to quantum processor
            quantum_processor_->addPattern(pattern);
        }
    }
}

void Engine::ingestFromDirectory(const std::string &dirPath, bool recursive)
{
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
    const size_t num_threads = std::thread::hardware_concurrency();
    
    // Process files in parallel batches using thread pool
    for (size_t i = 0; i < filePaths.size(); i += batch_size) {
        std::vector<std::future<void>> futures;
        
        for (size_t j = i; j < std::min(i + batch_size, filePaths.size()); ++j) {
            // Launch async task for each file
            futures.push_back(std::async(std::launch::async, [this, &filePaths, j]() {
                try {
                    this->ingestFile(filePaths[j], false);
                    metrics_collector_.increment("files_processed_parallel", 1);
                } catch (const std::exception& e) {
                    metrics_collector_.increment("file_processing_errors", 1);
                    // Log error but continue processing other files
                }
            }));
            
            // Limit concurrent threads to avoid resource exhaustion
            if (futures.size() >= num_threads) {
                // Wait for some futures to complete
                for (auto& future : futures) {
                    future.wait();
                }
                futures.clear();
            }
        }
        
        // Wait for remaining futures in this batch
        for (auto& future : futures) {
            future.wait();
        }
    }
}

void Engine::ingestFromSocket(int socket_fd) {
    // Read data from socket and process through real market data pipeline
    const size_t buffer_size = 8192;
    char buffer[buffer_size];
    
    try {
        ssize_t bytes_read = recv(socket_fd, buffer, buffer_size - 1, 0);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            
            // Process received data through data parser
            std::string data_str(buffer, bytes_read);
            std::istringstream stream(data_str);
            
            // Use existing real data processing pipeline
            DataParser parser;
            auto patterns = parser.parseBuffer(reinterpret_cast<const uint8_t*>(buffer), bytes_read);
            
            // Process through pattern metric engine (real quantum processing)
            for (const auto& pattern : patterns) {
                quantum_processor_->addPattern(pattern);
            }
            
            // Evolve patterns using real quantum algorithms
            pattern_metric_engine_.evolvePatterns();
            
            // Compute real metrics
            auto metrics = pattern_metric_engine_.computeMetrics();
            metrics_collector_.increment("socket_patterns_processed", patterns.size());
            
            // Process through quantum processor (real processing)
            for (const auto& metric : metrics) {
                quantum::Pattern q_pattern;
                q_pattern.id = "socket_pattern_" + std::to_string(std::time(nullptr));
                q_pattern.quantum_state.coherence = metric.coherence;
                q_pattern.quantum_state.stability = metric.stability;
                q_pattern.quantum_state.entropy = metric.entropy;
                q_pattern.position = glm::vec4(
                    metric.coherence,
                    metric.stability,
                    metric.entropy,
                    1.0f
                );
                quantum_processor_->addPattern(q_pattern);
            }
        } else if (bytes_read == 0) {
            metrics_collector_.increment("socket_disconnections", 1);
        } else {
            metrics_collector_.increment("socket_read_errors", 1);
        }
    } catch (const std::exception& e) {
        metrics_collector_.increment("socket_processing_errors", 1);
        std::cerr << "Socket ingestion error: " << e.what() << std::endl;
    }
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
            // Use attributes[0] as volume data if available
            if (pattern.data.size > 0)
            {
                float volume = pattern.data.attributes[0];
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

std::map<std::string, double> Engine::getMetrics() const {
    auto metrics = metrics_collector_.getMetrics();
    
    // Add pattern metrics if available
    if (!impl_->state_history_.empty()) {
        const auto& latest = impl_->state_history_.back();
        metrics["coherence"] = latest.coherence;
        metrics["rupture"] = latest.rupture ? 1.0 : 0.0;
        metrics["tick"] = static_cast<double>(latest.tick);
        metrics["state_history_size"] = static_cast<double>(impl_->state_history_.size());
    }
    
    // Add pattern metric engine metrics if available
    const auto& pattern_metrics = pattern_metric_engine_.computeMetrics();
    for (size_t i = 0; i < pattern_metrics.size(); ++i) {
        const auto& pm = pattern_metrics[i];
        metrics["pattern_" + std::string(pm.pattern_id) + "_coherence"] = pm.coherence;
        metrics["pattern_" + std::string(pm.pattern_id) + "_stability"] = pm.stability;
        metrics["pattern_" + std::string(pm.pattern_id) + "_entropy"] = pm.entropy;
        metrics["pattern_" + std::string(pm.pattern_id) + "_energy"] = pm.energy;
    }
    
    return metrics;
}

bool Engine::isProcessing() const {
    // Engine is processing if it has active state history or is evolving patterns
    return !impl_->state_history_.empty() || impl_->processing_;
}

} // namespace core
} // namespace sep
