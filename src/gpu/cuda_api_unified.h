#pragma once

#ifdef __cplusplus
#include "../compat/cuda_types_fwd.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CUDA API Unified Interface 
 * This provides a single entry point for all CUDA operations
 * to prevent circular dependencies and duplicate declarations
 */

/* Device Management */
int cuda_get_device_count(void);
int cuda_get_current_device(void);
int cuda_set_device(int device_id);
const char* cuda_get_device_name(int device_id);

/* Memory Management */
void* cuda_allocate_device_memory(size_t size);
int cuda_free_device_memory(void* ptr);
void* cuda_allocate_unified_memory(size_t size);
int cuda_free_unified_memory(void* ptr);

/* Memory Copy Operations */
int cuda_copy_host_to_device(void* dst, const void* src, size_t size);
int cuda_copy_device_to_host(void* dst, const void* src, size_t size);
int cuda_copy_device_to_device(void* dst, const void* src, size_t size);

/* Stream Management */
cudaStream_t cuda_create_stream(unsigned int flags);
int cuda_destroy_stream(cudaStream_t stream);
int cuda_synchronize_stream(cudaStream_t stream);

/* Error Handling */
cudaError_t cuda_get_last_error(void);
const char* cuda_get_error_string(cudaError_t error);

/* Unified API Initialization */
int cuda_initialize(void);
int cuda_is_available(void);

#ifdef __cplusplus
}
#endif

#endif /* __cplusplus */
