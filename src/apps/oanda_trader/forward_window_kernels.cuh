#ifndef SEP_APPS_OANDA_TRADER_FORWARD_WINDOW_KERNELS_CUH
#define SEP_APPS_OANDA_TRADER_FORWARD_WINDOW_KERNELS_CUH

#include <vector>
#include <cstdint>
#include "quantum/bitspace/trajectory.h"
#include "quantum/bitspace/forward_window_result.h"

namespace sep::apps::cuda {

// Use full namespace paths to avoid CUDA compilation issues
// using TrajectoryPoint = ::sep::quantum::bitspace::TrajectoryPoint;
// using DampedValue = ::sep::quantum::bitspace::DampedValue;
// using ForwardWindowResult = ::sep::quantum::bitspace::ForwardWindowResult;

// Device-side equivalent of TrajectoryPoint
struct TrajectoryPointDevice {
    double value;
    uint64_t timestamp;
};

// Device-side equivalent of DampedValue
struct DampedValueDevice {
    double final_value;
    double confidence;
    bool converged;
};

// Launcher for the trajectory analysis kernel
void launchTrajectoryKernel(const TrajectoryPointDevice* trajectory_points,
                            DampedValueDevice* results,
                            int num_trajectories,
                            int trajectory_length);

// CPU version for testing and fallback
void simulateForwardWindowMetrics(const std::vector<::sep::quantum::bitspace::TrajectoryPoint>& trajectories, std::vector<::sep::quantum::bitspace::DampedValue>& results);

} // namespace sep::apps::cuda

#endif // SEP_APPS_OANDA_TRADER_FORWARD_WINDOW_KERNELS_CUH
