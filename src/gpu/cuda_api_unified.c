/*
 * CUDA API Unified Implementation
 * 
 * This file provides the implementation for the unified CUDA API defined in 
 * cuda_api_unified.h. It handles both CUDA and non-CUDA environments through
 * conditional compilation.
 */

#include "cuda_api_unified.h"
#include "../util/error_handling.h"
#include <string.h>  /* For memcpy, memset, etc. */

/* Macro for unused parameters to prevent compiler warnings */
#define SEP_CUDA_UNUSED(x) (void)(x)

/* Determine if CUDA is available at compile time */
#if defined(SEP_ENGINE_HAS_CUDA) && SEP_ENGINE_HAS_CUDA
  #include <cuda_runtime.h>
  #define SEP_CUDA_AVAILABLE 1
#else
  #define SEP_CUDA_AVAILABLE 0
#endif

/* Static buffer for error strings */
static char sep_cuda_error_string_buffer[256];

/* Internal function to convert SEP_CUDA_MEMCPY_KIND to CUDA's cudaMemcpyKind */
#if SEP_CUDA_AVAILABLE
static cudaMemcpyKind convert_memcpy_kind(SEP_CUDA_MEMCPY_KIND kind) {
    switch (kind) {
        case SEP_CUDA_MEMCPY_HOST_TO_HOST:
            return cudaMemcpyHostToHost;
        case SEP_CUDA_MEMCPY_HOST_TO_DEVICE:
            return cudaMemcpyHostToDevice;
        case SEP_CUDA_MEMCPY_DEVICE_TO_HOST:
            return cudaMemcpyDeviceToHost;
        case SEP_CUDA_MEMCPY_DEVICE_TO_DEVICE:
            return cudaMemcpyDeviceToDevice;
        case SEP_CUDA_MEMCPY_DEFAULT:
        default:
            return cudaMemcpyDefault;
    }
}
#endif

/* Helper function to log CUDA errors */
static void log_cuda_error(SEP_ERROR_LEVEL level, int error_code, 
                          const char* message, const char* file, 
                          int line, const char* function) {
    /* Create an error context manually instead of using the macro */
    SEP_ERROR_CONTEXT ctx;
    ctx.file = file;
    ctx.line = line;
    ctx.function = function;
    ctx.level = level;
    ctx.category = SEP_ERROR_CATEGORY_CUDA;
    ctx.error_code = error_code;
    ctx.message = message;
    
    /* Report the error */
    sep_error_report(ctx);
}

/* Helper function to create an error with code and message */
SEP_CUDA_ERROR SEP_CUDA_CreateError(int code, const char* message) {
    SEP_CUDA_ERROR error;
    error.code = code;
    
    if (message) {
        size_t len = strlen(message);
        if (len >= sizeof(error.message)) {
            len = sizeof(error.message) - 1;
        }
        memcpy(error.message, message, len);
        error.message[len] = '\0';
    } else {
        error.message[0] = '\0';
    }
    
    return error;
}

/* Check if CUDA is available at runtime */
int SEP_CUDA_IsAvailable(void) {
#if SEP_CUDA_AVAILABLE
    int deviceCount = 0;
    cudaError_t error = cudaGetDeviceCount(&deviceCount);
    return (error == cudaSuccess && deviceCount > 0) ? 1 : 0;
#else
    return 0;
#endif
}

/* Initialization */
SEP_CUDA_ERROR SEP_CUDA_Init(void) {
#if SEP_CUDA_AVAILABLE
    cudaError_t error = cudaFree(0);  /* Simple call to initialize CUDA runtime */
    if (error != cudaSuccess) {
        log_cuda_error(SEP_ERROR_ERROR, error, "Failed to initialize CUDA runtime", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(error, cudaGetErrorString(error));
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "CUDA initialized successfully");
#else
    log_cuda_error(SEP_ERROR_WARNING, SEP_CUDA_ERROR_NOT_INITIALIZED, 
                 "CUDA not available in this build", 
                 __FILE__, __LINE__, __func__);
    return SEP_CUDA_CreateError(SEP_CUDA_ERROR_NOT_INITIALIZED, 
                             "CUDA not available in this build");
#endif
}

/* Memory allocation */
SEP_CUDA_ERROR SEP_CUDA_Malloc(void** ptr, size_t size) {
    if (!ptr) {
        log_cuda_error(SEP_ERROR_ERROR, SEP_CUDA_ERROR_INVALID_VALUE, 
                     "Invalid pointer in SEP_CUDA_Malloc", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(SEP_CUDA_ERROR_INVALID_VALUE, "Invalid pointer");
    }
    
#if SEP_CUDA_AVAILABLE
    cudaError_t error = cudaMalloc(ptr, size);
    if (error != cudaSuccess) {
        log_cuda_error(SEP_ERROR_ERROR, error, 
                     "Failed to allocate CUDA memory", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(error, cudaGetErrorString(error));
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Memory allocated successfully");
#else
    /* Mark size as used to prevent compiler warnings */
    SEP_CUDA_UNUSED(size);
    *ptr = NULL;
    log_cuda_error(SEP_ERROR_WARNING, SEP_CUDA_ERROR_NOT_INITIALIZED, 
                 "CUDA not available for memory allocation", 
                 __FILE__, __LINE__, __func__);
    return SEP_CUDA_CreateError(SEP_CUDA_ERROR_NOT_INITIALIZED, 
                             "CUDA not available in this build");
#endif
}

/* Memory deallocation */
SEP_CUDA_ERROR SEP_CUDA_Free(void* ptr) {
    if (!ptr) {
        /* NULL pointer is a no-op, not an error */
        return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "NULL pointer is a no-op");
    }
    
#if SEP_CUDA_AVAILABLE
    cudaError_t error = cudaFree(ptr);
    if (error != cudaSuccess) {
        log_cuda_error(SEP_ERROR_ERROR, error, 
                     "Failed to free CUDA memory", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(error, cudaGetErrorString(error));
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Memory freed successfully");
#else
    log_cuda_error(SEP_ERROR_WARNING, SEP_CUDA_ERROR_NOT_INITIALIZED, 
                 "CUDA not available for memory deallocation", 
                 __FILE__, __LINE__, __func__);
    return SEP_CUDA_CreateError(SEP_CUDA_ERROR_NOT_INITIALIZED, 
                             "CUDA not available in this build");
#endif
}

/* Memory copy */
SEP_CUDA_ERROR SEP_CUDA_Memcpy(void* dst, const void* src, size_t count, 
                             SEP_CUDA_MEMCPY_KIND kind) {
    /* Check for invalid parameters */
    if (!dst || !src) {
        log_cuda_error(SEP_ERROR_ERROR, SEP_CUDA_ERROR_INVALID_VALUE, 
                     "Invalid source or destination pointer in memory copy", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(SEP_CUDA_ERROR_INVALID_VALUE, 
                                 "Invalid source or destination pointer");
    }
    
#if SEP_CUDA_AVAILABLE
    cudaError_t error = cudaMemcpy(dst, src, count, convert_memcpy_kind(kind));
    if (error != cudaSuccess) {
        log_cuda_error(SEP_ERROR_ERROR, error, 
                     "Failed to copy memory", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(error, cudaGetErrorString(error));
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Memory copied successfully");
#else
    /* Fallback to CPU memcpy for host-to-host copies */
    if (kind == SEP_CUDA_MEMCPY_HOST_TO_HOST) {
        memcpy(dst, src, count);
        return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Memory copied successfully (CPU)");
    }
    log_cuda_error(SEP_ERROR_WARNING, SEP_CUDA_ERROR_NOT_INITIALIZED, 
                 "CUDA not available for device memory operations", 
                 __FILE__, __LINE__, __func__);
    return SEP_CUDA_CreateError(SEP_CUDA_ERROR_NOT_INITIALIZED, 
                             "CUDA not available for device memory operations");
#endif
}

/* Get device count */
SEP_CUDA_ERROR SEP_CUDA_GetDeviceCount(int* count) {
    if (!count) {
        log_cuda_error(SEP_ERROR_ERROR, SEP_CUDA_ERROR_INVALID_VALUE, 
                     "Invalid count pointer in GetDeviceCount", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(SEP_CUDA_ERROR_INVALID_VALUE, "Invalid count pointer");
    }
    
#if SEP_CUDA_AVAILABLE
    cudaError_t error = cudaGetDeviceCount(count);
    if (error != cudaSuccess) {
        *count = 0;
        log_cuda_error(SEP_ERROR_WARNING, error, 
                     "Failed to get device count", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(error, cudaGetErrorString(error));
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Device count retrieved successfully");
#else
    *count = 0;
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "No devices available (no CUDA)");
#endif
}

/* Get device properties */
SEP_CUDA_ERROR SEP_CUDA_GetDeviceProperties(SEP_CUDA_DEVICE_PROPS* prop, int device) {
    if (!prop) {
        log_cuda_error(SEP_ERROR_ERROR, SEP_CUDA_ERROR_INVALID_VALUE, 
                     "Invalid properties pointer in GetDeviceProperties", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(SEP_CUDA_ERROR_INVALID_VALUE, 
                                 "Invalid properties pointer");
    }
    
    /* Initialize properties with zeros */
    memset(prop, 0, sizeof(SEP_CUDA_DEVICE_PROPS));
    
#if SEP_CUDA_AVAILABLE
    cudaDeviceProp cudaProp;
    cudaError_t error = cudaGetDeviceProperties(&cudaProp, device);
    if (error != cudaSuccess) {
        log_cuda_error(SEP_ERROR_WARNING, error, 
                     "Failed to get device properties", 
                     __FILE__, __LINE__, __func__);
        return SEP_CUDA_CreateError(error, cudaGetErrorString(error));
    }
    
    /* Copy relevant properties */
    strncpy(prop->name, cudaProp.name, sizeof(prop->name) - 1);
    prop->name[sizeof(prop->name) - 1] = '\0';
    prop->major = cudaProp.major;
    prop->minor = cudaProp.minor;
    prop->total_global_mem = cudaProp.totalGlobalMem;
    prop->shared_mem_per_block = cudaProp.sharedMemPerBlock;
    prop->max_threads_per_block = cudaProp.maxThreadsPerBlock;
    prop->multi_processor_count = cudaProp.multiProcessorCount;
    prop->warp_size = cudaProp.warpSize;
    memcpy(prop->max_threads_dim, cudaProp.maxThreadsDim, sizeof(prop->max_threads_dim));
    memcpy(prop->max_grid_size, cudaProp.maxGridSize, sizeof(prop->max_grid_size));
    prop->unified_addressing = cudaProp.unifiedAddressing;
    
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Device properties retrieved successfully");
#else
    /* Mark device as used to prevent compiler warnings */
    SEP_CUDA_UNUSED(device);
    strcpy(prop->name, "No CUDA Device");
    log_cuda_error(SEP_ERROR_WARNING, SEP_CUDA_ERROR_NO_DEVICE, 
                 "No CUDA devices available", 
                 __FILE__, __LINE__, __func__);
    return SEP_CUDA_CreateError(SEP_CUDA_ERROR_NO_DEVICE, "No CUDA devices available");
#endif
}

/* Error string function */
const char* SEP_CUDA_ErrorString(SEP_CUDA_ERROR error) {
    /* Copy the error message to the static buffer to avoid returning a pointer to stack memory */
    strncpy(sep_cuda_error_string_buffer, error.message, sizeof(sep_cuda_error_string_buffer) - 1);
    sep_cuda_error_string_buffer[sizeof(sep_cuda_error_string_buffer) - 1] = '\0';
    
    return sep_cuda_error_string_buffer;
}