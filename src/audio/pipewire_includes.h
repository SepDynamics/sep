#pragma once

#ifdef SEP_HAS_AUDIO

#ifdef __cplusplus
extern "C" {
#endif

// System headers
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// PipeWire core headers must come first
#include <pipewire/pipewire.h>
#include <pipewire/context.h>
#include <pipewire/global.h>
#include <pipewire/keys.h>
#include <pipewire/stream.h>
#include <pipewire/thread-loop.h>
#include <pipewire/type.h>

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

#ifdef __cplusplus
}
#endif

// Make PipeWire types available in C++ code
using pw_stream_events = struct pw_stream_events;
using pw_stream = struct pw_stream;
using pw_context = struct pw_context;
using pw_core = struct pw_core;
using pw_thread_loop = struct pw_thread_loop;
using spa_hook = struct spa_hook;
using spa_pod_builder = struct spa_pod_builder;

#endif // SEP_HAS_AUDIO

