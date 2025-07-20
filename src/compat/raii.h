#ifndef SEP_CUDA_RAII_H
#define SEP_CUDA_RAII_H

#include <cuda_runtime.h>
#include <memory>

namespace sep {
namespace cuda {

class StreamRAII {
 public:
  explicit StreamRAII(unsigned int flags = cudaStreamDefault);
  ~StreamRAII() noexcept;

  StreamRAII(const StreamRAII&) = delete;
  StreamRAII& operator=(const StreamRAII&) = delete;

  StreamRAII(StreamRAII&& other) noexcept;
  StreamRAII& operator=(StreamRAII&& other) noexcept;

  cudaStream_t get() const;
  bool valid() const;
  void synchronize() const;

 private:
  cudaStream_t stream_ = nullptr;
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

  T* get() const;
  std::size_t count() const;
  bool valid() const;

 private:
  T* ptr_ = nullptr;
  std::size_t count_ = 0;
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_RAII_H
