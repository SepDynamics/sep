// Project headers
#include "audio/pipewire_capture.h"
#include "audio/config.h"
#include "audio/pipewire_includes.h"
#include "compat/component_bridge.h"
#include "compat/math_common.h"

// Standard library headers
#include <new>
#include <cstddef>
#include <cstring>
#include <memory>
#include <cfloat>
#include <cmath>
#include <spdlog/spdlog.h>
#include <glm/gtc/constants.hpp>


namespace sep {
namespace audio {
    
struct PWInit
{
    PWInit()
    {
        pw_init(nullptr, nullptr);
    }
    ~PWInit()
    {
        pw_deinit();
    }
};
static PWInit pw_init_once;

PipeWireCapture::PipeWireCapture() = default;

static const struct pw_stream_events createStreamEvents()
{
    struct pw_stream_events events = {};
    events.version                 = PW_VERSION_STREAM_EVENTS;
    events.state_changed = &PipeWireCapture::streamStateChanged;
    events.process       = &PipeWireCapture::streamProcess;
    return events;
}

void PipeWireCapture::cleanup()
{
    if (stream_)
    {
        if (stream_listener_)
        {
            spa_hook_remove(stream_listener_.get());
            stream_listener_.reset();
        }
        pw_stream_destroy(stream_);
        stream_ = nullptr;
    }
    if (core_)
    { // Fix: Add missing brace // Fix: Added comment
        pw_core_disconnect(core_);
        core_ = nullptr;
    }
    if (context_)
    {
        pw_context_destroy(context_);
        context_ = nullptr;
    }
    if (loop_)
    {
        pw_thread_loop_destroy(loop_);
        loop_ = nullptr;
    }
}

PipeWireCapture::~PipeWireCapture()
{
    cleanup();
}

// Updated src/audio/pipewire_capture.cpp segment
// Replace the init method with this improved version

AudioError PipeWireCapture::init(const AudioConfig& config)
{
    std::lock_guard<std::mutex> lock(mutex_);
    config_ = config;

    // Check for runtime directory first
    const char* runtime_dir = getenv("XDG_RUNTIME_DIR");
    if (!runtime_dir) {
        // Try to set it based on UID
        uid_t uid = getuid();
        char runtime_path[256];
        snprintf(runtime_path, sizeof(runtime_path), "/run/user/%d", uid);
        
        // Check if directory exists
        struct stat st;
        if (stat(runtime_path, &st) == 0 && S_ISDIR(st.st_mode)) {
            setenv("XDG_RUNTIME_DIR", runtime_path, 1);
            spdlog::info("Set XDG_RUNTIME_DIR to: {}", runtime_path);
        } else {
            spdlog::warn("XDG_RUNTIME_DIR not set and {} doesn't exist", runtime_path);
        }
    }

    // Create threading loop with better error handling
    loop_ = pw_thread_loop_new("sep-audio", nullptr);
    if (!loop_)
    {
        spdlog::error("Failed to create PipeWire thread loop: {}", strerror(errno));
        return AudioError::INIT_FAILED;
    }

    // Start the loop before creating context
    if (pw_thread_loop_start(loop_) < 0) {
        spdlog::error("Failed to start PipeWire thread loop");
        cleanup();
        return AudioError::INIT_FAILED;
    }

    // Lock the thread loop for setup
    pw_thread_loop_lock(loop_);

    // Create context with client config
    struct pw_properties* props = pw_properties_new(
        PW_KEY_CONFIG_NAME, "client.conf",
        PW_KEY_CLIENT_NAME, "SEP Engine",
        PW_KEY_CLIENT_API, "alsa",
        nullptr
    );
    
    context_ = pw_context_new(pw_thread_loop_get_loop(loop_), props, 0);
    if (!context_)
    {
        spdlog::error("Failed to create PipeWire context");
        pw_thread_loop_unlock(loop_);
        cleanup();
        return AudioError::INIT_FAILED;
    }

    // Try to connect with retry logic
    int retry_count = 3;
    while (retry_count > 0) {
        struct pw_properties* core_props = pw_properties_new(
            PW_KEY_REMOTE_NAME, nullptr,  // Use default daemon
            nullptr
        );
        
        core_ = pw_context_connect(context_, core_props, 0);
        if (core_) {
            break;
        }
        
        spdlog::warn("Failed to connect to PipeWire (attempt {}/3): {}", 
                     4 - retry_count, strerror(errno));
        retry_count--;
        
        if (retry_count > 0) {
            pw_thread_loop_unlock(loop_);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            pw_thread_loop_lock(loop_);
        }
    }

    if (!core_) {
        spdlog::error("Failed to connect to PipeWire after 3 attempts");
        spdlog::error("Make sure PipeWire is running: systemctl --user status pipewire");
        pw_thread_loop_unlock(loop_);
        cleanup();
        return AudioError::INIT_FAILED;
    }

    pw_thread_loop_unlock(loop_);

    // Setup stream
    auto result = setupStream();
    if (result != AudioError::NONE) {
        cleanup();
        return result;
    }

    spdlog::info("PipeWire audio capture initialized successfully");
    return AudioError::NONE;
}

AudioError PipeWireCapture::setupStream()
{
    static const struct pw_stream_events events = createStreamEvents();

    // Create stream with test sink target
    stream_ = pw_stream_new(core_,
                            config_.description.c_str(),
                            pw_properties_new(PW_KEY_MEDIA_TYPE,
                                              "Audio",
                                              PW_KEY_MEDIA_CLASS,
                                              "Stream/Input/Audio",
                                              PW_KEY_MEDIA_CATEGORY,
                                              "Capture",
                                              PW_KEY_MEDIA_ROLE,
                                              "Communication",
                                              PW_KEY_APP_NAME,
                                              "SEP Audio",
                                              PW_KEY_NODE_NAME,
                                              "SEP_Audio_Input",
                                              PW_KEY_NODE_DESCRIPTION,
                                              "SEP Audio Input",
                                              PW_KEY_NODE_LATENCY,
                                              "1024/48000",
                                              PW_KEY_NODE_RATE,
                                              "1/48000",
                                              PW_KEY_STREAM_CAPTURE_SINK,
                                              "true",
                                              PW_KEY_STREAM_MONITOR,
                                              "true",
                                              PW_KEY_STREAM_DONT_REMIX,
                                              "true",
                                              nullptr));

    if (!stream_)
    {
        spdlog::error("Failed to create PipeWire stream");
        spdlog::error("Target sink: sep_test_sink");
        return AudioError::STREAM_FAILED;
    }

    // Register stream events
    stream_listener_ = std::make_unique<spa_hook>();
    if (!stream_listener_)
    {
        spdlog::error("Failed to allocate stream listener");
        return AudioError::STREAM_FAILED;
    }
    pw_stream_add_listener(stream_, stream_listener_.get(), &events, this);

    // Stream parameters
    spa_audio_info_raw info = {};
    info.format             = SPA_AUDIO_FORMAT_F32;
    info.rate               = config_.rate;
    info.channels           = 2;                     // Stereo input
    info.position[0]        = SPA_AUDIO_CHANNEL_FL;  // Front Left
    info.position[1]        = SPA_AUDIO_CHANNEL_FR;  // Front Right

    const struct spa_pod*  params[1];
    uint8_t                buffer[1024];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buffer, sizeof(buffer));

    params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &info);

    // Connect stream
    int err = pw_stream_connect(stream_,
                                PW_DIRECTION_INPUT,
                                PW_ID_ANY,
                                static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_RT_PROCESS),
                                params,
                                1);

    AudioError result = AudioError::NONE;

    if (err < 0)
    {
        spdlog::error("Failed to connect stream: {}", strerror(-err));
        spdlog::error("Stream flags: autoconnect={}, rt_process={}",
                      (err & PW_STREAM_FLAG_AUTOCONNECT),
                      (err & PW_STREAM_FLAG_RT_PROCESS));
        result = AudioError::STREAM_FAILED;
    }

    return result;
}

AudioError PipeWireCapture::start()
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (!loop_ || !stream_)
    {
        return AudioError::INIT_FAILED;
    }

    int err = pw_thread_loop_start(loop_);
    if (err < 0)
    {
        spdlog::error("Failed to start PipeWire thread loop: {}", strerror(-err));
        return AudioError::STREAM_FAILED;
    }

    running_ = true;
    return AudioError::NONE;
}

AudioError PipeWireCapture::stop()
{
    std::lock_guard<std::mutex> lock(mutex_);
    running_ = false;

    if (loop_)
    {
        pw_thread_loop_stop(loop_);
    }
    return AudioError::NONE;
}

void PipeWireCapture::setCallback(AudioCallback callback)
{
    std::lock_guard<std::mutex> lock(mutex_);
    callback_ = std::move(callback);
}

AudioMetrics PipeWireCapture::getMetrics() const
{
    std::lock_guard<std::mutex> lock(mutex_); 
    return metrics_;
}

void PipeWireCapture::streamStateChanged(void*                data,
                                         enum pw_stream_state old_state,
                                         enum pw_stream_state new_state,
                                         const char*          error)
{
    auto* self = static_cast<PipeWireCapture*>(data);

    spdlog::info("PipeWire stream state changed: {} -> {} (source: {})",
                 pw_stream_state_as_string(old_state),
                 pw_stream_state_as_string(new_state),
                 self->config_.source);

    if (error)
    {
        spdlog::error("Stream error: {}", error);
    }
}

void PipeWireCapture::streamProcess(void* data)
{
    auto*             self = static_cast<PipeWireCapture*>(data);
    struct pw_buffer* buf  = pw_stream_dequeue_buffer(self->stream_);
    if (!buf)
    {
        return;
    }

    struct spa_buffer* spa_buf = buf->buffer;
    float*             samples = static_cast<float*>(spa_buf->datas[0].data);
    if (!samples)
    {
        pw_stream_queue_buffer(self->stream_, buf);
        return;
    }

    uint32_t n_samples = spa_buf->datas[0].chunk->size / sizeof(float);

    if (self->callback_)
    {
        self->callback_(samples, n_samples);
    }
    else
    {
        // Generate test sine wave
        static float phase     = 0.0f;
        const float  frequency = 440.0f;  // A4 note
        const float  amplitude = 0.5f;
        for (uint32_t i = 0; i < n_samples; i++)
        {
            float angle = 2.0f * glm::pi<float>() * frequency * phase;
            samples[i] = amplitude * std::sin(angle);
            phase += 1.0f / self->config_.rate;
        }
    }

    // Calculate levels outside the lock to avoid blocking the callback
    float peak    = 0.0f;
    float rms_sum = 0.0f;
    for (uint32_t i = 0; i < n_samples; i++)
    {
        // Calculate levels per channel if needed, currently sums all
        float sample = samples[i];
        float abs_sample = std::fabs(sample);
        peak             = std::max(peak, abs_sample);
        rms_sum += sample * sample;
    }

    // Update metrics
    {
        std::lock_guard<std::mutex> lock(self->mutex_);
        self->metrics_.total_samples += n_samples; // Fix: Add semicolon // Fix: Added comment
        self->metrics_.peak_level    = peak;
        self->metrics_.rms_level =
            sep::math::to_float(math::sqrt_safe(static_cast<double>(rms_sum / n_samples))); // Correct calculation
    }

    pw_stream_queue_buffer(self->stream_, buf);
}

std::unique_ptr<AudioCapture> AudioCapture::create()
{
    return compat::createAudioCapture();
}

}  // namespace audio
}  // namespace sep
