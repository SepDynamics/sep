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
    GPUContext() = default;
    virtual ~GPUContext() = default;

    // The only method actually used by pattern_processor.cpp
    virtual SEPResult init(int device_index [[maybe_unused]] = -1) { return SEPResult::SUCCESS; }

    // Simplified shader handling for tests
    SEPResult loadComputeShader(const ::sep::shim::string& path [[maybe_unused]]) {
        ++shader_revision_;
        return SEPResult::SUCCESS;
    }

    SEPResult reloadComputeShaderIfNeeded() { return SEPResult::SUCCESS; }
    uint32_t getShaderRevision() const { return shader_revision_; }

    // These methods are included for API compatibility with the original implementation
    virtual void deleteBuffer(GPUBuffer* buffer [[maybe_unused]]) {}
    virtual void* mapBuffer(GPUBuffer* buffer [[maybe_unused]]) { return nullptr; }
    virtual void unmapBuffer(GPUBuffer* buffer [[maybe_unused]]) {}

private:
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
