#include "audio/pipewire_capture.h"
#include "audio/types.h"
#include "audio/capture.h"
#include <string.h> // For strerror, strncmp, strstr
#include <time.h>   // For CLOCK_MONOTONIC, nanosleep
#include "audio/config.h"

#ifdef SEP_HAS_AUDIO

// PipeWire headers
extern "C" {
#include <pipewire/pipewire.h>
#include <pipewire/context.h>
#include <pipewire/global.h>
#include <pipewire/keys.h>
#include <pipewire/stream.h>
#include <pipewire/thread-loop.h>
#include <pipewire/type.h>

// SPA headers
#include <spa/param/audio/format-utils.h>
#include <spa/param/props.h>
#include <spa/support/log.h>
#include <spa/utils/hook.h>
#include <spa/utils/type.h>
}

#include <spdlog/spdlog.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glm/gtc/constants.hpp>
#include <cstdio>

#include <sys/stat.h> // For stat
#include <unistd.h>   // For getuid
#include <cstdio>     // For snprintf
#include <cstdlib>    // For getenv, setenv, system, popen, pclose
#include <thread>     // For std::this_thread::sleep_for

// Additional project headers
#include "compat/math_common.h"

// Standard library headers
#include <new>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <memory>
#include <cfloat>
#include <cmath>


namespace sep {
namespace audio {

// Initialize PipeWire before class implementation
namespace sep {
namespace audio {

namespace {
struct PWInit {
    bool ok{false};
    PWInit() {
        pw_init(nullptr, nullptr);
        ok = true;
    }
    ~PWInit() {
        if (ok) {
            pw_deinit();
        }
    }
} pw_init_once;
}

PipeWireCapture::PipeWireCapture() : stream_events_{} {}

static const pw_stream_events createStreamEvents()
{
    pw_stream_events events = {};
    events.version = PW_VERSION_STREAM_EVENTS;
    events.state_changed = &sep::audio::PipeWireCapture::streamStateChanged;
    events.process = &sep::audio::PipeWireCapture::streamProcess;
    return events;
}

void sep::audio::PipeWireCapture::streamProcess(void* data)
{
    auto* self = static_cast<PipeWireCapture*>(data);
    pw_buffer* buf = pw_stream_dequeue_buffer(self->stream_);
    if (!buf) {
        return;
    }

    spa_buffer* spa_buf = buf->buffer;
    float* samples = static_cast<float*>(spa_buf->datas[0].data);
    if (!samples) {
        pw_stream_queue_buffer(self->stream_, buf);
        return;
    }

    uint32_t n_samples = spa_buf->datas[0].chunk->size / sizeof(float);
    float peak = 0.0f;
    float rms_sum = 0.0f;

    {
        std::lock_guard<std::mutex> lock(self->mutex_);
        if (self->running_ && self->callback_) {
            self->callback_(samples, n_samples);
        }

        // Update metrics
        for (uint32_t i = 0; i < n_samples; i++) {
            float abs_sample = std::fabs(samples[i]);
            peak = std::max(peak, abs_sample);
            rms_sum += abs_sample * abs_sample;
        }

        if (self->running_) {
            self->metrics_.total_samples += n_samples;
            self->metrics_.peak_level = peak;
            self->metrics_.rms_level =
                sep::math::to_float(math::sqrt_safe(static_cast<double>(rms_sum / n_samples)));
        }
    }

    pw_stream_queue_buffer(self->stream_, buf);
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
    { 
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

    if (!pw_init_once.ok)
    {
        spdlog::error("PipeWire library failed to initialize");
        return AudioError::INIT_FAILED;
    }

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
        spdlog::error("Ensure libpipewire is installed and accessible");
        return AudioError::INIT_FAILED;
    }

    // Start the loop before creating context
    int start_err = pw_thread_loop_start(loop_);
    if (start_err < 0)
    {
        spdlog::error("Failed to start PipeWire thread loop: {}", spa_strerror(start_err));
        if (start_err == -EEXIST)
        {
            spdlog::error("Thread loop already running");
        }
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

    // Check and start required PipeWire services
    const char* services[] = {
        "pipewire.socket",          // Base PipeWire socket
        "pipewire.service",         // PipeWire daemon
        "wireplumber.service",      // Session manager
        "pipewire-pulse.service"    // PulseAudio compatibility
    };

    // First check if socket is available
    std::string check_socket = "test -e \"$XDG_RUNTIME_DIR/pipewire-0\"";
    if (system(check_socket.c_str()) != 0) {
        spdlog::error("PipeWire socket not found in XDG_RUNTIME_DIR");
        spdlog::info("Creating PipeWire runtime directory...");
        
        std::string runtime_dir = std::getenv("XDG_RUNTIME_DIR") ? std::getenv("XDG_RUNTIME_DIR") : "/run/user/1000";
        std::string mkdir_cmd = "mkdir -p \"" + runtime_dir + "/pipewire-0\"";
        if (system(mkdir_cmd.c_str()) != 0) {
            spdlog::error("Failed to create PipeWire runtime directory");
            pw_thread_loop_unlock(loop_);
            cleanup();
            return AudioError::INIT_FAILED;
        }
    }

    for (const char* service : services) {
        // Check if service is enabled
        std::string check_enabled = "systemctl --user is-enabled " + std::string(service);
        if (system(check_enabled.c_str()) != 0) {
            spdlog::info("Enabling {}...", service);
            std::string enable_cmd = "systemctl --user enable " + std::string(service);
            system(enable_cmd.c_str());
        }

        // Check if service is running
        std::string check_cmd = "systemctl --user is-active " + std::string(service);
        FILE* fp = popen(check_cmd.c_str(), "r");
        if (!fp) {
            spdlog::error("Failed to check service status: {}", service);
            continue; // Try next service
        }

        char service_status[128];
        bool service_running = false;
        if (fgets(service_status, sizeof(service_status), fp) != nullptr) {
            service_running = (strncmp(service_status, "active", 6) == 0);
        }
        pclose(fp);

        if (!service_running) {
            spdlog::info("Starting {}...", service);
            
            // Try starting the service
            std::string start_cmd = "systemctl --user start " + std::string(service);
            int start_result = system(start_cmd.c_str());
            if (start_result != 0) {
                spdlog::error("Failed to start {} (error code: {})", service, start_result);
                // Check common issues
                std::string status_cmd = "systemctl --user status " + std::string(service);
                FILE* status_fp = popen(status_cmd.c_str(), "r");
                if (status_fp) {
                    char status_buf[1024];
                    while (fgets(status_buf, sizeof(status_buf), status_fp)) {
                        if (strstr(status_buf, "Permission denied") ||
                            strstr(status_buf, "Failed to connect") ||
                            strstr(status_buf, "Unit not found")) {
                            spdlog::error("Service error: {}", status_buf);
                        }
                    }
                    pclose(status_fp);
                }
                continue; // Try next service
            }
            
            // Wait for service to be ready
            int retries = 20; // Increased retries
            while (retries > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                
                FILE* check_fp = popen(check_cmd.c_str(), "r");
                if (check_fp) {
                    if (fgets(service_status, sizeof(service_status), check_fp) != nullptr) {
                        if (strncmp(service_status, "active", 6) == 0) {
                            service_running = true;
                            pclose(check_fp);
                            break;
                        }
                    }
                    pclose(check_fp);
                }
                retries--;
            }
            
            if (!service_running) {
                spdlog::error("Timeout waiting for {} to start", service);
                continue; // Try next service
            }
            
            spdlog::info("{} started successfully", service);
        }
    }

    // Final check if core services are running
    if (system("systemctl --user is-active pipewire.service") != 0 ||
        system("systemctl --user is-active wireplumber.service") != 0) {
        spdlog::error("Critical PipeWire services failed to start");
        pw_thread_loop_unlock(loop_);
        cleanup();
        return AudioError::INIT_FAILED;
    }

    // Wait for services to be fully initialized
    std::this_thread::sleep_for(std::chrono::seconds(3));

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
        spdlog::error("Please ensure PipeWire is installed and running:");
        spdlog::error("1. Check if installed: which pipewire");
        spdlog::error("2. Start service: systemctl --user start pipewire.service");
        spdlog::error("3. Enable service: systemctl --user enable pipewire.service");
        pw_thread_loop_unlock(loop_);
        cleanup();
        return AudioError::INIT_FAILED;
    }

    // Setup stream while holding the lock
    static const struct pw_stream_events events = createStreamEvents();

    // Create stream for capturing audio
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
        return AudioError::STREAM_FAILED;
    }

    // Register stream events
    stream_listener_ = std::make_unique<spa_hook>();
    if (!stream_listener_)
    {
        spdlog::error("Failed to allocate stream listener");
        pw_thread_loop_unlock(loop_);
        cleanup();
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
                               static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT |
                                                          PW_STREAM_FLAG_RT_PROCESS),
                               params,
                               1);

    if (err < 0)
    {
        spdlog::error("Failed to connect stream: {}", spa_strerror(err));
        spdlog::error("Source: {} rate: {}", config_.source, config_.rate);
        if (err == -ENOENT)
        {
            spdlog::error("Requested capture device not found");
        }
        pw_thread_loop_unlock(loop_);
        cleanup();
        return AudioError::STREAM_FAILED;
    }

    pw_thread_loop_unlock(loop_);
    spdlog::info("PipeWire audio capture initialized successfully");
    return AudioError::NONE;
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
        spdlog::error("Failed to start PipeWire thread loop: {}", spa_strerror(err));
        if (err == -EEXIST)
        {
            spdlog::warn("Thread loop was already running");
        }
        else if (err == -EACCES || err == -EPERM)
        {
            spdlog::error("Insufficient permissions to start real-time thread");
        }
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

void PipeWireCapture::streamStateChanged(void* data,
                                       enum pw_stream_state old_state,
                                       enum pw_stream_state new_state,
                                       const char* error)
{
    auto* self = static_cast<PipeWireCapture*>(data);

    spdlog::info("PipeWire stream state changed: {} -> {} (source: {})",
                 pw_stream_state_as_string(old_state),
                 pw_stream_state_as_string(new_state),
                 self->config_.source);

    if (error) {
        spdlog::error("Stream error: {}", error);
        return;
    }

    std::lock_guard<std::mutex> lock(self->mutex_);
    
    switch (new_state) {
        case PW_STREAM_STATE_ERROR:
            self->running_ = false;
            spdlog::error("Stream entered error state");
            break;
            
        case PW_STREAM_STATE_PAUSED:
            // Ensure we start streaming when ready
            if (old_state == PW_STREAM_STATE_CONNECTING) {
                int err = pw_stream_set_active(self->stream_, true);
                if (err < 0) {
                    spdlog::error("Failed to activate stream: {}", spa_strerror(err));
                    self->running_ = false;
                }
            }
            break;
            
        case PW_STREAM_STATE_STREAMING:
            self->running_ = true;
            spdlog::info("Stream is now active");
            break;
            
        case PW_STREAM_STATE_UNCONNECTED:
            self->running_ = false;
            break;
            
        default:
            break;
    }
}

void PipeWireCapture::streamProcess(void* data)
{
    auto* self = static_cast<PipeWireCapture*>(data);
    
    // Early exit if not running or wrong state
    auto stream_state = pw_stream_get_state(self->stream_, nullptr);
    bool is_running = self->running_.load();
    if (!is_running || stream_state != PW_STREAM_STATE_STREAMING) {
        spdlog::debug("Stream not ready: running={}, state={}",
                     is_running ? "true" : "false",
                     pw_stream_state_as_string(stream_state));
        return;
    }

    // Take a local copy of the callback to prevent race conditions
    AudioCallback callback;
    {
        std::lock_guard<std::mutex> lock(self->mutex_);
        callback = self->callback_;
    }

    struct pw_buffer* buf = pw_stream_dequeue_buffer(self->stream_);
    if (!buf) {
        spdlog::debug("No buffer available");
        return;
    }

    struct spa_buffer* spa_buf = buf->buffer;
    float* samples = static_cast<float*>(spa_buf->datas[0].data);
    if (!samples) {
        spdlog::warn("Received empty buffer");
        pw_stream_queue_buffer(self->stream_, buf);
        return;
    }

    uint32_t n_samples = spa_buf->datas[0].chunk->size / sizeof(float);
    
    static uint64_t total_processed = 0;
    total_processed += n_samples;
    
    if (total_processed % (self->config_.rate * 10) == 0) {  // Log every 10 seconds
        spdlog::info("Audio processing stats: {} samples processed ({:.1f} seconds)",
                    total_processed,
                    static_cast<float>(total_processed) / self->config_.rate);
    }

    // Process audio data
    if (callback) {
        callback(samples, n_samples);
    } else {
        // Generate test sine wave with thread-safe phase tracking
        static std::atomic<float> phase{0.0f};
        const float frequency = 440.0f;  // A4 note
        const float amplitude = 0.5f;
        
        float current_phase = phase.load(std::memory_order_relaxed);
        for (uint32_t i = 0; i < n_samples; i++) {
            float angle = 2.0f * glm::pi<float>() * frequency * current_phase;
            samples[i] = amplitude * std::sin(angle);
            current_phase += 1.0f / self->config_.rate;
        }
        phase.store(current_phase, std::memory_order_relaxed);
    }

    // Calculate audio levels
    float peak = 0.0f;
    float rms_sum = 0.0f;
    for (uint32_t i = 0; i < n_samples; i++) {
        float sample = samples[i];
        float abs_sample = std::fabs(sample);
        peak = std::max(peak, abs_sample);
        rms_sum += sample * sample;
    }

    // Update metrics atomically
    {
        std::lock_guard<std::mutex> lock(self->mutex_);
        if (self->running_) {  // Check again under lock
            self->metrics_.total_samples += n_samples;
            self->metrics_.peak_level = peak;
            self->metrics_.rms_level =
                sep::math::to_float(math::sqrt_safe(static_cast<double>(rms_sum / n_samples)));
            
            if (peak > 0.8f) {  // Log high audio levels
                spdlog::info("High audio level detected: {:.2f}", peak);
            }
        }
    }

    pw_stream_queue_buffer(self->stream_, buf);
}

std::unique_ptr<AudioCapture> AudioCapture::create()
{
    return std::make_unique<PipeWireCapture>();
}

} // namespace audio
} // namespace sep

#endif // SEP_HAS_AUDIO
