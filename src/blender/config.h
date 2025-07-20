#pragma once

#include <cstddef>
#include <cstdint>

// SEP Configuration structure
struct SEPConfig {
    // Pattern processing
    struct {
        float update_threshold;    // Min entropy for updates (default: 0.1)
        float coherence_target;    // Target pattern stability (default: 0.8)
        size_t max_vertices;       // Max vertices per pattern (default: 1M)
        bool enable_mutations;     // Allow pattern evolution (default: true)
    } pattern;

    // Memory settings
    struct {
        float vram_target;         // Target VRAM usage (default: 0.7)
        size_t cache_size;         // Pattern cache size (default: 1024)
        bool use_unified;          // Use unified memory (default: true)
        size_t stm_size;          // STM buffer size (default: 4GB)
        size_t mtm_size;          // MTM buffer size (default: 16GB)
        size_t ltm_size;          // LTM buffer size (default: 32GB)
    } memory;

    // Processing settings
    struct {
        size_t batch_size;         // Update batch size (default: 256)
        float max_latency;         // Max processing time in ms (default: 16.0)
        bool async_updates;        // Enable async processing (default: true)
        bool gpu_acceleration;     // Enable GPU acceleration (default: true)
    } processing;

    // Audio settings
    struct {
        uint32_t sample_rate;      // Sample rate in Hz (default: 48000)
        uint32_t channels;         // Number of channels (default: 2)
        uint32_t buffer_size;      // Buffer size in frames (default: 1024)
        bool rt_processing;        // Real-time processing (default: true)
    } audio;

    // Default configuration
    static SEPConfig getDefault() {
        SEPConfig cfg{};
        cfg.pattern.update_threshold = 0.1f;
        cfg.pattern.coherence_target = 0.8f;
        cfg.pattern.max_vertices = 1000000;
        cfg.pattern.enable_mutations = true;

        cfg.memory.vram_target = 0.7f;
        cfg.memory.cache_size = 1024;
        cfg.memory.use_unified = true;
        cfg.memory.stm_size = 4ULL * 1024 * 1024 * 1024;
        cfg.memory.mtm_size = 16ULL * 1024 * 1024 * 1024;
        cfg.memory.ltm_size = 32ULL * 1024 * 1024 * 1024;

        cfg.processing.batch_size = 256;
        cfg.processing.max_latency = 16.0f;
        cfg.processing.async_updates = true;
        cfg.processing.gpu_acceleration = true;

        cfg.audio.sample_rate = 48000;
        cfg.audio.channels = 2;
        cfg.audio.buffer_size = 1024;
        cfg.audio.rt_processing = true;

        return cfg;
    }
};

// Pattern metrics
struct SEPPatternMetrics {
    size_t active_patterns;       // Current pattern count
    float avg_coherence;          // Average stability
    float peak_entropy;           // Maximum entropy
    uint64_t updates_processed;   // Total updates

    // Performance metrics
    struct {
        float process_time;        // Processing time (ms)
        size_t memory_used;        // Memory usage (bytes)
        float gpu_utilization;     // GPU usage (%)
    } performance;

    // Evolution metrics
    struct {
        size_t mutations;          // Pattern mutations
        float stability;           // Overall stability
        size_t promotions;         // Tier promotions
    } evolution;
};

// Audio processing metrics
struct SEPAudioMetrics {
    float peak_level;             // Peak audio level
    float rms_level;              // RMS audio level
    uint32_t frames_processed;    // Processed frame count
    float latency;                // Processing latency (ms)
    
    struct {
        float cpu_usage;           // CPU utilization (%)
        size_t buffer_overruns;    // Buffer overrun count
        size_t xruns;             // PipeWire xrun count
    } performance;
};
