#ifndef SEP_APPS_OANDA_TRADER_FORWARD_WINDOW_KERNELS_CUH
#define SEP_APPS_OANDA_TRADER_FORWARD_WINDOW_KERNELS_CUH

#include <vector>
#include <cstdint>

namespace sep::apps::cuda {

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

} // namespace sep::apps::cuda

#endif // SEP_APPS_OANDA_TRADER_FORWARD_WINDOW_KERNELS_CUH
