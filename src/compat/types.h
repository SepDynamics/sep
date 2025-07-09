#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace sep {
namespace cuda {

// Basic CUDA types for the SEP system
using DevicePtr = void*;
using HostPtr = void*;

// Memory alignment constants
constexpr size_t CUDA_MEMORY_ALIGNMENT = 256;
constexpr size_t WARP_SIZE = 32;

// Error handling
enum class CudaResult {
  SUCCESS = 0,
  ERROR_INVALID_VALUE,
  ERROR_OUT_OF_MEMORY,
  ERROR_NOT_INITIALIZED,
  ERROR_DEINITIALIZED,
  ERROR_PROFILER_DISABLED,
  ERROR_PROFILER_NOT_INITIALIZED,
  ERROR_PROFILER_ALREADY_STARTED,
  ERROR_PROFILER_ALREADY_STOPPED,
  ERROR_INVALID_CONFIGURATION,
  ERROR_INVALID_PITCH_VALUE,
  ERROR_INVALID_SYMBOL,
  ERROR_INVALID_HOST_POINTER,
  ERROR_INVALID_DEVICE_POINTER,
  ERROR_INVALID_TEXTURE,
  ERROR_INVALID_TEXTURE_BINDING,
  ERROR_INVALID_CHANNEL_DESCRIPTOR,
  ERROR_INVALID_MEMCPY_DIRECTION,
  ERROR_ADDRESS_OF_CONSTANT,
  ERROR_TEXTURE_FETCH_FAILED,
  ERROR_TEXTURE_NOT_BOUND,
  ERROR_SYNCHRONIZATION_ERROR,
  ERROR_INVALID_FILTER_SETTING,
  ERROR_INVALID_NORM_SETTING,
  ERROR_MIXED_DEVICE_EXECUTION,
  ERROR_CUDART_UNLOADING,
  ERROR_UNKNOWN,
  ERROR_NOT_YET_IMPLEMENTED,
  ERROR_MEMORY_VALUE_TOO_LARGE,
  ERROR_INVALID_RESOURCE_HANDLE,
  ERROR_NOT_READY,
  ERROR_INSUFFICIENT_DRIVER,
  ERROR_SET_ON_ACTIVE_PROCESS,
  ERROR_INVALID_SURFACE,
  ERROR_NO_DEVICE,
  ERROR_ECC_UNCORRECTABLE,
  ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND,
  ERROR_SHARED_OBJECT_INIT_FAILED,
  ERROR_UNSUPPORTED_LIMIT,
  ERROR_DUPLICATE_VARIABLE_NAME,
  ERROR_DUPLICATE_TEXTURE_NAME,
  ERROR_DUPLICATE_SURFACE_NAME,
  ERROR_DEVICES_UNAVAILABLE,
  ERROR_INVALID_KERNEL_IMAGE,
  ERROR_NO_KERNEL_IMAGE_FOR_DEVICE,
  ERROR_INCOMPATIBLE_DRIVER_CONTEXT,
  ERROR_PEER_ACCESS_ALREADY_ENABLED,
  ERROR_PEER_ACCESS_NOT_ENABLED,
  ERROR_DEVICE_ALREADY_IN_USE,
  ERROR_PROFILER_DISABLED_DEPRECATED,
  ERROR_ASSERT,
  ERROR_TOO_MANY_PEERS,
  ERROR_HOST_MEMORY_ALREADY_REGISTERED,
  ERROR_HOST_MEMORY_NOT_REGISTERED,
  ERROR_OPERATING_SYSTEM,
  ERROR_PEER_ACCESS_UNSUPPORTED,
  ERROR_LAUNCH_MAX_DEPTH_EXCEEDED,
  ERROR_LAUNCH_FILE_SCOPED,
  ERROR_LAUNCH_PENDING_COUNT_EXCEEDED,
  ERROR_NOT_PERMITTED,
  ERROR_NOT_SUPPORTED,
  ERROR_HARDWARE_STACK_ERROR,
  ERROR_ILLEGAL_INSTRUCTION,
  ERROR_MISALIGNED_ADDRESS,
  ERROR_INVALID_ADDRESS_SPACE,
  ERROR_INVALID_PC,
  ERROR_ILLEGAL_ADDRESS,
  ERROR_INVALID_PTX,
  ERROR_INVALID_GRAPHICS_CONTEXT,
  ERROR_STARTUP_FAILURE = 0x7f
};

// Device properties
struct DeviceProperties {
  char name[256];
  size_t totalGlobalMem;
  size_t sharedMemPerBlock;
  int regsPerBlock;
  int warpSize;
  size_t memPitch;
  int maxThreadsPerBlock;
  int maxThreadsDim[3];
  int maxGridSize[3];
  int clockRate;
  size_t totalConstMem;
  int major;
  int minor;
  size_t textureAlignment;
  size_t texturePitchAlignment;
  int deviceOverlap;
  int multiProcessorCount;
  int kernelExecTimeoutEnabled;
  int integrated;
  int canMapHostMemory;
  int computeMode;
  int maxTexture1D;
  int maxTexture1DMipmap;
  int maxTexture1DLinear;
  int maxTexture2D[2];
  int maxTexture2DMipmap[2];
  int maxTexture2DLinear[3];
  int maxTexture2DGather[2];
  int maxTexture3D[3];
  int maxTexture3DAlt[3];
  int maxTextureCubemap;
  int maxTexture1DLayered[2];
  int maxTexture2DLayered[3];
  int maxTextureCubemapLayered[2];
  int maxSurface1D;
  int maxSurface2D[2];
  int maxSurface3D[3];
  int maxSurface1DLayered[2];
  int maxSurface2DLayered[3];
  int maxSurfaceCubemap;
  int maxSurfaceCubemapLayered[2];
  size_t surfaceAlignment;
  int concurrentKernels;
  int ECCEnabled;
  int pciBusID;
  int pciDeviceID;
  int pciDomainID;
  int tccDriver;
  int asyncEngineCount;
  int unifiedAddressing;
  int memoryClockRate;
  int memoryBusWidth;
  int l2CacheSize;
  int maxThreadsPerMultiProcessor;
  int streamPrioritiesSupported;
  int globalL1CacheSupported;
  int localL1CacheSupported;
  size_t sharedMemPerMultiprocessor;
  int regsPerMultiprocessor;
  int managedMemory;
  int isMultiGpuBoard;
  int multiGpuBoardGroupID;
};

// Quantum State Hierarchy Result structure
struct QSHResult {
  std::vector<std::vector<std::uint32_t>> collapse_indices;
  std::vector<std::uint32_t> collapse_counts;
  std::uint32_t total_collapses{0};
  std::uint32_t total_states{0};
};

}  // namespace cuda
}  // namespace sep