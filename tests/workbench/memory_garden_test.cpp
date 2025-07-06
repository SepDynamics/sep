#include <gtest/gtest.h>
#include "demos/memory_garden.hpp"
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
    bool renderMemoryTiersCalled = false;
    bool renderMemoryStatsCalled = false;
    
    void setRotation(float rotation) override { setRotationCalled = true; }
    void setZoom(float zoom) override { setZoomCalled = true; }
    void setWireframe(bool wireframe) override { setWireframeCalled = true; }
    void renderMemoryTiers(const MemoryTierState& state) { renderMemoryTiersCalled = true; }
    void renderMemoryStats(const MemoryStats& stats) { renderMemoryStatsCalled = true; }
};

class MemoryGardenDemoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create mock engine and renderer
        engine_ = std::make_unique<MockEngine>();
        renderer_ = std::make_unique<MockCyclesRenderer>();
        
        // Initialize the demo
        demo_ = std::make_unique<MemoryGardenDemo>();
        demo_->engine_ = engine_.get();
        demo_->renderer_ = renderer_.get();
    }

    void TearDown() override {
        demo_->cleanup();
    }

    std::unique_ptr<MockEngine> engine_;
    std::unique_ptr<MockCyclesRenderer> renderer_;
    std::unique_ptr<MemoryGardenDemo> demo_;
};

TEST_F(MemoryGardenDemoTest, Initialization) {
    // Test that initialization works without crashing
    demo_->init();
    
    // Verify that memory tier manager is created
    EXPECT_NE(demo_->memory_tier_manager_.get(), nullptr);
}

TEST_F(MemoryGardenDemoTest, UpdateUpdatesMemoryStats) {
    demo_->init();
    
    // Call update
    demo_->update(0.1f);
    
    // Verify that memory stats are updated
    EXPECT_GE(demo_->stats_.stm_usage, 0.0f);
    EXPECT_GE(demo_->stats_.mtm_usage, 0.0f);
    EXPECT_GE(demo_->stats_.ltm_usage, 0.0f);
}

TEST_F(MemoryGardenDemoTest, UpdateVisualizationCallsRendererMethods) {
    demo_->init();
    
    // Get renderer mock
    auto* renderer_mock = static_cast<MockCyclesRenderer*>(demo_->renderer_);
    
    // Call updateVisualization
    demo_->updateVisualization();
    
    // Verify that renderer methods were called
    EXPECT_TRUE(renderer_mock->setRotationCalled);
    EXPECT_TRUE(renderer_mock->setZoomCalled);
    EXPECT_TRUE(renderer_mock->setWireframeCalled);
    EXPECT_TRUE(renderer_mock->renderMemoryTiersCalled);
    EXPECT_TRUE(renderer_mock->renderMemoryStatsCalled);
}

TEST_F(MemoryGardenDemoTest, KeyboardHandling) {
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
    
    // Test 'c' key toggles auto-cleanup
    bool initial_auto_cleanup = demo_->auto_cleanup_;
    demo_->handleKeyboard('c');
    EXPECT_NE(demo_->auto_cleanup_, initial_auto_cleanup);
}

TEST_F(MemoryGardenDemoTest, MouseHandling) {
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

} // namespace sep::workbench::test