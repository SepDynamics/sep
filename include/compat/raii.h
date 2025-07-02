#ifndef SEP_CUDA_RAII_H
#define SEP_CUDA_RAII_H

#include <cstddef>
#include <cstdint>
#include <memory>

#include "compat/math_common.h"
#include <cuda_runtime.h>
#include "compat/cuda_common.h"
#include "core/types.h"

namespace sep {
namespace cuda {

class StreamRAII {
 public:
  explicit StreamRAII(sep::StreamFlags flags = sep::StreamFlags::Default);
  ~StreamRAII() noexcept;

  StreamRAII(const StreamRAII&) = delete;
  StreamRAII& operator=(const StreamRAII&) = delete;

  StreamRAII(StreamRAII&& other) noexcept;
  StreamRAII& operator=(StreamRAII&& other) noexcept;

  cudaStream_t get() const { return stream_; }
  bool valid() const { return stream_ != nullptr; }
  void synchronize() const;

 private:
  cudaStream_t stream_{nullptr};
};

class EventRAII {
 public:
  EventRAII();
  ~EventRAII() noexcept;

  EventRAII(const EventRAII&) = delete;
  EventRAII& operator=(const EventRAII&) = delete;

  EventRAII(EventRAII&& other) noexcept;
  EventRAII& operator=(EventRAII&& other) noexcept;

  cudaEvent_t get() const { return event_; }
  bool valid() const { return event_ != nullptr; }
  void synchronize() const;

 private:
  cudaEvent_t event_{nullptr};
};

template <typename T>
class DeviceBufferRAII {
 public:
  explicit DeviceBufferRAII(std::size_t count = 0);
  ~DeviceBufferRAII() noexcept;

  DeviceBufferRAII(const DeviceBufferRAII&) = delete;
  DeviceBufferRAII& operator=(const DeviceBufferRAII&) = delete;

  DeviceBufferRAII(DeviceBufferRAII&& other) noexcept;
  DeviceBufferRAII& operator=(DeviceBufferRAII&& other) noexcept;

  T* get() const { return ptr_; }
  std::size_t size() const { return count_; }
  bool valid() const { return ptr_ != nullptr; }

 private:
  T* ptr_{nullptr};
  std::size_t count_{0};
};

// Type alias for shared Event pointer
using EventPtr = std::shared_ptr<EventRAII>;

// Forward declare memory functions to avoid circular includes
void* allocateDeviceMemory(std::size_t size);
void freeDeviceMemory(void* ptr);
void* allocateUnifiedMemory(std::size_t size, cudaStream_t stream = nullptr);
void freeUnifiedMemory(void* ptr);

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_RAII_H

