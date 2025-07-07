#pragma once

#ifdef SEP_HAS_AUDIO

#include "audio/types.h"
#include "audio/capture.h"
#include "audio/pipewire_includes.h"

// Forward declarations for internal use
struct spa_hook;
struct spa_pod_builder;
#include <atomic>
#include <memory>
#include <mutex>

namespace sep {
namespace audio {

class SEP_AUDIO_API PipeWireCapture final : public AudioCapture {
public:
    PipeWireCapture();
    ~PipeWireCapture() override;

    AudioError init(const AudioConfig& config) override;
    AudioError start() override;
    AudioError stop() override;
    void setCallback(AudioCallback callback) override;
    AudioMetrics getMetrics() const override;

    // PipeWire callbacks (public since used by PipeWire)
    static void streamStateChanged(void* data, enum pw_stream_state old_state,
                                 enum pw_stream_state state, const char* error);
    static void streamProcess(void* data);

private:
    void cleanup();
    AudioError setupStream();

    // PipeWire state
    pw_thread_loop* loop_{nullptr};
    pw_context* context_{nullptr};
    pw_core* core_{nullptr};
    pw_stream* stream_{nullptr};
    std::unique_ptr<spa_hook> stream_listener_;
    spa_pod_builder pod_builder_{};
    pw_stream_events stream_events_{};

    // Audio processing state
    AudioConfig config_;
    AudioCallback callback_{nullptr};
    std::atomic<bool> running_{false};
    mutable std::mutex mutex_;
    AudioMetrics metrics_{};
};

} // namespace audio
} // namespace sep

#endif // SEP_HAS_AUDIO
