#include <gtest/gtest.h>
#include "demos/annealing_demo.hpp"
#include "core/manager.h"
#include <memory>

namespace sep::workbench::test {

// Mock classes for testing
class MockEngine : public Engine {
public:
    // Minimal implementation for testing
    bool isInitialized() const { return true; }
};

class MockCyclesRenderer : public CyclesRenderer {
public:
    // Track method calls for verification
    bool setRotationCalled = false;
    bool setZoomCalled = false;
    bool setWireframeCalled = false;
    bool renderAnnealingStateCalled = false;
    bool renderAnnealingStatsCalled = false;
    
    void setRotation(float rotation) override { setRotationCalled = true; }
    void setZoom(float zoom) override { setZoomCalled = true; }
    void setWireframe(bool wireframe) override { setWireframeCalled = true; }
    void renderAnnealingState(const AnnealingState& state) { renderAnnealingStateCalled = true; }
    void renderAnnealingStats(const AnnealingStats& stats) { renderAnnealingStatsCalled = true; }
};

class AnnealingDemoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create mock engine and renderer
        engine_ = std::make_unique<MockEngine>();
        renderer_ = std::make_unique<MockCyclesRenderer>();
        
        // Initialize the demo
        demo_ = std::make_unique<AnnealingDemo>();
        demo_->engine_ = engine_.get();
        demo_->renderer_ = renderer_.get();
    }

    void TearDown() override {
        demo_->cleanup();
    }

    std::unique_ptr<MockEngine> engine_;
    std::unique_ptr<MockCyclesRenderer> renderer_;
    std::unique_ptr<AnnealingDemo> demo_;
};

TEST_F(AnnealingDemoTest, Initialization) {
    // Test that initialization works without crashing
    demo_->init();
    
    // Verify that annealing processor is created
    EXPECT_NE(demo_->annealing_processor_.get(), nullptr);
    
    // Verify initial temperature
    EXPECT_GT(demo_->current_temperature_, 0.0f);
}

TEST_F(AnnealingDemoTest, UpdateUpdatesAnnealingState) {
    demo_->init();
    
    // Store initial state
    float initial_temperature = demo_->current_temperature_;
    int initial_iteration = demo_->current_iteration_;
    
    // Call update
    demo_->update(0.1f);
    
    // Verify that annealing state is updated
    if (demo_->is_running_) {
        // If annealing is running, temperature should decrease and iterations increase
        EXPECT_LT(demo_->current_temperature_, initial_temperature);
        EXPECT_GT(demo_->current_iteration_, initial_iteration);
    } else {
        // If not running, state should remain the same
        EXPECT_FLOAT_EQ(demo_->current_temperature_, initial_temperature);
        EXPECT_EQ(demo_->current_iteration_, initial_iteration);
    }
}

TEST_F(AnnealingDemoTest, UpdateVisualizationCallsRendererMethods) {
    demo_->init();
    
    // Get renderer mock
    auto* renderer_mock = static_cast<MockCyclesRenderer*>(demo_->renderer_);
    
    // Call updateVisualization
    demo_->updateVisualization();
    
    // Verify that renderer methods were called
    EXPECT_TRUE(renderer_mock->setRotationCalled);
    EXPECT_TRUE(renderer_mock->setZoomCalled);
    EXPECT_TRUE(renderer_mock->setWireframeCalled);
    EXPECT_TRUE(renderer_mock->renderAnnealingStateCalled);
    EXPECT_TRUE(renderer_mock->renderAnnealingStatsCalled);
}

TEST_F(AnnealingDemoTest, KeyboardHandling) {
    demo_->init();
    
    // Test 'w' key toggles wireframe
    bool initial_wireframe = demo_->view_.wireframe;
    demo_->handleKeyboard('w');
    EXPECT_NE(demo_->view_.wireframe, initial_wireframe);
    
    // Test 'r' key resets view
    demo_->view_.rotation = 45.0f;
    demo_->view_.zoom = 2.0f;
    demo_->handleKeyboard('r');
    EXPECT_FLOAT_EQ(demo_->view_.rotation, 0.0f);
    EXPECT_FLOAT_EQ(demo_->view_.zoom, 1.0f);
    
    // Test 's' key toggles annealing process
    bool initial_running = demo_->is_running_;
    demo_->handleKeyboard('s');
    EXPECT_NE(demo_->is_running_, initial_running);
    
    // Test 'n' key resets annealing
    demo_->current_iteration_ = 100;
    demo_->current_temperature_ = 0.1f;
    demo_->handleKeyboard('n');
    EXPECT_EQ(demo_->current_iteration_, 0);
    EXPECT_GT(demo_->current_temperature_, 0.1f);
}

TEST_F(AnnealingDemoTest, MouseHandling) {
    demo_->init();
    
    // Test left button (0) updates rotation
    float initial_rotation = demo_->view_.rotation;
    demo_->handleMouse(10, 0, 0);
    EXPECT_NE(demo_->view_.rotation, initial_rotation);
    
    // Test right button (1) updates zoom
    float initial_zoom = demo_->view_.zoom;
    demo_->handleMouse(0, 10, 1);
    EXPECT_NE(demo_->view_.zoom, initial_zoom);
}

TEST_F(AnnealingDemoTest, AnnealingProcessCompletes) {
    demo_->init();
    
    // Force annealing to run
    demo_->is_running_ = true;
    
    // Set temperature close to stopping condition
    demo_->current_temperature_ = 0.01f;
    
    // Run update multiple times
    for (int i = 0; i < 10; i++) {
        demo_->update(0.1f);
    }
    
    // Verify that annealing stops when temperature reaches minimum
    if (demo_->current_temperature_ <= demo_->min_temperature_) {
        EXPECT_FALSE(demo_->is_running_);
    }
}

} // namespace sep::workbench::test