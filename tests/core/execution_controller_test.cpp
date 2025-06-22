#include "core/execution_controller.h"
#include "mock_execution_controller.hpp"
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

using namespace sep::core;
using json = nlohmann::json;

class ExecutionControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        controller = std::make_unique<MockExecutionController>();
    }

    std::unique_ptr<MockExecutionController> controller;
};

TEST_F(ExecutionControllerTest, DefaultThresholds) {
    // Check default thresholds
    EXPECT_EQ(controller->getLowThreshold(), 0);
    EXPECT_EQ(controller->getMediumThreshold(), 50);
    EXPECT_EQ(controller->getHighThreshold(), 80);
    EXPECT_FALSE(controller->wasOptimizationAttempted());
}

TEST_F(ExecutionControllerTest, SetPriorityThresholds) {
    // Set new thresholds
    controller->setPriorityThresholds(10, 60, 90);

    // Verify thresholds were updated
    EXPECT_EQ(controller->getLowThreshold(), 10);
    EXPECT_EQ(controller->getMediumThreshold(), 60);
    EXPECT_EQ(controller->getHighThreshold(), 90);
    EXPECT_TRUE(controller->wasOptimizationAttempted());
}

TEST_F(ExecutionControllerTest, ThresholdOrdering) {
    // Set thresholds in ascending order
    controller->setPriorityThresholds(20, 40, 60);

    // Verify thresholds maintain order
    EXPECT_LT(controller->getLowThreshold(), controller->getMediumThreshold());
    EXPECT_LT(controller->getMediumThreshold(), controller->getHighThreshold());
    EXPECT_TRUE(controller->wasOptimizationAttempted());
}

TEST_F(ExecutionControllerTest, MultipleThresholdUpdates) {
    // First update
    controller->setPriorityThresholds(10, 30, 50);
    EXPECT_TRUE(controller->wasOptimizationAttempted());

    // Second update
    controller->setPriorityThresholds(15, 35, 55);
    EXPECT_TRUE(controller->wasOptimizationAttempted());

    // Verify final thresholds
    EXPECT_EQ(controller->getLowThreshold(), 15);
    EXPECT_EQ(controller->getMediumThreshold(), 35);
    EXPECT_EQ(controller->getHighThreshold(), 55);
}

TEST_F(ExecutionControllerTest, BoundaryThresholds) {
    // Test boundary values
    controller->setPriorityThresholds(0, 50, 100);

    EXPECT_EQ(controller->getLowThreshold(), 0);
    EXPECT_EQ(controller->getMediumThreshold(), 50);
    EXPECT_EQ(controller->getHighThreshold(), 100);
    EXPECT_TRUE(controller->wasOptimizationAttempted());
}
