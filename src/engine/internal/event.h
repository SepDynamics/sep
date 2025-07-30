#ifndef SEP_CUDA_EVENT_H
#define SEP_CUDA_EVENT_H

#include <cuda_runtime.h>
#include "engine/internal/stream.h"

namespace sep {
namespace cuda {

class Event {
 public:
  explicit Event(unsigned int flags = cudaEventDefault);
  ~Event();

  Event(const Event&) = delete;
  Event& operator=(const Event&) = delete;

  Event(Event&& other) noexcept;
  Event& operator=(Event&& other) noexcept;

  void record(Stream& stream);
  void synchronize();
  float elapsedTime(Event& start);

  cudaEvent_t handle() const;
  bool valid() const;

 private:
  cudaEvent_t event_ = nullptr;
};

}  // namespace cuda
}  // namespace sep

#endif  // SEP_CUDA_EVENT_H
