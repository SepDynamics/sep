#include "blender/gpu_context.h"

namespace sep {

// Implementation of the GPUBufferDeleter operator()
void GPUBufferDeleter::operator()(GPUBuffer* buffer) const noexcept {
    if (ctx && buffer) {
        ctx->deleteBuffer(buffer);
    }
}

// That's it! The GPUContext methods are already implemented inline in the header.

}  // namespace sep