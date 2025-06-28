#include "core/engine.h"

#include "audio/capture.h"
#include "blender/pattern_bridge.h"
#include "memory/manager.h"

#include "compat/shim.h"
#include "core/error_handler.h"
#include <cstdint>
#include <cstdio>
#include <exception>

#include "compat/core.h"
#include "compat/cuda_common.h"

#include "compat/macros.h"
#include "compat/memory.h"
#include "compat/stream.h"
#include "api/types.h"

#ifndef SEP_HAS_EXCEPTIONS
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define SEP_HAS_EXCEPTIONS 1
#else
#define SEP_HAS_EXCEPTIONS 0
#endif
#endif

namespace sep {
namespace core {

using namespace ::sep::cuda;

struct Engine::Impl {
    ::sep::cuda::StreamPtr stream_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_bitfield_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_probe_indices_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_expectations_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_corrections_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_correction_count_;
    ::sep::cuda::DeviceMemory<std::uint64_t> d_chunks_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_collapse_indices_;
    ::sep::cuda::DeviceMemory<std::uint32_t> d_collapse_counts_;
    ::sep::shim::vector<StateNode> state_history_;
    ::sep::config::APIConfig config;
    bool initialized{false};
};

Engine::Engine() noexcept(false) : impl_(std::make_unique<Impl>()) {}

bool Engine::init(const sep::config::APIConfig& config) {
    impl_->config = config;
    printf("DEBUG: Engine::init - Before CudaCore instance\n");
    fflush(stdout);
    
    auto& cuda_core = cuda::CudaCore::instance();
    printf("DEBUG: Engine::init - Got CudaCore instance\n");
    fflush(stdout);
    
    // Try GPU device 0 first with extensive error logging
    printf("DEBUG: Engine::init - Trying GPU device 0\n");
    fflush(stdout);
    
    auto init_err = cuda_core.initialize(0);
    printf("DEBUG: Engine::init - initialize(0) returned code %d, message: %s\n",
           static_cast<int>(init_err.code), init_err.message.c_str());
    fflush(stdout);
    
    if (init_err.code != sep::SEPResult::SUCCESS) {
        printf("DEBUG: Engine::init - Trying GPU device 1\n");
        fflush(stdout);
        
        // Try GPU device 1 as fallback
        init_err = cuda_core.initialize(1);
        printf("DEBUG: Engine::init - initialize(1) returned code %d, message: %s\n",
               static_cast<int>(init_err.code), init_err.message.c_str());
        fflush(stdout);
        
        if (init_err.code != sep::SEPResult::SUCCESS) {
            printf("DEBUG: Engine::init - All GPU devices failed, giving up\n");
            fflush(stdout);
            return false;
        }
    }
    
    printf("DEBUG: Engine::init - CUDA initialized successfully\n");
    fflush(stdout);
    
    // Create default stream
    printf("DEBUG: Engine::init - Creating CUDA stream\n");
    fflush(stdout);
    
    impl_->stream_ = cuda_core.createStream(sep::StreamFlags::Default);
    if (!impl_->stream_) {
        printf("DEBUG: Engine::init - Failed to create CUDA stream\n");
        fflush(stdout);
        return false;
    }
    printf("DEBUG: Engine::init - CUDA stream created successfully\n");
    fflush(stdout);

    // Allocate device memory
    printf("DEBUG: Engine::init - Allocating device memory\n");
    fflush(stdout);
    
    try {
        impl_->d_bitfield_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
        impl_->d_probe_indices_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
        impl_->d_expectations_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
        impl_->d_corrections_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
        impl_->d_correction_count_ = cuda::DeviceMemory<std::uint32_t>(1);
        impl_->d_chunks_ = cuda::DeviceMemory<std::uint64_t>(DEFAULT_SIZE);
        impl_->d_collapse_indices_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE * PAIRS_PER_CHUNK);
        impl_->d_collapse_counts_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
        
        printf("DEBUG: Engine::init - Device memory allocated successfully\n");
        fflush(stdout);
    } catch (const std::exception& e) {
        printf("DEBUG: Engine::init - Exception during device memory allocation: %s\n", e.what());
        fflush(stdout);
        return false;
    } catch (...) {
        printf("DEBUG: Engine::init - Unknown exception during device memory allocation\n");
        fflush(stdout);
        return false;
    }

    // Create default stream
    impl_->stream_ = cuda_core.createStream(sep::StreamFlags::Default);
    if (!impl_->stream_) {
        return false;
    }

    impl_->d_bitfield_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
    impl_->d_probe_indices_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
    impl_->d_expectations_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
    impl_->d_corrections_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);
    impl_->d_correction_count_ = cuda::DeviceMemory<std::uint32_t>(1);
    impl_->d_chunks_ = cuda::DeviceMemory<std::uint64_t>(DEFAULT_SIZE);
    impl_->d_collapse_indices_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE * PAIRS_PER_CHUNK);
    impl_->d_collapse_counts_ = cuda::DeviceMemory<std::uint32_t>(DEFAULT_SIZE);

    printf("DEBUG: Engine::init - Initializing audio capture\n");
    fflush(stdout);
    
    try {
        audio_capture_ = audio::AudioCapture::create();
        if (!audio_capture_) {
            printf("DEBUG: Engine::init - Failed to create audio capture\n");
            fflush(stdout);
        } else {
            auto err = audio_capture_->init(audio::AudioConfig{});
            if (err != audio::AudioError::NONE) {
                printf("DEBUG: Engine::init - Audio capture init failed with error %d\n", static_cast<int>(err));
                fflush(stdout);
                audio_capture_.reset();
            } else {
                printf("DEBUG: Engine::init - Audio capture initialized successfully\n");
                fflush(stdout);
            }
        }
    } catch (const std::exception& e) {
        printf("DEBUG: Engine::init - Exception during audio capture init: %s\n", e.what());
        fflush(stdout);
    } catch (...) {
        printf("DEBUG: Engine::init - Unknown exception during audio capture init\n");
        fflush(stdout);
    }

#if SEP_HAS_BLENDER
    printf("DEBUG: Engine::init - Initializing Blender bridge\n");
    fflush(stdout);
    
    try {
        blender_bridge_ = std::make_shared<pattern::BlenderBridge>();
        printf("DEBUG: Engine::init - Blender bridge created successfully\n");
        fflush(stdout);
    } catch (const std::exception& e) {
        printf("DEBUG: Engine::init - Exception during Blender bridge creation: %s\n", e.what());
        fflush(stdout);
    } catch (...) {
        printf("DEBUG: Engine::init - Unknown exception during Blender bridge creation\n");
        fflush(stdout);
    }
#endif

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
        audio_capture_->start();
    }
}

void Engine::shutdown() {
    if (audio_capture_) {
        audio_capture_->stop();
    }
}

namespace {
#if SEP_HAS_EXCEPTIONS
void log_cleanup_exception(const std::exception* ex) noexcept {
    try {
        if (ex) {
            (void)fprintf(stderr, "Warning: Exception during Engine cleanup: %s\n", ex->what());
        } else {
            (void)fprintf(stderr, "%s\n", "Warning: Unknown exception during Engine cleanup");
        }
    } catch (...) {
        std::terminate();
    }
}
#endif
}  // namespace

Engine::~Engine() {
#if SEP_HAS_EXCEPTIONS
    try {
#endif
        if (impl_ && impl_->stream_) {
            auto& cuda_core = cuda::CudaCore::instance();
            if (cuda_core.is_initialized()) {
                cuda_core.synchronizeStream(static_cast<cudaStream_t>(impl_->stream_->handle()));
                impl_->stream_.reset();
            }
        }
#if SEP_HAS_EXCEPTIONS
    } catch (const std::exception& e) {
        log_cleanup_exception(&e);
    } catch (...) {
        log_cleanup_exception(nullptr);
    }
#endif
}

void Engine::generate_probes(const ::sep::shim::vector<::sep::PinState>& inputs,
                             ::sep::shim::vector<std::uint32_t>& indices,
                             ::sep::shim::vector<std::uint32_t>& expectations, std::uint64_t tick ) {
    if (inputs.empty()) {
        ::sep::core::ErrorHandler::instance().reportError(
            {sep::SEPResult::INVALID_ARGUMENT, "No input states", "Engine::generate_probes"});
        return;
    }

    auto& cuda_core = cuda::CudaCore::instance();

    // Copy input states to device
    SEP_CUDA_CHECK(cudaMemcpyAsync(impl_->d_chunks_.get(), reinterpret_cast<const std::uint64_t*>(inputs.data()),
                                   inputs.size() * sizeof(std::uint64_t), cudaMemcpyHostToDevice,
                                   reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    // Process batch to generate probes
    indices.resize(inputs.size());
    expectations.resize(inputs.size());

    cuda_core.launchQBSA(impl_->d_probe_indices_, impl_->d_expectations_, static_cast<std::uint32_t>(inputs.size()),
                         impl_->d_bitfield_, impl_->d_corrections_, impl_->d_correction_count_, *impl_->stream_);

    // Copy results back to host
    SEP_CUDA_CHECK(cudaMemcpyAsync(indices.data(), impl_->d_probe_indices_.get(), inputs.size() * sizeof(std::uint32_t),
                                   cudaMemcpyDeviceToHost, reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    SEP_CUDA_CHECK(cudaMemcpyAsync(expectations.data(), impl_->d_expectations_.get(),
                                   inputs.size() * sizeof(std::uint32_t), cudaMemcpyDeviceToHost,
                                   reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    // Synchronize to ensure all operations are complete
    cuda_core.synchronizeStream(static_cast<cudaStream_t>(impl_->stream_->handle()));
}

void Engine::process_batch(const ::sep::shim::vector<::sep::PinState>& inputs, std::uint64_t tick ,
                           ::sep::quantum::QBSAResult& qbsa_result, ::sep::cuda::QSHResult& qsh_result) {
    if (inputs.empty()) {
        ::sep::core::ErrorHandler::instance().reportError(
            {sep::SEPResult::INVALID_ARGUMENT, "No input states", "Engine::process_batch"});
        return;
    }

    if (inputs.size() > DEFAULT_SIZE) {
        ::sep::core::ErrorHandler::instance().reportError(
            {sep::SEPResult::INVALID_ARGUMENT, "Batch too large", "Engine::process_batch"});
        return;
    }

    auto& cuda_core = cuda::CudaCore::instance();

    // Copy input states to device
    SEP_CUDA_CHECK(cudaMemcpyAsync(impl_->d_chunks_.get(), reinterpret_cast<const std::uint64_t*>(inputs.data()),
                                   inputs.size() * sizeof(std::uint64_t), cudaMemcpyHostToDevice,
                                   reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    // Process quantum bit states
    cuda_core.launchQBSA(impl_->d_probe_indices_, impl_->d_expectations_, static_cast<std::uint32_t>(inputs.size()),
                         impl_->d_bitfield_, impl_->d_corrections_, impl_->d_correction_count_, *impl_->stream_);

    // Process quantum state history
    cuda_core.launchQSH(impl_->d_chunks_, static_cast<std::uint32_t>(inputs.size()), impl_->d_collapse_indices_,
                        impl_->d_collapse_counts_, *impl_->stream_);

    // Copy QBSA results
    std::uint32_t correction_count = 0;
    SEP_CUDA_CHECK(cudaMemcpyAsync(&correction_count, impl_->d_correction_count_.get(), sizeof(std::uint32_t),
                                   cudaMemcpyDeviceToHost, reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    // Set corrections count and calculate correction ratio
    qbsa_result.corrections.clear();
    qbsa_result.corrections.resize(correction_count);  // Will be empty for now
    qbsa_result.correction_ratio = static_cast<float>(correction_count) / inputs.size();
    qbsa_result.collapse_detected = (qbsa_result.correction_ratio > 0.3f);  // 30% threshold for collapse

    if (correction_count > 0) {
        ::sep::shim::vector<uint32_t> temp_corr(correction_count);
        SEP_CUDA_CHECK(cudaMemcpyAsync(temp_corr.data(), impl_->d_corrections_.get(),
                                       correction_count * sizeof(uint32_t), cudaMemcpyDeviceToHost,
                                       reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));
        cuda_core.synchronizeStream(reinterpret_cast<cudaStream_t>(impl_->stream_->handle()));
        // Note: correction data is available in temp_corr if needed for analysis
    }

    // Copy QSH results
    qsh_result.collapse_indices.clear();
    qsh_result.collapse_indices.resize(inputs.size());
    qsh_result.collapse_counts.resize(inputs.size());
    qsh_result.total_collapses = 0;
    qsh_result.total_states = inputs.size();

    // Temporary buffer for all possible indices
    ::sep::shim::vector<uint32_t> temp_indices(inputs.size() * PAIRS_PER_CHUNK);

    SEP_CUDA_CHECK(cudaMemcpyAsync(temp_indices.data(), impl_->d_collapse_indices_.get(),
                                   temp_indices.size() * sizeof(std::uint32_t), cudaMemcpyDeviceToHost,
                                   reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    SEP_CUDA_CHECK(cudaMemcpyAsync(qsh_result.collapse_counts.data(), impl_->d_collapse_counts_.get(),

                                   inputs.size() * sizeof(std::uint32_t), cudaMemcpyDeviceToHost,
                                   reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));

    // Wait for copies to complete before processing
    cuda_core.synchronizeStream(reinterpret_cast<cudaStream_t>(impl_->stream_->handle()));

    // Convert flat indices to vector of vectors
    qsh_result.total_collapses = 0;

    for (size_t i = 0; i < inputs.size(); ++i) {
        const size_t base_idx = i * PAIRS_PER_CHUNK;
        const size_t count = qsh_result.collapse_counts[i];

        // Initialize vector for this input's collapse indices
        // collapse_indices uses std::vector, so deduce the correct type
        auto& indices = qsh_result.collapse_indices[i];
        indices.clear();
        indices.reserve(count);

        bool rupture = false;
        size_t actual_count = 0;
        for (size_t j = 0; j < count && j < PAIRS_PER_CHUNK; ++j) {
            uint32_t val = temp_indices[base_idx + j];
            if (val == 0xFFFFFFFFU) {
                rupture = true;
            } else {
                indices.push_back(val);
                actual_count++;
            }
        }

        // Note: rupture detection for analysis (not stored in result struct)
        if (rupture) {
            // Could log or handle rupture detection here if needed
        }

        qsh_result.total_collapses += actual_count;
    }

    // Synchronize to ensure all operations are complete
    cuda_core.synchronizeStream(static_cast<cudaStream_t>(impl_->stream_->handle()));

    qsh_result.total_collapses = 0;
    for (const auto& count : qsh_result.collapse_counts) {
        qsh_result.total_collapses += count;
    }

    // Update state DAG
    StateNode node;
    node.tick = tick;
    // Calculate coherence from collapse ratio (fewer collapses = higher coherence)
    node.coherence =
        qsh_result.total_states > 0
            ? 1.0f - (static_cast<float>(qsh_result.total_collapses) / static_cast<float>(qsh_result.total_states))
            : 0.0f;
    // Detect rupture based on collapse threshold (>30% collapse rate indicates rupture)
    node.rupture = qsh_result.total_states > 0 && (static_cast<float>(qsh_result.total_collapses) /
                                                   static_cast<float>(qsh_result.total_states)) > 0.3f;
    if (!impl_->state_history_.empty()) {
        node.parents.push_back(impl_->state_history_.size() - 1);
    }
    impl_->state_history_.push_back(node);
}

const ::sep::shim::vector<Engine::StateNode>& Engine::getStateHistory() const noexcept {
    return impl_->state_history_;
}

::sep::shim::vector<float> Engine::getCoherenceHistory() const {
    ::sep::shim::vector<float> history;
    history.reserve(impl_->state_history_.size());
    for (const auto& n : impl_->state_history_) {
        history.push_back(n.coherence);
    }
    return history;
}

}  // namespace core
}  // namespace sep
