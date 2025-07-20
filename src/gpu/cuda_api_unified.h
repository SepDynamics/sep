/*
 * CUDA API Unified Interface
 * 
 * This header provides a unified interface to CUDA functionality that works
 * in both CUDA and non-CUDA environments through conditional compilation.
 */

#ifndef SEP_CUDA_API_UNIFIED_H
#define SEP_CUDA_API_UNIFIED_H

#include <stddef.h>  /* For size_t */

#ifdef __cplusplus
#include <string>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Error codes */
#define SEP_CUDA_SUCCESS                      0
#define SEP_CUDA_ERROR_INVALID_VALUE          1
#define SEP_CUDA_ERROR_OUT_OF_MEMORY          2
#define SEP_CUDA_ERROR_NOT_INITIALIZED        3
#define SEP_CUDA_ERROR_INVALID_DEVICE         4
#define SEP_CUDA_ERROR_NO_DEVICE              5
#define SEP_CUDA_ERROR_LAUNCH_FAILED          6
#define SEP_CUDA_ERROR_LAUNCH_TIMEOUT         7
#define SEP_CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES 8
#define SEP_CUDA_ERROR_SYNCHRONIZATION_ERROR  9
#define SEP_CUDA_ERROR_NOT_ENOUGH_MEMORY      10
#define SEP_CUDA_ERROR_INVALID_CONFIGURATION  11

/* Memory copy kinds */
typedef enum {
    SEP_CUDA_MEMCPY_HOST_TO_HOST = 0,
    SEP_CUDA_MEMCPY_HOST_TO_DEVICE = 1,
    SEP_CUDA_MEMCPY_DEVICE_TO_HOST = 2,
    SEP_CUDA_MEMCPY_DEVICE_TO_DEVICE = 3,
    SEP_CUDA_MEMCPY_DEFAULT = 4
} SEP_CUDA_MEMCPY_KIND;

/* Error structure */
typedef struct {
    int code;
    char message[256];
} SEP_CUDA_ERROR;

/* Device properties structure */
typedef struct {
    char name[256];
    int major;
    int minor;
    size_t total_global_mem;
    size_t shared_mem_per_block;
    int max_threads_per_block;
    int multi_processor_count;
    int warp_size;
    int max_threads_dim[3];
    int max_grid_size[3];
    int unified_addressing;
} SEP_CUDA_DEVICE_PROPS;

/* Helper function to create an error with code and message */
SEP_CUDA_ERROR SEP_CUDA_CreateError(int code, const char* message);

/* Check if CUDA is available at runtime */
int SEP_CUDA_IsAvailable(void);

/* Initialization */
SEP_CUDA_ERROR SEP_CUDA_Init(void);

/* Memory allocation */
SEP_CUDA_ERROR SEP_CUDA_Malloc(void** ptr, size_t size);

/* Memory deallocation */
SEP_CUDA_ERROR SEP_CUDA_Free(void* ptr);

/* Memory copy */
SEP_CUDA_ERROR SEP_CUDA_Memcpy(void* dst, const void* src, size_t count, 
                             SEP_CUDA_MEMCPY_KIND kind);

/* Get device count */
SEP_CUDA_ERROR SEP_CUDA_GetDeviceCount(int* count);

/* Get device properties */
SEP_CUDA_ERROR SEP_CUDA_GetDeviceProperties(SEP_CUDA_DEVICE_PROPS* prop, int device);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* Error string function */
shim::string SEP_CUDA_ErrorString(SEP_CUDA_ERROR error);
#endif

#endif /* SEP_CUDA_API_UNIFIED_H */
