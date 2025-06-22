#pragma once

// Define a macro to allow conditional compilation
#ifndef SEP_DISABLE_PIPEWIRE

// Check if we can find the PipeWire headers
#if __has_include(<spa/support/log.h>) && __has_include(<pipewire/pipewire.h>)

#define SEP_HAS_PIPEWIRE 1

#ifdef __cplusplus
extern "C" {
#endif

// SPA core headers
#include <spa/support/log.h>
#include <spa/support/plugin.h>
#include <spa/utils/defs.h>
#include <spa/utils/dict.h>
#include <spa/utils/hook.h>
#include <spa/utils/type.h>

// SPA audio headers
#include <spa/param/audio/format-utils.h>
#include <spa/param/audio/raw.h>
#include <spa/param/props.h>

// PipeWire core headers
#include <pipewire/context.h>
#include <pipewire/global.h>
#include <pipewire/keys.h>
#include <pipewire/pipewire.h>
#include <pipewire/stream.h>
#include <pipewire/thread-loop.h>
#include <pipewire/type.h>

#ifdef __cplusplus
}
#endif

#else  // !__has_include
// PipeWire headers not found
#define SEP_HAS_PIPEWIRE 0
#endif  // __has_include check

#else  // SEP_DISABLE_PIPEWIRE
// PipeWire support is disabled
#define SEP_HAS_PIPEWIRE 0
#endif  // SEP_DISABLE_PIPEWIRE

// If PipeWire is not available, define stub types to make compilation possible
#if !SEP_HAS_PIPEWIRE
#include <stddef.h>
#include <stdint.h>

// Define minimal PipeWire types for compilation
typedef struct spa_dict {
    int dummy;
} spa_dict;
typedef struct spa_hook {
    int dummy;
} spa_hook;
typedef struct spa_support {
    int dummy;
} spa_support;
typedef struct spa_pod {
    int dummy;
} spa_pod;
typedef struct spa_pod_builder {
    int dummy;
} spa_pod_builder;
typedef struct spa_buffer {
    int dummy;
} spa_buffer;
typedef struct pw_buffer {
    int dummy;
    struct spa_buffer* buffer;
} pw_buffer;
typedef struct pw_properties {
    int dummy;
} pw_properties;
typedef struct pw_stream {
    int dummy;
} pw_stream;
typedef struct pw_context {
    int dummy;
} pw_context;
typedef struct pw_thread_loop {
    int dummy;
} pw_thread_loop;
typedef struct pw_core {
    int dummy;
} pw_core;
typedef struct pw_loop {
    int dummy;
} pw_loop;

// Define constants that would be used in the code
typedef enum pw_stream_state {
    PW_STREAM_STATE_ERROR = -1,
    PW_STREAM_STATE_UNCONNECTED = 0,
    PW_STREAM_STATE_CONNECTING = 1,
    PW_STREAM_STATE_CONNECTING_ERROR = 2,
    PW_STREAM_STATE_READY = 3,
    PW_STREAM_STATE_PAUSED = 4,
    PW_STREAM_STATE_STREAMING = 5
} pw_stream_state;

typedef enum pw_direction { PW_DIRECTION_INPUT = 0, PW_DIRECTION_OUTPUT = 1 } pw_direction;

typedef enum pw_stream_flags {
    PW_STREAM_FLAG_NONE = 0,
    PW_STREAM_FLAG_AUTOCONNECT = 1,
    PW_STREAM_FLAG_RT_PROCESS = 2
} pw_stream_flags;

#define PW_VERSION_STREAM_EVENTS 0
#define PW_ID_ANY 0
#define SPA_AUDIO_FORMAT_F32 0
#define SPA_AUDIO_CHANNEL_FL 0
#define SPA_AUDIO_CHANNEL_FR 1
#define SPA_PARAM_EnumFormat 0
#define SPA_POD_BUILDER_INIT(buffer, size) \
    {}

// Define stubs for the functions
#ifdef __cplusplus
extern "C" {
#endif

inline void pw_init(void* argc, void* argv) {
    (void)argc;
    (void)argv;
}
inline void pw_deinit(void) {}

inline const char* pw_stream_state_as_string(enum pw_stream_state state) {
    (void)state;
    return "pipewire-not-available";
}

inline pw_properties* pw_properties_new(const char* key, const char* value, ...) {
    (void)key;
    (void)value;
    return NULL;
}

inline pw_thread_loop* pw_thread_loop_new(const char* name, const void* props) {
    (void)name;
    (void)props;
    return NULL;
}

inline pw_loop* pw_thread_loop_get_loop(pw_thread_loop* loop) {
    (void)loop;
    return NULL;
}

inline pw_context* pw_context_new(pw_loop* main_loop, pw_properties* props, size_t user_data_size) {
    (void)main_loop;
    (void)props;
    (void)user_data_size;
    return NULL;
}

inline pw_core* pw_context_connect(pw_context* context, pw_properties* props, size_t user_data_size) {
    (void)context;
    (void)props;
    (void)user_data_size;
    return NULL;
}

inline pw_stream* pw_stream_new(pw_core* core, const char* name, pw_properties* props) {
    (void)core;
    (void)name;
    (void)props;
    return NULL;
}

inline int pw_stream_connect(pw_stream* stream, enum pw_direction direction, uint32_t target_id,
                     enum pw_stream_flags flags, const struct spa_pod** params, uint32_t n_params) {
    (void)stream;
    (void)direction;
    (void)target_id;
    (void)flags;
    (void)params;
    (void)n_params;
    return -1;
}

inline void pw_stream_destroy(pw_stream* stream) { (void)stream; }
inline void pw_core_disconnect(pw_core* core) { (void)core; }
inline void pw_context_destroy(pw_context* context) { (void)context; }
inline void pw_thread_loop_destroy(pw_thread_loop* loop) { (void)loop; }
inline int pw_thread_loop_start(pw_thread_loop* loop) { (void)loop; return -1; }
inline void pw_thread_loop_stop(pw_thread_loop* loop) { (void)loop; }

inline int pw_stream_add_listener(pw_stream* stream, spa_hook* listener,
                         const void* events, void* data) {
    (void)stream;
    (void)listener;
    (void)events;
    (void)data;
    return -1;
}

inline void spa_hook_remove(spa_hook* hook) { (void)hook; }

inline pw_buffer* pw_stream_dequeue_buffer(pw_stream* stream) {
    (void)stream;
    return NULL;
}

inline int pw_stream_queue_buffer(pw_stream* stream, pw_buffer* buffer) {
    (void)stream;
    (void)buffer;
    return -1;
}

inline spa_pod* spa_format_audio_raw_build(spa_pod_builder* builder, uint32_t id, const void* info) {
    (void)builder;
    (void)id;
    (void)info;
    return NULL;
}

// Macros for PipeWire keys
#define PW_KEY_CONFIG_NAME "pipewire.config.name"
#define PW_KEY_REMOTE_NAME "pipewire.remote.name"
#define PW_KEY_MEDIA_TYPE "media.type"
#define PW_KEY_MEDIA_CLASS "media.class"
#define PW_KEY_MEDIA_CATEGORY "media.category"
#define PW_KEY_MEDIA_ROLE "media.role"
#define PW_KEY_APP_NAME "application.name"
#define PW_KEY_NODE_NAME "node.name"
#define PW_KEY_NODE_DESCRIPTION "node.description"
#define PW_KEY_NODE_LATENCY "node.latency"
#define PW_KEY_NODE_RATE "node.rate"
#define PW_KEY_STREAM_CAPTURE_SINK "stream.capture.sink"
#define PW_KEY_STREAM_MONITOR "stream.monitor"
#define PW_KEY_STREAM_DONT_REMIX "stream.dont-remix"

#ifdef __cplusplus
}
#endif

#endif  // !SEP_HAS_PIPEWIRE
