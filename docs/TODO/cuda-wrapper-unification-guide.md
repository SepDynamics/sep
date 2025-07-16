# CUDA Wrapper Unification Deep Dive

## Problem Analysis

Your CUDA integration is suffering from what I call "Wrapper Hell" - multiple competing abstraction layers that are all trying to control the same underlying API.

### Current State Diagnosis

```
cuda_runtime.h → defines SEP_cuda* macros
     ↓
cuda_wrappers.h → defines cuda* functions directly
     ↓
cuda_impl.cpp → tries to implement both (💥 BOOM!)
```

## The Unification Strategy

### Step 1: Wrapper Inventory

First, map out EVERY CUDA function your codebase uses:

```bash
#!/bin/bash
# tools/cuda_inventory.sh

echo "=== Direct CUDA Calls ==="
grep -rh "cuda[A-Z][a-zA-Z]*(" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*\(cuda[A-Z][a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo -e "\n=== SEP Wrapped Calls ==="
grep -rh "SEP_cuda[a-zA-Z]*(" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*\(SEP_cuda[a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr

echo -e "\n=== Namespace Calls ==="
grep -rh "sep::cuda::[a-zA-Z]*(" src/ --include="*.cpp" --include="*.h" | \
  sed 's/.*sep::cuda::\([a-zA-Z]*\)(.*/\1/' | sort | uniq -c | sort -nr
```

### Step 2: Create the Unified Interface

```cpp
// src/gpu/cuda_api_unified.h
#pragma once

#include <memory>
#include <string>

namespace sep {
namespace gpu {

// Forward declarations to avoid including CUDA headers
struct CudaStream;
struct CudaEvent;

enum class MemcpyKind {
    HostToHost,
    HostToDevice,
    DeviceToHost,
    DeviceToDevice,
    Default
};

enum class StreamFlags {
    Default = 0x00,
    NonBlocking = 0x01
};

// Error handling
class CudaError {
public:
    CudaError(int code, const std::string& message);
    bool is_success() const { return code_ == 0; }
    const std::string& message() const { return message_; }
    int code() const { return code_; }
    
    static CudaError success() { return CudaError(0, "Success"); }
    
private:
    int code_;
    std::string message_;
};

// Main API class - Singleton pattern for global state management
class CudaAPI {
public:
    static CudaAPI& instance();
    
    // Memory operations
    CudaError malloc(void** ptr, size_t size);
    CudaError free(void* ptr);
    CudaError memcpy(void* dst, const void* src, size_t count, MemcpyKind kind);
    CudaError memcpy_async(void* dst, const void* src, size_t count, 
                          MemcpyKind kind, CudaStream* stream);
    CudaError memset(void* ptr, int value, size_t count);
    
    // Stream operations
    CudaError create_stream(CudaStream** stream, StreamFlags flags = StreamFlags::Default);
    CudaError destroy_stream(CudaStream* stream);
    CudaError stream_synchronize(CudaStream* stream);
    CudaError stream_wait_event(CudaStream* stream, CudaEvent* event);
    
    // Device management
    CudaError get_device_count(int* count);
    CudaError set_device(int device);
    CudaError get_device(int* device);
    
    // Error handling
    CudaError get_last_error();
    std::string error_string(const CudaError& error);
    
private:
    CudaAPI() = default;
    ~CudaAPI() = default;
    CudaAPI(const CudaAPI&) = delete;
    CudaAPI& operator=(const CudaAPI&) = delete;
};

// RAII Wrappers
class CudaMemory {
public:
    explicit CudaMemory(size_t size);
    ~CudaMemory();
    
    void* get() { return ptr_; }
    const void* get() const { return ptr_; }
    size_t size() const { return size_; }
    
    // Move semantics only
    CudaMemory(CudaMemory&& other) noexcept;
    CudaMemory& operator=(CudaMemory&& other) noexcept;
    
    // No copy
    CudaMemory(const CudaMemory&) = delete;
    CudaMemory& operator=(const CudaMemory&) = delete;
    
private:
    void* ptr_ = nullptr;
    size_t size_ = 0;
};

} // namespace gpu
} // namespace sep
```

### Step 3: Implementation Strategy

```cpp
// src/gpu/cuda_api_unified.cpp
#include "cuda_api_unified.h"

#ifdef SEP_HAS_CUDA
  #include <cuda_runtime_api.h>
  
  // Map our types to CUDA types
  namespace sep::gpu {
    struct CudaStream { cudaStream_t handle; };
    struct CudaEvent { cudaEvent_t handle; };
  }
#else
  // CPU fallback implementation
  namespace sep::gpu {
    struct CudaStream { int dummy; };
    struct CudaEvent { int dummy; };
  }
#endif

namespace sep::gpu {

// Convert our enums to CUDA enums
static cudaMemcpyKind convert_memcpy_kind(MemcpyKind kind) {
    switch (kind) {
        case MemcpyKind::HostToHost: return cudaMemcpyHostToHost;
        case MemcpyKind::HostToDevice: return cudaMemcpyHostToDevice;
        case MemcpyKind::DeviceToHost: return cudaMemcpyDeviceToHost;
        case MemcpyKind::DeviceToDevice: return cudaMemcpyDeviceToDevice;
        default: return cudaMemcpyDefault;
    }
}

CudaAPI& CudaAPI::instance() {
    static CudaAPI instance;
    return instance;
}

CudaError CudaAPI::malloc(void** ptr, size_t size) {
#ifdef SEP_HAS_CUDA
    cudaError_t err = ::cudaMalloc(ptr, size);
    return CudaError(err, ::cudaGetErrorString(err));
#else
    *ptr = std::malloc(size);
    return CudaError::success();
#endif
}

// ... implement all other methods similarly

} // namespace sep::gpu
```

### Step 4: Migration Process

#### Phase 1: Parallel Implementation (Week 1)
1. Implement the new unified API alongside existing code
2. Don't change any existing code yet
3. Write comprehensive unit tests for the new API

#### Phase 2: Gradual Migration (Week 2-3)
1. Start with leaf modules (no dependencies)
2. Update one file at a time
3. Run tests after each file migration

#### Phase 3: Cleanup (Week 4)
1. Remove old wrapper files
2. Update build system
3. Full regression testing

### Step 5: Build System Integration

```cmake
# src/gpu/CMakeLists.txt
add_library(sep_gpu_interface INTERFACE)
target_include_directories(sep_gpu_interface INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(SEP_ENABLE_CUDA)
    add_library(sep_gpu_cuda STATIC
        cuda_api_unified.cpp
    )
    target_link_libraries(sep_gpu_cuda 
        PRIVATE 
            CUDA::cudart
        PUBLIC
            sep_gpu_interface
    )
    add_library(sep::gpu ALIAS sep_gpu_cuda)
else()
    add_library(sep_gpu_cpu STATIC
        cpu_api_fallback.cpp
    )
    target_link_libraries(sep_gpu_cpu 
        PUBLIC
            sep_gpu_interface
    )
    add_library(sep::gpu ALIAS sep_gpu_cpu)
endif()
```

## Common Pitfalls and Solutions

### Pitfall 1: Include Order Dependencies

**Problem**: CUDA headers must be included before certain system headers.

**Solution**: Use a precompiled header
```cpp
// src/gpu/cuda_pch.h
#ifdef SEP_HAS_CUDA
  #include <cuda_runtime_api.h>
  #include <cuda.h>
#endif

#include <cstddef>
#include <cstring>
#include <memory>
```

### Pitfall 2: Error Handling Inconsistency

**Problem**: Mix of error codes, exceptions, and silent failures.

**Solution**: Consistent error handling pattern
```cpp
#define CUDA_CHECK(call) \
    do { \
        auto error = call; \
        if (!error.is_success()) { \
            SEP_LOG_ERROR("CUDA error at {}:{}: {}", \
                         __FILE__, __LINE__, error.message()); \
            return error; \
        } \
    } while(0)

// Usage:
CudaError some_operation() {
    void* ptr;
    CUDA_CHECK(CudaAPI::instance().malloc(&ptr, 1024));
    // ... more operations
    return CudaError::success();
}
```

### Pitfall 3: Stream/Context Management

**Problem**: Mixing default stream with custom streams causes synchronization issues.

**Solution**: Explicit stream management
```cpp
class CudaStreamPool {
public:
    CudaStream* acquire();
    void release(CudaStream* stream);
    
private:
    std::vector<std::unique_ptr<CudaStream>> streams_;
    std::queue<CudaStream*> available_;
    std::mutex mutex_;
};
```

## Testing Strategy

### Unit Tests
```cpp
TEST(CudaAPI, BasicMemoryOperations) {
    auto& api = CudaAPI::instance();
    
    void* ptr = nullptr;
    ASSERT_TRUE(api.malloc(&ptr, 1024).is_success());
    ASSERT_NE(ptr, nullptr);
    
    ASSERT_TRUE(api.memset(ptr, 0, 1024).is_success());
    ASSERT_TRUE(api.free(ptr).is_success());
}
```

### Integration Tests
```cpp
TEST(CudaAPI, HostDeviceTransfer) {
    std::vector<float> host_data(1024, 1.0f);
    CudaMemory device_mem(1024 * sizeof(float));
    
    auto& api = CudaAPI::instance();
    ASSERT_TRUE(api.memcpy(device_mem.get(), host_data.data(), 
                          device_mem.size(), MemcpyKind::HostToDevice).is_success());
}
```

## Verification Checklist

- [ ] All CUDA calls go through unified API
- [ ] No direct cuda* function calls in application code
- [ ] No SEP_cuda* macros used
- [ ] Build succeeds with SEP_ENABLE_CUDA=ON
- [ ] Build succeeds with SEP_ENABLE_CUDA=OFF
- [ ] All tests pass in both configurations
- [ ] No compilation warnings about redefinitions
- [ ] Performance benchmarks show no regression

## Migration Tracking

Create a tracking spreadsheet:

| File | Status | CUDA Calls | Migrated | Tested | Notes |
|------|--------|------------|----------|--------|-------|
| quantum/processor.cpp | In Progress | 12 | 3/12 | No | Stream sync issues |
| quantum/evolution.cpp | Not Started | 8 | 0/8 | No | - |

## Emergency Rollback Plan

If migration causes critical issues:

1. Keep old headers in `src/gpu/legacy/`
2. Use preprocessor flag: `-DSEP_USE_LEGACY_CUDA=1`
3. Conditional compilation in CMake
4. Gradual re-migration after fixes

This approach gives you a clear path from chaos to order while maintaining the ability to build and test throughout the process.
