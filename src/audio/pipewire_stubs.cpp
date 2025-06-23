#include "audio/pipewire_includes.h"

#if !SEP_HAS_PIPEWIRE
extern "C" {
void pw_init(void* argc, void* argv) { (void)argc; (void)argv; }
void pw_deinit(void) {}

const char* pw_stream_state_as_string(enum pw_stream_state state) {
    (void)state;
    return "pipewire-not-available";
}

pw_properties* pw_properties_new(const char* key, const char* value, ...) {
    (void)key; (void)value; return nullptr;
}

pw_thread_loop* pw_thread_loop_new(const char* name, const void* props) {
    (void)name; (void)props; return nullptr;
}

pw_loop* pw_thread_loop_get_loop(pw_thread_loop* loop) {
    (void)loop; return nullptr;
}

pw_context* pw_context_new(pw_loop* main_loop, pw_properties* props, size_t user_data_size) {
    (void)main_loop; (void)props; (void)user_data_size; return nullptr;
}

pw_core* pw_context_connect(pw_context* context, pw_properties* props, size_t user_data_size) {
    (void)context; (void)props; (void)user_data_size; return nullptr;
}

pw_stream* pw_stream_new(pw_core* core, const char* name, pw_properties* props) {
    (void)core; (void)name; (void)props; return nullptr;
}

int pw_stream_connect(pw_stream* stream, enum pw_direction direction, uint32_t target_id,
                      enum pw_stream_flags flags, const struct spa_pod** params, uint32_t n_params) {
    (void)stream; (void)direction; (void)target_id; (void)flags; (void)params; (void)n_params; return -1;
}

void pw_stream_destroy(pw_stream* stream) { (void)stream; }
void pw_core_disconnect(pw_core* core) { (void)core; }
void pw_context_destroy(pw_context* context) { (void)context; }
void pw_thread_loop_destroy(pw_thread_loop* loop) { (void)loop; }
int  pw_thread_loop_start(pw_thread_loop* loop) { (void)loop; return -1; }
void pw_thread_loop_stop(pw_thread_loop* loop) { (void)loop; }

int pw_stream_add_listener(pw_stream* stream, spa_hook* listener,
                           const void* events, void* data) {
    (void)stream; (void)listener; (void)events; (void)data; return -1;
}

void spa_hook_remove(spa_hook* hook) { (void)hook; }

pw_buffer* pw_stream_dequeue_buffer(pw_stream* stream) {
    (void)stream; return nullptr;
}

int pw_stream_queue_buffer(pw_stream* stream, pw_buffer* buffer) {
    (void)stream; (void)buffer; return -1;
}

spa_pod* spa_format_audio_raw_build(spa_pod_builder* builder, uint32_t id, const void* info) {
    (void)builder; (void)id; (void)info; return nullptr;
}
} // extern "C"
#endif // !SEP_HAS_PIPEWIRE
