#include "compat/math_common.h"

#include "blender/types.h"
#include "blender/config.h"
#include "blender/pattern_bridge.h"
#include "compat/component_bridge.h"
#include <memory>
#include <string>

namespace {
// Version information
constexpr char VERSION[]    = "1.0.0";
constexpr char BUILD_INFO[] = "Built with CUDA 12.9.0, clang";

// Helper to validate bridge instance
bool validateBridge(SEPBlenderBridge* bridge)
{
    return bridge && bridge->impl;
}

// Helper to validate mesh data
bool validateMesh(Object* obj, Mesh* mesh)
{
    if (!obj || !mesh)
        return false;
    if (obj->type != OB_MESH)
        return false;
    if (static_cast<size_t>(mesh->totvert) > SEPConfig::getDefault().pattern.max_vertices)
        return false;
    return true;
}
}  // namespace

extern "C" sep::SEPResult sep_blender_init(sep::GPUContext* gpu_ctx, const SEPConfig* config, SEPBlenderBridge** bridge_out)
{
    if (!gpu_ctx || !bridge_out)
    {
        return sep::SEPResult::INITIALIZATION_FAILED; // Fix: Return appropriate error
    }

    // Use config parameter to avoid unused warning
    (void)config;

    auto bridge_ptr = sep::compat::createBlenderBridge();
    if (!bridge_ptr)
    {
        return sep::SEPResult::ALLOCATION_FAILED;
    }
    bridge_ptr->audio_metrics   = SEPAudioMetrics{};
    bridge_ptr->pattern_metrics = SEPPatternMetrics{};

    auto result = bridge_ptr->impl->init(gpu_ctx);
    if (static_cast<int32_t>(result) != static_cast<int32_t>(sep::SEPResult::SUCCESS))
    {
        // bridge_ptr goes out of scope and cleans up
        return static_cast<sep::SEPResult>(static_cast<int32_t>(result));
    }
    *bridge_out = bridge_ptr.release();  // Release ownership to the caller
    return sep::SEPResult::SUCCESS;
}

extern "C" sep::SEPResult
sep_register_mesh(SEPBlenderBridge* bridge, Object* bl_object, Mesh* bl_mesh, sep::pattern::ObjectHandle* handle_out)
{
    if (!validateBridge(bridge) || !validateMesh(bl_object, bl_mesh) || !handle_out)
    {
        return sep::SEPResult::NOT_FOUND;
    }

    sep::pattern::PatternConfig config{};
    config.update_threshold = 0.1f;
    config.enable_mutations = true;
    config.max_patterns     = 1000;
    config.batch_size       = 64;

    auto result = bridge->impl->registerObject(bl_object, config, handle_out);
    return static_cast<sep::SEPResult>(static_cast<int32_t>(result));
}

extern "C" sep::SEPResult
sep_update_mesh(SEPBlenderBridge* bridge, sep::pattern::ObjectHandle handle, const SEPPatternMetrics* data, bool* updated_out)
{
    if (!validateBridge(bridge) || !data || !updated_out)
    {
        return sep::SEPResult::NOT_FOUND;
    }

    {
        sep::pattern::PatternMetrics metrics{};
        metrics.avg_coherence                       = data->avg_coherence;
        metrics.peak_entropy                        = data->peak_entropy;
        metrics.active_patterns                     = data->active_patterns;
        metrics.updates_processed                   = data->updates_processed;
        metrics.performance.process_time            = data->performance.process_time;
        metrics.performance.gpu_utilization         = data->performance.gpu_utilization;
        metrics.evolution.mutations                 = data->evolution.mutations;
        metrics.evolution.stability                 = data->evolution.stability;
        metrics.evolution.mtm_candidates            = data->evolution.promotions;
        metrics.evolution.ltm_candidates            = 0;
        metrics.evolution.avg_relationship_strength = 0.0f;
        metrics.evolution.relationship_count        = 0;
        metrics.evolution.path_count                = 0;
        metrics.evolution.avg_path_length           = 0.0f;

        auto result = bridge->impl->updateObject(handle, metrics);
        auto converted_result = static_cast<sep::SEPResult>(static_cast<int32_t>(result));
        *updated_out = (converted_result == sep::SEPResult::SUCCESS);
        return converted_result;
    }
}

extern "C" sep::SEPResult
sep_process_audio(SEPBlenderBridge* bridge, const float* samples, size_t count, SEPAudioMetrics* metrics_out)
{
    if (!validateBridge(bridge) || !samples || !metrics_out || count == 0)
    {
        return sep::SEPResult::NOT_FOUND;
    }

    float peak    = 0.0f;
    float rms_sum = 0.0f;

    for (size_t i = 0; i < count; ++i)
    {
        float abs_sample = std::fabs(samples[i]);
        peak             = std::max(peak, abs_sample);
        rms_sum += abs_sample * abs_sample;
    }

    metrics_out->peak_level            = peak;
    metrics_out->rms_level             = sep::math::to_float(sep::math::sqrt_safe(static_cast<double>(rms_sum / count))); 
    metrics_out->frames_processed      = static_cast<uint32_t>(count);
    metrics_out->latency               = 0.0f;
    metrics_out->performance.cpu_usage = 0.0f;
    metrics_out->performance.buffer_overruns = 0;
    metrics_out->performance.xruns           = 0;

    bridge->audio_metrics = *metrics_out;
    return sep::SEPResult::SUCCESS;
}

extern "C" sep::SEPResult sep_sync_memory(SEPBlenderBridge* bridge, sep::MemoryTierEnum tier, bool force)
{
    if (!validateBridge(bridge))
    {
        return sep::SEPResult::NOT_FOUND;
    }

    auto result = bridge->impl->syncMemory(tier, force);
    return static_cast<sep::SEPResult>(static_cast<int32_t>(result));
}

extern "C" sep::SEPResult sep_blender_cleanup(SEPBlenderBridge* bridge)
{
    if (!validateBridge(bridge))
    {
        return sep::SEPResult::NOT_FOUND;
    }

    delete bridge;
    return sep::SEPResult::SUCCESS;
}

extern "C" sep::SEPResult sep_get_metrics(SEPBlenderBridge* bridge, SEPPatternMetrics* metrics_out)
{
    if (!validateBridge(bridge) || !metrics_out)
    {
        return sep::SEPResult::NOT_FOUND;
    }

    *metrics_out = bridge->pattern_metrics;
    return sep::SEPResult::SUCCESS;
}

extern "C" sep::SEPResult sep_reset_state(SEPBlenderBridge* bridge)
{
    if (!validateBridge(bridge))
    {
        return sep::SEPResult::NOT_FOUND;
    }

    bridge->audio_metrics   = SEPAudioMetrics{};
    bridge->pattern_metrics = SEPPatternMetrics{};
    return sep::SEPResult::SUCCESS;
}

extern "C" const char* sep_get_version(void)
{
    return VERSION;
}

extern "C" const char* sep_get_build_info(void)
{
    return BUILD_INFO;
}
