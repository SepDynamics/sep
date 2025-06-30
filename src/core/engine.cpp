#if defined(SEP_HAS_CUDA) && !defined(SEP_USE_CUDA)
#define SEP_USE_CUDA 1
#endif
#include "compat/cuda_runtime.h"  // for sep::cuda::cudaMemcpyAsync
#include "api/types.h"
#include "core/common.h"  // defines sep::SEPResult
#include "core/engine.h"
#include "core/types.h"
#include "core/error_handler.h"
#include "compat/core.h" // Fix: Include core
#include "compat/shim.h"
#include "compat/cuda_common.h" // Fix: Include cuda_common
#include "compat/component_bridge.h"
#include "compat/macros.h"
#include "compat/memory.h"
#include "compat/stream.h"
#include "memory/manager.h"  // This is actually the logging manager
#include "memory/memory_tier_manager.hpp"
#if SEP_HAS_BLENDER
#include "blender/pattern_bridge.h"
#include "blender/types.h" // For SEPBlenderBridge definition
#endif
#include "audio/capture.h"

#include <cstdint> // Fix: Include cstdint
#include <cstdio> // Fix: Include cstdio
#include <exception> // Fix: Include exception

// Define namespace alias for clarity
namespace logging = sep::logging; // Fix: Use namespace alias

namespace sep {
namespace core {
using namespace ::sep::cuda;

struct Engine::Impl {
    // CPU fallback buffers
    ::sep::shim::vector<std::uint32_t> d_bitfield_;
    ::sep::shim::vector<std::uint32_t> d_probe_indices_;
    ::sep::shim::vector<std::uint32_t> d_expectations_;
    ::sep::shim::vector<std::uint32_t> d_corrections_;
    ::sep::shim::vector<std::uint32_t> d_correction_count_;
    ::sep::shim::vector<std::uint64_t> d_chunks_;
    ::sep::shim::vector<std::uint32_t> d_collapse_indices_;
    ::sep::shim::vector<std::uint32_t> d_collapse_counts_;
    ::sep::shim::vector<StateNode> state_history_;
    ::sep::config::APIConfig config;
    bool initialized{false};
};

Engine::Engine() noexcept(false) : impl_(std::make_unique<Impl>()) {}

bool Engine::init(const sep::config::APIConfig& config) {
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

    try {
        audio_capture_ = ::sep::compat::createAudioCapture();
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
        blender_bridge_ = ::sep::compat::createBlenderBridge();
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
        auto result = audio_capture_->start();
        if (result != audio::AudioError::NONE) {
            spdlog::error("Failed to start audio capture: {}", static_cast<int>(result));
        }
    }
}

void Engine::shutdown() {
    if (audio_capture_) {
        auto result = audio_capture_->stop();
        if (result != audio::AudioError::NONE) {
            spdlog::error("Failed to stop audio capture: {}", static_cast<int>(result));
        }
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
        (void)impl_; // nothing to clean up in CPU-only mode
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
                              ::sep::shim::vector<std::uint32_t>& expectations, std::uint64_t tick) {
    if (inputs.empty()) {
        ::sep::core::ErrorHandler::instance().reportError(
            {sep::SEPResult::INVALID_ARGUMENT, "No input states", "Engine::generate_probes"});
        return;
    }

    indices.resize(inputs.size());
    expectations.resize(inputs.size());
    for (size_t i = 0; i < inputs.size(); ++i) {
        indices[i] = static_cast<std::uint32_t>(i);
        expectations[i] = static_cast<std::uint32_t>(inputs[i].state);
    }
}

void Engine::process_batch(const ::sep::shim::vector<::sep::PinState>& inputs, std::uint64_t tick,
                           ::sep::quantum::QBSAResult& qbsa_result, ::sep::cuda::QSHResult& qsh_result) {
    if (inputs.empty()) {
        ::sep::core::ErrorHandler::instance().reportError({sep::SEPResult::INVALID_ARGUMENT, "No input states", "Engine::process_batch"});
        return;
    }

    if (inputs.size() > DEFAULT_SIZE) {
        ::sep::core::ErrorHandler::instance().reportError({sep::SEPResult::INVALID_ARGUMENT, "Batch too large", "Engine::process_batch"});
        return;
    }

    qbsa_result.corrections.clear();
    qbsa_result.correction_ratio = 0.0f;
    qbsa_result.collapse_detected = false;

    qsh_result.collapse_indices.assign(inputs.size(), {});
    qsh_result.collapse_counts.assign(inputs.size(), 0);
    qsh_result.total_collapses = 0;
    qsh_result.total_states = inputs.size();

    StateNode node;
    node.tick = tick;
    node.coherence = 1.0f;
    node.rupture = false;
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
