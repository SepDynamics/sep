#include <cuda_runtime.h>

#include "event.h"
#include "stream.h"

namespace sep::cuda {

Event::Event(unsigned int flags) {
    cudaEventCreateWithFlags(&event_, flags);
}

Event::~Event() {
    if (event_) {
        cudaEventDestroy(event_);
    }
}

Event::Event(Event&& other) noexcept : event_(other.event_) {
    other.event_ = nullptr;
}

Event& Event::operator=(Event&& other) noexcept {
    if (this != &other) {
        if (event_) {
            cudaEventDestroy(event_);
        }
        event_ = other.event_;
        other.event_ = nullptr;
    }
    return *this;
}

void Event::record(Stream& stream) {
    if (event_ && stream.isValid()) {
        cudaEventRecord(event_, stream.handle());
    }
}

void Event::synchronize() {
    if (event_) {
        cudaEventSynchronize(event_);
    }
}

float Event::elapsedTime(Event& start) {
    float time = 0.0f;
    if (event_ && start.valid()) {
        cudaEventElapsedTime(&time, start.handle(), event_);
    }
    return time;
}

cudaEvent_t Event::handle() const {
    return event_;
}

bool Event::valid() const {
    return event_ != nullptr;
}

}  // namespace sep::cuda
