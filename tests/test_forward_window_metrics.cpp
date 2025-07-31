#include <gtest/gtest.h>
#include "apps/oanda_trader/forward_window_kernels.cuh"

TEST(ForwardWindowMetricsTest, AllFlip) {
    // Create trajectory points using the proper types
    std::vector<sep::quantum::bitspace::TrajectoryPoint> trajectory_points(10);
    for (int i = 0; i < 10; ++i) {
        trajectory_points[i].value = i % 2;
        trajectory_points[i].timestamp = i;
    }
    
    // Create a trajectory vector for the simulation
    std::vector<sep::quantum::bitspace::TrajectoryPoint> trajectories = trajectory_points;
    std::vector<sep::quantum::bitspace::DampedValue> results;
    
    sep::apps::cuda::simulateForwardWindowMetrics(trajectories, results);

    ASSERT_EQ(results.size(), 1);
    EXPECT_TRUE(results[0].converged);
    EXPECT_NEAR(results[0].final_value, 0.5, 1e-5);
}
