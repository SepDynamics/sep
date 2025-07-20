#include "cuda_api_unified.h"
#include <string>
#include <string.h>
#include <stdio.h>

#ifdef SEP_HAS_CUDA
#include <cuda_runtime_api.h>

#include "cuda_sep.h"

static cudaMemcpyKind convert_memcpy_kind(SEP_CUDA_MEMCPY_KIND kind) {
    switch (kind) {
        case SEP_CUDA_MEMCPY_HOST_TO_HOST: return ::cudaMemcpyHostToHost;
        case SEP_CUDA_MEMCPY_HOST_TO_DEVICE: return ::cudaMemcpyHostToDevice;
        case SEP_CUDA_MEMCPY_DEVICE_TO_HOST: return ::cudaMemcpyDeviceToHost;
        case SEP_CUDA_MEMCPY_DEVICE_TO_DEVICE: return ::cudaMemcpyDeviceToDevice;
        default: return ::cudaMemcpyDefault;
    }
}

static SEP_CUDA_ERROR make_error(cudaError_t cuda_err) {
    SEP_CUDA_ERROR err;
    err.code = (int)cuda_err;
    const char* msg = ::cudaGetErrorString(cuda_err);
    strncpy(err.message, msg, sizeof(err.message) - 1);
    err.message[sizeof(err.message) - 1] = '\0';
    return err;
}

#else
// Non-CUDA stubs
static SEP_CUDA_ERROR make_error(int code, const char* msg) {
    SEP_CUDA_ERROR err;
    err.code = code;
    strncpy(err.message, msg, sizeof(err.message) - 1);
    err.message[sizeof(err.message) - 1] = '\0';
    return err;
}
#endif

SEP_CUDA_ERROR SEP_CUDA_CreateError(int code, const char* message) {
    SEP_CUDA_ERROR err;
    err.code = code;
    strncpy(err.message, message, sizeof(err.message) - 1);
    err.message[sizeof(err.message) - 1] = '\0';
    return err;
}

int SEP_CUDA_IsAvailable(void) {
#ifdef SEP_HAS_CUDA
    int device_count = 0;
    cudaError_t err = ::cudaGetDeviceCount(&device_count);
    return (err == cudaSuccess && device_count > 0) ? 1 : 0;
#else
    return 0;
#endif
}

SEP_CUDA_ERROR SEP_CUDA_Init(void) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaSetDevice(0);
    if (err != cudaSuccess) {
        return make_error(err);
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#else
    return make_error(SEP_CUDA_ERROR_NO_DEVICE, "CUDA not available");
#endif
}

SEP_CUDA_ERROR SEP_CUDA_Malloc(void** ptr, size_t size) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaMalloc(ptr, size);
    if (err != cudaSuccess) {
        return make_error(err);
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#else
    *ptr = malloc(size);
    if (*ptr == NULL) {
        return make_error(SEP_CUDA_ERROR_OUT_OF_MEMORY, "Out of memory");
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#endif
}

SEP_CUDA_ERROR SEP_CUDA_Free(void* ptr) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaFree(ptr);
    if (err != cudaSuccess) {
        return make_error(err);
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#else
    free(ptr);
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#endif
}

SEP_CUDA_ERROR SEP_CUDA_Memcpy(void* dst, const void* src, size_t count, 
                             SEP_CUDA_MEMCPY_KIND kind) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaMemcpy(dst, src, count, convert_memcpy_kind(kind));
    if (err != cudaSuccess) {
        return make_error(err);
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#else
    memcpy(dst, src, count);
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#endif
}

SEP_CUDA_ERROR SEP_CUDA_GetDeviceCount(int* count) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaGetDeviceCount(count);
    if (err != cudaSuccess) {
        return make_error(err);
    }
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#else
    *count = 0;
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#endif
}

SEP_CUDA_ERROR SEP_CUDA_GetDeviceProperties(SEP_CUDA_DEVICE_PROPS* prop, int device) {
#ifdef SEP_HAS_CUDA
    cudaDeviceProp cuda_prop;
    cudaError_t err = ::cudaGetDeviceProperties(&cuda_prop, device);
    if (err != cudaSuccess) {
        return make_error(err);
    }
    
    // Copy relevant properties
    strncpy(prop->name, cuda_prop.name, sizeof(prop->name) - 1);
    prop->name[sizeof(prop->name) - 1] = '\0';
    prop->major = cuda_prop.major;
    prop->minor = cuda_prop.minor;
    prop->total_global_mem = cuda_prop.totalGlobalMem;
    prop->shared_mem_per_block = cuda_prop.sharedMemPerBlock;
    prop->max_threads_per_block = cuda_prop.maxThreadsPerBlock;
    prop->multi_processor_count = cuda_prop.multiProcessorCount;
    prop->warp_size = cuda_prop.warpSize;
    prop->max_threads_dim[0] = cuda_prop.maxThreadsDim[0];
    prop->max_threads_dim[1] = cuda_prop.maxThreadsDim[1];
    prop->max_threads_dim[2] = cuda_prop.maxThreadsDim[2];
    prop->max_grid_size[0] = cuda_prop.maxGridSize[0];
    prop->max_grid_size[1] = cuda_prop.maxGridSize[1];
    prop->max_grid_size[2] = cuda_prop.maxGridSize[2];
    prop->unified_addressing = cuda_prop.unifiedAddressing;
    
    return SEP_CUDA_CreateError(SEP_CUDA_SUCCESS, "Success");
#else
    return make_error(SEP_CUDA_ERROR_NO_DEVICE, "CUDA not available");
#endif
}

std::string SEP_CUDA_ErrorString(SEP_CUDA_ERROR error) {
    return std::string(error.message);
}
