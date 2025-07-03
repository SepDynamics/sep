#pragma once

/**
 * @brief Configuration structures and helpers for the audio pipeline.
 */

#ifdef SEP_HAS_AUDIO
#include "audio/types.h"
#include "compat/math_common.h"


namespace sep {
namespace audio {

// Audio pipeline configuration with quantum coherence thresholds
struct AudioPipelineConfig {
    // Core audio parameters
    AudioConfig base_config;

    // Quantum coherence parameters for pattern processing
    struct {
        float coherence_threshold = 0.7f;      // MTM promotion threshold
        float stability_threshold = 0.85f;     // LTM stability requirement
        uint32_t pattern_buffer_size = 4096;   // Pattern analysis window
        uint32_t coherence_window_ms = 100;    // Coherence calculation window
    } quantum;

    // Memory tier configuration for audio patterns
    struct {
        uint32_t stm_retention_ms = 3600000;   // 1 hour STM retention
        uint32_t mtm_retention_ms = 604800000; // 7 day MTM retention
        uint32_t max_patterns_stm = 1000;      // STM pattern limit
        uint32_t max_patterns_mtm = 10000;     // MTM pattern limit
    } memory_tiers;

    // PipeWire-specific configuration
    struct {
        bool enable_monitor_capture = true;
        bool enable_rt_priority = true;
        uint32_t thread_priority = 88;
        std::string client_name = "SEP_Audio_Pipeline";
        std::string node_description = "SEP Quantum Audio Processor";
    } pipewire;

};

// Audio pattern coherence calculator
class AudioCoherenceEngine {
public:
    // Calculate pattern coherence score (0.0-1.0)
    static float calculateCoherence(const float* pSamples, uint32_t count, float sample_rate);

    // Determine memory tier based on coherence and stability
    static uint8_t determineMemoryTier(float coherence, float stability, uint32_t generation_count);

    // Pattern stability assessment
    static float assessStability(const float* pPatternHistory, uint32_t history_length);

private:
    static constexpr float COHERENCE_THRESHOLD_STM = 0.1f;
    static constexpr float COHERENCE_THRESHOLD_MTM = 0.7f;
    static constexpr float COHERENCE_THRESHOLD_LTM = 0.9f;
    static constexpr uint32_t MIN_GENERATIONS_MTM = 5;
    static constexpr uint32_t MIN_GENERATIONS_LTM = 100;
};

// Global audio pipeline configuration instance
extern AudioPipelineConfig g_audio_config;

// Configuration initialization and validation
bool initializeAudioConfig(const AudioPipelineConfig& config = {});
bool validateAudioConfig(const AudioPipelineConfig& config);

}  // namespace audio
}  // namespace sep
#endif // SEP_HAS_AUDIO
