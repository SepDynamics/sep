#ifndef SEP_API_BRIDGE_H
#define SEP_API_BRIDGE_H

#include <cstddef> // For size_t

// Cross-platform API export macro
#if defined(_WIN32)
#ifdef BUILDING_SEP_BRIDGE
#define SEP_API __declspec(dllexport)
#else
#define SEP_API __declspec(dllimport)
#endif
#else
#define SEP_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

SEP_API int sep_bridge_init(void);
SEP_API int sep_bridge_cleanup(void);
SEP_API int sep_process_context(const char *context_json, const char *layer,
                                char *result_buffer, size_t buffer_size);
SEP_API int sep_bridge_get_last_error(char *buffer, size_t buffer_size);
SEP_API size_t sep_get_required_buffer_size(void);
SEP_API int sep_bridge_set_config(const char *key, const char *value);
SEP_API int sep_bridge_get_config(const char *key, char *buffer,
                                  size_t buffer_size);
SEP_API int
sep_bridge_register_callback(const char *event_type,
                             void (*callback)(const char *event_data));

#ifdef __cplusplus
}
#endif

#endif // SEP_API_BRIDGE_H
