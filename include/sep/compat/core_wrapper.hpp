#pragma once

namespace sep::cuda {

class CudaCore {
public:
    // Initialize CUDA with the specified device
    static void initialize(int device);
};

} // namespace sep::cuda