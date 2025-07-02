#pragma once

/**
 * @brief PipeWire-based audio capture implementation.
 *
 * Wraps the PipeWire API to provide AudioCapture functionality for the
 * audio pipeline.
 */

#include <atomic>
#include <mutex>

#include "audio/pipewire_includes.h"
#include "audio/capture.h"

#ifndef SEP_HAS_PIPEWIRE
#  if __has_include(<pipewire/pipewire.h>)
#    define SEP_HAS_PIPEWIRE 1
#  else
#    define SEP_HAS_PIPEWIRE 0
#  endif
#endif

#if SEP_HAS_PIPEWIRE
#  include <pipewire/stream.h>
#endif

// Forward declarations to avoid exposing PipeWire types in header
struct pw_context;
struct pw_core;
struct pw_stream;
struct pw_thread_loop;
struct spa_hook;

namespace sep {
namespace audio {

#if SEP_HAS_PIPEWIRE

class PipeWireCapture : public AudioCapture {
  public:
    PipeWireCapture();
    ~PipeWireCapture() override;

    // AudioCapture interface implementation
    AudioError init(const AudioConfig &config) override;
    AudioError start() override;
    AudioError stop() override;
    void setCallback(AudioCallback callback) override;
    AudioMetrics getMetrics() const override;

    // PipeWire callbacks (public since used by PipeWire)
    static void streamStateChanged(void *data, enum pw_stream_state old_state,
                                  enum pw_stream_state new_state,
                                  const char *error);
    static void streamProcess(void *data);

  private:
    // PipeWire state
    pw_thread_loop *loop_{nullptr};
    pw_context *context_{nullptr};
    pw_core *core_{nullptr};
    pw_stream *stream_{nullptr};
    std::unique_ptr<spa_hook> stream_listener_;

    // Audio processing state
    AudioConfig config_;
    AudioCallback callback_;
    mutable std::mutex mutex_;
    std::atomic<bool> running_{false};
    AudioMetrics metrics_;

    // Internal methods
    void cleanup();
    AudioError setupStream();
};
#else

class PipeWireCapture : public AudioCapture {
  public:
    AudioError init(const AudioConfig&) override { return AudioError::INIT_FAILED; }
    AudioError start() override { return AudioError::INIT_FAILED; }
    AudioError stop() override { return AudioError::NONE; }
    void setCallback(AudioCallback) override {}
    AudioMetrics getMetrics() const override { return {}; }
};

#endif

} // namespace audio
} // namespace sep
