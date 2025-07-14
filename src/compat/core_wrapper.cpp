#include "compat/core.h"

extern "C" sep::cuda::Error cuda_core_initialize(int device_id) {
    return sep::cuda::CudaCore::instance().initialize(device_id);
}
