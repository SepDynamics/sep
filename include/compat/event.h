#ifndef SEP_CUDA_EVENT_H
#define SEP_CUDA_EVENT_H

#include "compat/macros.h"
#include "compat/cuda_common.h"
#if SEP_CUDA_AVAILABLE
#include "compat/cuda_helpers.h"
#endif
#include "core/types.h"

namespace sep {
namespace cuda {

// Forward declaration
class Stream;

class Event {
public:
    explicit Event(EventFlags flags = EventFlags::Default);
    ~Event();

    // Non-copyable
    Event(const Event&) = delete;
    Event& operator=(const Event&) = delete;

    // Movable
    Event(Event&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr;
    }

    Event& operator=(Event&& other) noexcept {
        if (this != &other) {
            if (handle_) {
                cudaEventDestroy(handle_);
            }
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }

    // Event operations
    void record(Stream& stream);
    void synchronize();
    float elapsedTime(Event& start);

    // Access to underlying handle
    cudaEvent_t handle() const {
        return handle_;
    }
    bool valid() const {
        return handle_ != nullptr;
    }

private:
    cudaEvent_t handle_{nullptr};
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_EVENT_H