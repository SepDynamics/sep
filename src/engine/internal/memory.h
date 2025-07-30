#pragma once

#include <cstddef>
#ifdef __CUDACC__
#include <cuda_runtime.h>
#endif

namespace sep {
namespace cuda {

// Device memory template
template <typename T>
class DeviceMemory {
public:
    DeviceMemory(size_t size = 0);
    ~DeviceMemory();

    DeviceMemory(const DeviceMemory&) = delete;
    DeviceMemory& operator=(const DeviceMemory&) = delete;

    DeviceMemory(DeviceMemory&& other) noexcept;
    DeviceMemory& operator=(DeviceMemory&& other) noexcept;

    T* get() const;
    size_t size() const;

private:
    T* ptr_ = nullptr;
    size_t size_ = 0;
};

}  // namespace cuda
}  // namespace sep
