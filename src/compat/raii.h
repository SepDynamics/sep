#ifndef SEP_CUDA_RAII_H
#define SEP_CUDA_RAII_H

#include "compat/cuda_unified.h"

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

  cudaStream_t get() const;
  bool valid() const;
  void synchronize() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_RAII_H

