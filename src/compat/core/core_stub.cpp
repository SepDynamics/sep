#if !SEP_CUDA_AVAILABLE
#include "compat/core.h"
#include <cstring>

namespace sep::cuda {

CudaCore::CudaCore() = default;

Error CudaCore::initialize(int device_id) {
  initialized_ = true;
  current_device_ = device_id;
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

Error CudaCore::setDevice(int device) {
  current_device_ = device;
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

int CudaCore::getDeviceCount() const { return 1; }

Error CudaCore::getDeviceProperties(cudaDeviceProp& props, int device) const {
  (void)device;
  std::memset(&props, 0, sizeof(props));
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

StreamPtr CudaCore::createStream(sep::StreamFlags) { return nullptr; }

Error CudaCore::destroyStream(cudaStream_t) { return {Status::Success, "", "", SEPResult::SUCCESS}; }

Error CudaCore::synchronizeStream(cudaStream_t) { return {Status::Success, "", "", SEPResult::SUCCESS}; }

Error CudaCore::getMemoryInfo(size_t& free, size_t& total) const {
  free = total = 0;
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

Error CudaCore::getLastError() const { return {Status::Success, "", "", SEPResult::SUCCESS}; }

std::string CudaCore::getErrorString(cudaError_t) const { return ""; }

CudaMetrics CudaCore::getMetrics() const { return {}; }

Error CudaCore::updateMetrics() { return {Status::Success, "", "", SEPResult::SUCCESS}; }

Error CudaCore::initializeDevice(int device) {
  current_device_ = device;
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

Error CudaCore::queryDeviceProperties() { return {Status::Success, "", "", SEPResult::SUCCESS}; }

Error CudaCore::launchQBSA(const DeviceMemory<std::uint32_t>&, const DeviceMemory<std::uint32_t>&,
                           std::uint32_t, DeviceMemory<std::uint32_t>&,
                           DeviceMemory<std::uint32_t>&, DeviceMemory<std::uint32_t>&,
                           Stream&) {
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

Error CudaCore::launchQSH(const DeviceMemory<std::uint64_t>&, std::uint32_t,
                          DeviceMemory<std::uint32_t>&, DeviceMemory<std::uint32_t>&,
                          Stream&) {
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

Error CudaCore::launchSimilarity(const DeviceMemory<float>&, const DeviceMemory<float>&,
                                 const DeviceMemory<float>&, std::uint32_t, Stream&) {
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

Error CudaCore::launchBlend(DeviceMemory<float>&, const DeviceMemory<float>&,
                            const DeviceMemory<float>&, std::uint32_t, std::uint32_t,
                            Stream&) {
  return {Status::Success, "", "", SEPResult::SUCCESS};
}

}  // namespace sep::cuda
#endif // !SEP_CUDA_AVAILABLE
