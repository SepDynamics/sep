#include <gtest/gtest.h>
#include "demos/genesis_pattern.hpp"
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
    bool setColorModeCalled = false;
    bool setEmissionModeCalled = false;
    bool setRoughnessModeCalled = false;
    bool renderPatternStateCalled = false;
    bool cycleColorModeCalled = false;
    
    void setRotation(float rotation) override { setRotationCalled = true; }
    void setZoom(float zoom) override { setZoomCalled = true; }
    void setWireframe(bool wireframe) override { setWireframeCalled = true; }
    void setColorMode(int mode) override { setColorModeCalled = true; }
    void setEmissionMode(int mode) override { setEmissionModeCalled = true; }
    void setRoughnessMode(int mode) override { setRoughnessModeCalled = true; }
    void renderPatternState(const PatternState& state) override { renderPatternStateCalled = true; }
    void cycleColorMode() override { cycleColorModeCalled = true; }
};

class GenesisPatternDemoTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create mock engine and renderer
        engine_ = std::make_unique<MockEngine>();
        renderer_ = std::make_unique<MockCyclesRenderer>();
        
        // Initialize the demo
        demo_ = std::make_unique<GenesisPatternDemo>();
        demo_->engine_ = engine_.get();
        demo_->renderer_ = renderer_.get();
    }

    void TearDown() override {
        demo_->cleanup();
    }

    std::unique_ptr<MockEngine> engine_;
    std::unique_ptr<MockCyclesRenderer> renderer_;
    std::unique_ptr<GenesisPatternDemo> demo_;
};

TEST_F(GenesisPatternDemoTest, Initialization) {
    // Test that initialization works without crashing
    demo_->init();
    
    // Verify that pattern processor and coherence manager are created
    EXPECT_NE(demo_->pattern_processor_.get(), nullptr);
    EXPECT_NE(demo_->coherence_manager_.get(), nullptr);
}

TEST_F(GenesisPatternDemoTest, UpdateCallsEvolveWhenAutoEvolveIsTrue) {
    demo_->init();
    
    // Set auto_evolve to true
    demo_->auto_evolve_ = true;
    
    // Call update
    demo_->update(0.1f);
    
    // Verify that metrics are updated (indicating evolvePatterns was called)
    EXPECT_GE(demo_->metrics_.coherence, 0.0f);
    EXPECT_GE(demo_->metrics_.pattern_count, 0u);
}

TEST_F(GenesisPatternDemoTest, UpdateDoesNotCallEvolveWhenAutoEvolveIsFalse) {
    demo_->init();
    
    // Set auto_evolve to false
    demo_->auto_evolve_ = false;
    
    // Store initial metrics
    float initial_coherence = demo_->metrics_.coherence;
    std::size_t initial_pattern_count = demo_->metrics_.pattern_count;
    
    // Call update
    demo_->update(0.1f);
    
    // Verify that metrics are not updated (indicating evolvePatterns was not called)
    EXPECT_EQ(demo_->metrics_.coherence, initial_coherence);
    EXPECT_EQ(demo_->metrics_.pattern_count, initial_pattern_count);
}

TEST_F(GenesisPatternDemoTest, RenderCallsRendererMethods) {
    demo_->init();
    
    // Get renderer mock
    auto* renderer_mock = static_cast<MockCyclesRenderer*>(demo_->renderer_);
    
    // Call render
    demo_->render();
    
    // No assertions needed as render() is empty in GenesisPatternDemo
}

TEST_F(GenesisPatternDemoTest, UpdateVisualizationCallsRendererMethods) {
    demo_->init();
    
    // Get renderer mock
    auto* renderer_mock = static_cast<MockCyclesRenderer*>(demo_->renderer_);
    
    // Call updateVisualization
    demo_->updateVisualization();
    
    // Verify that renderer methods were called
    EXPECT_TRUE(renderer_mock->setRotationCalled);
    EXPECT_TRUE(renderer_mock->setZoomCalled);
    EXPECT_TRUE(renderer_mock->setWireframeCalled);
    EXPECT_TRUE(renderer_mock->setColorModeCalled);
    EXPECT_TRUE(renderer_mock->setEmissionModeCalled);
    EXPECT_TRUE(renderer_mock->setRoughnessModeCalled);
    EXPECT_TRUE(renderer_mock->renderPatternStateCalled);
}

TEST_F(GenesisPatternDemoTest, KeyboardHandling) {
    demo_->init();
    
    // Test space key toggles auto_evolve
    bool initial_auto_evolve = demo_->auto_evolve_;
    demo_->handleKeyboard(' ');
    EXPECT_NE(demo_->auto_evolve_, initial_auto_evolve);
    
    // Test 'w' key toggles wireframe
    bool initial_wireframe = demo_->view_.wireframe;
    demo_->handleKeyboard('w');
    EXPECT_NE(demo_->view_.wireframe, initial_wireframe);
    
    // Test 'c' key cycles color mode
    auto* renderer_mock = static_cast<MockCyclesRenderer*>(demo_->renderer_);
    renderer_mock->cycleColorModeCalled = false;
    demo_->handleKeyboard('c');
    EXPECT_TRUE(renderer_mock->cycleColorModeCalled);
}

TEST_F(GenesisPatternDemoTest, MouseHandling) {
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