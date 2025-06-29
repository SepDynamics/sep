#pragma once

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

