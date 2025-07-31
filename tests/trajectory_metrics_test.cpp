#include <gtest/gtest.h>
#include "quantum/bitspace/trajectory.h"
#include "quantum/bitspace/pattern_processor.h"
#include "apps/oanda_trader/forward_window_kernels.cuh"
#include <cuda_runtime.h>

// Test fixture for trajectory metrics tests
class TrajectoryMetricsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup can go here
    }

    void TearDown() override {
        // Common teardown can go here
    }
};

TEST_F(TrajectoryMetricsTest, DampedValueCalculation) {
    std::vector<sep::quantum::bitspace::TrajectoryPoint> points = {{1.0, 0}, {1.5, 1}, {1.2, 2}};
    sep::quantum::bitspace::Trajectory trajectory(points);
    auto damped_value = trajectory.calculateDampedValue();
    EXPECT_TRUE(damped_value.converged);
    EXPECT_NEAR(damped_value.final_value, 1.23, 0.01);
}

TEST_F(TrajectoryMetricsTest, MetricsCalculation) {
    std::vector<sep::quantum::bitspace::TrajectoryPoint> points = {{1.0, 0}, {1.5, 1}, {1.2, 2}};
    sep::quantum::bitspace::Trajectory trajectory(points);
    sep::quantum::bitspace::PatternProcessor processor;
    auto metrics = processor.processTrajectory(trajectory);
    
    EXPECT_NEAR(metrics.coherence, 0.989, 0.01);
    EXPECT_NEAR(metrics.stability, 1.0, 0.01);
    EXPECT_NEAR(metrics.entropy, 0.113, 0.01);
}

TEST_F(TrajectoryMetricsTest, ConfidenceScoring) {
    std::map<std::string, std::vector<double>> historical_paths;
    historical_paths["path1"] = {1.0, 1.5, 1.2};

    std::vector<sep::quantum::bitspace::TrajectoryPoint> points = {{1.0, 0}, {1.5, 1}, {1.2, 2}};
    sep::quantum::bitspace::Trajectory trajectory(points);
    sep::quantum::bitspace::PatternProcessor processor(historical_paths);
    auto metrics = processor.processTrajectory(trajectory);

    EXPECT_NEAR(metrics.confidence, 0.994, 0.01);
}

TEST_F(TrajectoryMetricsTest, CudaCpuParity) {
    std::vector<sep::quantum::bitspace::TrajectoryPoint> points = {{1.0, 0}, {1.5, 1}, {1.2, 2}};
    sep::quantum::bitspace::Trajectory trajectory(points);
    auto cpu_damped_value = trajectory.calculateDampedValue();

    // Prepare data for CUDA kernel
    std::vector<sep::apps::cuda::TrajectoryPointDevice> device_points;
    for(const auto& p : points) {
        device_points.push_back({p.value, p.timestamp});
    }

    sep::apps::cuda::TrajectoryPointDevice* d_points;
    sep::apps::cuda::DampedValueDevice* d_result;

    cudaMalloc(&d_points, sizeof(sep::apps::cuda::TrajectoryPointDevice) * device_points.size());
    cudaMalloc(&d_result, sizeof(sep::apps::cuda::DampedValueDevice));

    cudaMemcpy(d_points, device_points.data(), sizeof(sep::apps::cuda::TrajectoryPointDevice) * device_points.size(), cudaMemcpyHostToDevice);

    // Launch kernel
    sep::apps::cuda::launchTrajectoryKernel(d_points, d_result, 1, device_points.size());

    // Copy result back
    sep::apps::cuda::DampedValueDevice h_result;
    cudaMemcpy(&h_result, d_result, sizeof(sep::apps::cuda::DampedValueDevice), cudaMemcpyDeviceToHost);

    EXPECT_TRUE(h_result.converged);
    EXPECT_NEAR(cpu_damped_value.final_value, h_result.final_value, 1e-5);

    cudaFree(d_points);
    cudaFree(d_result);
}
