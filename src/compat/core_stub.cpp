#if !SEP_CUDA_AVAILABLE
#include "compat/core_stub.h" // Include the stub header
#include "compat/cuda_runtime.h"
#include <cstring>
#include <cstdlib>

namespace sep::cuda {

cudaError_t cudaMallocManaged(void** ptr, size_t size) {
    if (!ptr) return cudaErrorInvalidValue;
    *ptr = std::malloc(size);
    return *ptr ? cudaSuccess : cudaErrorMemoryAllocation;
}

cudaError_t cudaFree(void* ptr) {
    std::free(ptr);
    return cudaSuccess;
}

cudaError_t cudaMemcpy(void* dst, const void* src, size_t size, int kind) {
    (void)kind;
    if (dst && src) std::memcpy(dst, src, size);
    return cudaSuccess;
}

cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t size, int kind, cudaStream_t stream) {
    (void)stream;
    return cudaMemcpy(dst, src, size, kind);
}

CudaCore::CudaCore() = default;

Error CudaCore::initialize(int device_id) {
  initialized_ = true;
  current_device_ = device_id;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::setDevice(int device) {
  current_device_ = device;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

int CudaCore::getDeviceCount() const { return 1; }

Error CudaCore::getDeviceProperties(cudaDeviceProp& props, int device) const {
  (void)device;
  std::memset(&props, 0, sizeof(props));
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

// Dummy implementation of Stream for CPU-only mode
class DummyStream : public Stream {
public:
    DummyStream() = default;
    ~DummyStream() = default;
    
    void synchronize() {}
    void wait(void* event) { (void)event; }
    void record(void* event) { (void)event; }
    void* handle() const { return nullptr; }
    bool isValid() const { return true; }
};

std::shared_ptr<Stream> CudaCore::createStream(sep::StreamFlags) { // Fix: Return shared_ptr<Stream>
    // Create a dummy stream implementation that doesn't fail
    return std::make_shared<DummyStream>();
}

Error CudaCore::destroyStream(cudaStream_t) { return {Status::Success, "", "", sep::SEPResult::SUCCESS}; }

Error CudaCore::synchronizeStream(cudaStream_t) { return {Status::Success, "", "", sep::SEPResult::SUCCESS}; }

Error CudaCore::getMemoryInfo(size_t& free, size_t& total) const {
  free = total = 0;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::getLastError() const { return {Status::Success, "", "", sep::SEPResult::SUCCESS}; }

std::string CudaCore::getErrorString(cudaError_t) const { return ""; }

CudaMetrics CudaCore::getMetrics() const { return {}; }

Error CudaCore::updateMetrics() { return {Status::Success, "", "", sep::SEPResult::SUCCESS}; }

Error CudaCore::initializeDevice(int device) {
  current_device_ = device;
  return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::queryDeviceProperties() { return {Status::Success, "", "", sep::SEPResult::SUCCESS}; }

Error CudaCore::launchQBSA(const DeviceMemory<std::uint32_t>&, const DeviceMemory<std::uint32_t>&,
                           std::uint32_t, DeviceMemory<std::uint32_t>&,
                           DeviceMemory<std::uint32_t>&, DeviceMemory<std::uint32_t>&,
                           Stream&) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::launchQSH(const DeviceMemory<std::uint64_t>&, std::uint32_t,
                          DeviceMemory<std::uint32_t>&, DeviceMemory<std::uint32_t>&,
                          Stream&) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::launchSimilarity(const DeviceMemory<float>&, const DeviceMemory<float>&,
                                 const DeviceMemory<float>&, std::uint32_t, Stream&) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

Error CudaCore::launchBlend(DeviceMemory<float>&, const DeviceMemory<float>&,
                            const DeviceMemory<float>&, std::uint32_t, std::uint32_t,
                            Stream&) {
    return {Status::Success, "", "", sep::SEPResult::SUCCESS};
}

}  // namespace sep::cuda
#endif // !SEP_CUDA_AVAILABLE
