#include "audio/config.h"

#include <algorithm>
#include <cmath>
#include <spdlog/spdlog.h>

namespace sep {
namespace audio {

// Global audio pipeline configuration
AudioPipelineConfig g_audio_config;

// AudioCoherenceEngine implementation
float AudioCoherenceEngine::calculateCoherence(const float* pSamples, uint32_t count, float sample_rate)
{
    if (!pSamples || count == 0)
    {
        return 0.0f;
    }

    // Calculate RMS energy for base coherence
    float rms_sum = 0.0f;
    for (uint32_t i = 0; i < count; ++i)
    {
        rms_sum += pSamples[i] * pSamples[i];
    }
    float rms = math::to_float(math::sqrt_safe(static_cast<double>(rms_sum / count)));

    // Calculate spectral coherence using autocorrelation
    float    autocorr_sum = 0.0f;
    uint32_t max_lag      = std::min(count / 4, static_cast<uint32_t>(sample_rate * 0.01f));  // 10ms max lag

    for (uint32_t lag = 1; lag <= max_lag; ++lag)
    {
        float corr = 0.0f;
        for (uint32_t i = 0; i < count - lag; ++i)
        {
            corr += pSamples[i] * pSamples[i + lag];
        }
        autocorr_sum += std::abs(corr / (count - lag));
    }

    float spectral_coherence = autocorr_sum / max_lag;

    // Combine RMS and spectral coherence with quantum-inspired weighting
    float base_coherence = (rms * 0.6f + spectral_coherence * 0.4f);

    // Apply quantum coherence transformation (sigmoid-like)
    float quantum_coherence = 1.0f / (1.0f + math::exp_safe(-5.0f * (base_coherence - 0.5f)));

    return std::clamp(quantum_coherence, 0.0f, 1.0f);
}

uint8_t AudioCoherenceEngine::determineMemoryTier(float coherence, float stability, uint32_t generation_count)
{
    // STM: Low coherence or new patterns
    if (coherence < COHERENCE_THRESHOLD_MTM || generation_count < MIN_GENERATIONS_MTM)
    {
        return 0;  // STM
    }

    // LTM: High coherence, stability, and sufficient generations
    if (coherence >= COHERENCE_THRESHOLD_LTM && stability >= 0.85f && generation_count >= MIN_GENERATIONS_LTM)
    {
        return 2;  // LTM
    }

    // MTM: Medium coherence and stability
    return 1;  // MTM
}

float AudioCoherenceEngine::assessStability(const float* pPatternHistory, uint32_t history_length)
{
    if (!pPatternHistory || history_length < 2)
    {
        return 0.0f;
    }

    // Calculate variance in pattern coherence over time
    float mean = 0.0f;
    for (uint32_t i = 0; i < history_length; ++i)
    {
        mean += pPatternHistory[i];
    }
    mean /= history_length;

    float variance = 0.0f;
    for (uint32_t i = 0; i < history_length; ++i)
    {
        float diff = pPatternHistory[i] - mean;
        variance += diff * diff;
    }
    variance /= history_length;

    // Stability is inverse of variance, normalized
    float stability = 1.0f / (1.0f + variance);

    return std::clamp(stability, 0.0f, 1.0f);
}

// Configuration management
bool initializeAudioConfig(const AudioPipelineConfig& config)
{
    g_audio_config = config;

    // Validate configuration
    if (!validateAudioConfig(g_audio_config))
    {
        spdlog::error("Audio configuration validation failed");
        return false;
    }

    // Observability instrumentation removed

    spdlog::info("Audio pipeline configuration initialized");
    spdlog::info("  Coherence threshold: {}", g_audio_config.quantum.coherence_threshold);
    spdlog::info("  Stability threshold: {}", g_audio_config.quantum.stability_threshold);
    spdlog::info("  Pattern buffer size: {}", g_audio_config.quantum.pattern_buffer_size);
    spdlog::info("  STM retention: {}ms", g_audio_config.memory_tiers.stm_retention_ms);
    spdlog::info("  MTM retention: {}ms", g_audio_config.memory_tiers.mtm_retention_ms);

    return true;
}

bool validateAudioConfig(const AudioPipelineConfig& config)
{
    // Validate quantum parameters
    if (config.quantum.coherence_threshold < 0.0f || config.quantum.coherence_threshold > 1.0f)
    {
        spdlog::error("Invalid coherence threshold: {}", config.quantum.coherence_threshold);
        return false;
    }

    if (config.quantum.stability_threshold < 0.0f || config.quantum.stability_threshold > 1.0f)
    {
        spdlog::error("Invalid stability threshold: {}", config.quantum.stability_threshold);
        return false;
    }

    // Validate memory tier parameters
    if (config.memory_tiers.max_patterns_stm == 0 || config.memory_tiers.max_patterns_mtm == 0)
    {
        spdlog::error("Invalid memory tier pattern limits");
        return false;
    }

    // Validate base audio config
    if (config.base_config.rate == 0 || config.base_config.channels == 0)
    {
        spdlog::error("Invalid base audio configuration");
        return false;
    }

    return true;
}

}  // namespace audio
}  // namespace sep
