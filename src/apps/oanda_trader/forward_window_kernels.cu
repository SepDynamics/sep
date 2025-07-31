#include "forward_window_kernels.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cmath>

namespace sep::apps::cuda {

// CUDA kernel to calculate damped value for multiple trajectories
__global__ void trajectoryKernel(const TrajectoryPointDevice* trajectories,
                                 DampedValueDevice* results,
                                 int num_trajectories,
                                 int trajectory_length) {
    int idx = blockIdx.x;
    if (idx >= num_trajectories) {
        return;
    }

    const TrajectoryPointDevice* current_trajectory = &trajectories[idx * trajectory_length];
    double current_value = current_trajectory[0].value;

    const double decay_rate = 0.1;
    const double convergence_threshold = 1e-5;
    const int max_iterations = 100;

    bool converged = false;
    for (int i = 0; i < max_iterations; ++i) {
        double next_value = 0.0;
        double total_weight = 0.0;

        for (int j = 0; j < trajectory_length; ++j) {
            double weight = exp(-static_cast<double>(j) * decay_rate);
            next_value += current_trajectory[j].value * weight;
            total_weight += weight;
        }

        if (total_weight > 0) {
            next_value /= total_weight;
        }

        if (fabs(next_value - current_value) < convergence_threshold) {
            converged = true;
            break;
        }
        current_value = next_value;
    }

    results[idx].final_value = current_value;
    results[idx].converged = converged;
    // Confidence calculation would require historical data on the device, 
    // which is a more complex implementation. For now, we set a neutral value.
    results[idx].confidence = 0.5;
}

// Launcher function for the trajectory kernel
void launchTrajectoryKernel(const TrajectoryPointDevice* trajectory_points,
                            DampedValueDevice* results,
                            int num_trajectories,
                            int trajectory_length) {
    dim3 blockSize(1);
    dim3 gridSize(num_trajectories);

    trajectoryKernel<<<gridSize, blockSize>>>(trajectory_points, results, num_trajectories, trajectory_length);
    
    // It's important to check for errors after launching the kernel
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        // In a real application, you would handle this error appropriately
    }
}

} // namespace sep::apps::cuda

namespace sep::apps::cuda {

void simulateForwardWindowMetrics(const std::vector<sep::quantum::bitspace::TrajectoryPoint>& trajectory_points, std::vector<sep::quantum::bitspace::DampedValue>& results) {
    if (trajectory_points.empty()) {
        results.clear();
        return;
    }

    // This function simulates the kernel's behavior for a single trajectory.
    // The input vector `trajectory_points` is treated as a single trajectory.
    
    // Create a Trajectory object from the input points.
    sep::quantum::bitspace::Trajectory trajectory(trajectory_points);
    
    // Calculate the damped value.
    // The parameters for decay_rate, convergence_threshold, and max_iterations
    // are using the default values from the Trajectory class.
    sep::quantum::bitspace::DampedValue damped_value = trajectory.calculateDampedValue();
    
    // The function is expected to return a vector of results, so we resize it to 1
    // and place our single result in it.
    results.resize(1);
    results[0] = damped_value;
}

} // namespace sep::apps::cuda
