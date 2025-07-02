#pragma once

#include "core/common.h"
#include "compat/shim.h"
#include <cstdint>

namespace sep {

// Forward declaration for GPU buffer
struct GPUBuffer {
    void* ptr{nullptr};
    size_t size{0};
    bool mapped{false};
};

// Minimal implementation of GPU context for pattern_processor.cpp
class GPUContext {
public:
    GPUContext();
    virtual ~GPUContext();

    // Initialization with optional device index
    virtual SEPResult init(int device_index = -1);
    virtual SEPResult selectDevice(int device_index);
    int getDeviceIndex() const { return device_index_; }

    // Simplified shader handling for tests
    SEPResult loadComputeShader(const ::sep::shim::string& path) {
        ++shader_revision_;
        return SEPResult::SUCCESS;
    }

    SEPResult reloadComputeShaderIfNeeded() { return SEPResult::SUCCESS; }
    uint32_t getShaderRevision() const { return shader_revision_; }

    // Buffer management helpers
    virtual GpuBufferPtr createBuffer(size_t size, const void* data = nullptr);
    virtual void deleteBuffer(GPUBuffer* buffer);
    virtual void* mapBuffer(GPUBuffer* buffer);
    virtual void unmapBuffer(GPUBuffer* buffer);

private:
    int device_index_{-1};
    uint32_t shader_revision_{0};
};

// RAII wrapper for GPUBuffer that works with the simplified context
struct GpuBufferPtr {
    GPUContext* ctx{nullptr};
    GPUBuffer* buffer{nullptr};

    GpuBufferPtr() = default;
    GpuBufferPtr(GPUContext* c, GPUBuffer* b) : ctx(c), buffer(b) {}
    GpuBufferPtr(const GpuBufferPtr&) = delete;
    GpuBufferPtr& operator=(const GpuBufferPtr&) = delete;
    
    GpuBufferPtr(GpuBufferPtr&& other) noexcept 
        : ctx(other.ctx), buffer(other.buffer) {
        other.ctx = nullptr;
        other.buffer = nullptr;
    }
    
    GpuBufferPtr& operator=(GpuBufferPtr&& other) noexcept {
        if (this != &other) {
            reset();
            ctx = other.ctx;
            buffer = other.buffer;
            other.ctx = nullptr;
            other.buffer = nullptr;
        }
        return *this;
    }
    
    ~GpuBufferPtr() {
        reset();
    }

    GPUBuffer* get() const { return buffer; }
    GPUBuffer* operator->() const { return buffer; }
    explicit operator bool() const { return buffer != nullptr; }
    
    GPUBuffer* release() {
        GPUBuffer* tmp = buffer;
        ctx = nullptr;
        buffer = nullptr;
        return tmp;
    }
    
    void reset(GPUBuffer* b = nullptr) {
        if (buffer && ctx) {
            ctx->deleteBuffer(buffer);
        }
        buffer = b;
    }
};

// Deleter for use with unique_ptr
struct GPUBufferDeleter {
    GPUContext* ctx{nullptr};
    
    void operator()(GPUBuffer* buffer) const noexcept {
        if (ctx && buffer) {
            ctx->deleteBuffer(buffer);
        }
    }
};

}  // namespace sep
