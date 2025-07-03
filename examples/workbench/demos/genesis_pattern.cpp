#include "genesis_pattern.hpp"
#include "core/engine.h"
#include "blender/cycles_renderer.h"
#include "quantum/processor.h"
#include "quantum/data.hpp"
#include "memory/quantum_coherence_manager.h"
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

void GenesisPatternDemo::init() {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    pattern_processor_ = std::make_unique<sep::pattern::PatternProcessor>();
    coherence_manager_ = std::make_unique<sep::memory::QuantumCoherenceManager>(sep::memory::QuantumCoherenceManager::Config{});

    // Initialize from config
    evolution_rate_ = genesis_config.initial_pattern.evolution_rate;
    coherence_threshold_ = genesis_config.initial_pattern.coherence_threshold;
    
    initializePatterns();
}

void GenesisPatternDemo::initializePatterns() {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    pattern::PatternData pattern;
    pattern.id = "seed";
    pattern.position = glm::vec4(0.0f);
    pattern_processor_->addPattern(pattern);
    coherence_manager_->setCoherenceThreshold(coherence_threshold_);
}

void GenesisPatternDemo::update(float dt) {
    if (auto_evolve_) {
        evolvePatterns(dt);
    }
    updateVisualization();
}

void GenesisPatternDemo::evolvePatterns(float dt) {
    (void)dt;
    pattern_processor_->evolvePatterns();
}

void GenesisPatternDemo::updateVisualization() {
    if (!renderer_) return;

    // Update renderer with current patterns
    const auto& patterns = pattern_processor_->getPatterns();
    
    // Render updated pattern state
    renderer_->createSceneFromPatterns(patterns);
    sep::blender::ccl::CyclesRenderer::RenderParams params;
    params.width = 640;
    params.height = 480;
    params.samples = 16;
    renderer_->renderScene(params);
}

void GenesisPatternDemo::render() {
    // Additional rendering handled by Cycles renderer
}

void GenesisPatternDemo::cleanup() {
    // Save current state to config if needed
    const auto& config = Config::getInstance();
    auto& genesis_config = config.genesis_pattern();
    genesis_config.save_state.evolution_rate = evolution_rate_;
    genesis_config.save_state.coherence_threshold = coherence_threshold_;
    genesis_config.save_state.view_settings = view_;
    
    // Release resources
    pattern_processor_.reset();
    coherence_manager_.reset();
    renderer_.reset();
}

void GenesisPatternDemo::handleKeyboard(unsigned char key) {
    switch (key) {
        case ' ':  // Space - toggle auto evolution
            auto_evolve_ = !auto_evolve_;
            break;
        case 'w':  // Toggle wireframe
            view_.wireframe = !view_.wireframe;
            break;
        case '+':  // Increase evolution rate
            evolution_rate_ *= rate_step_;
            evolution_rate_ = std::min(evolution_rate_, max_rate_);
            break;
        case '-':  // Decrease evolution rate
            evolution_rate_ *= 1.0f / rate_step_;
            evolution_rate_ = std::max(evolution_rate_, min_rate_);
            break;
        case 'r':  // Reset view and parameters
            view_.rotation = 0.0f;
            view_.zoom = 1.0f;
            evolution_rate_ = Config::getInstance().genesis_pattern().initial_pattern.evolution_rate;
            break;
        case 'c':  // Cycle color modes
            renderer_->cycleColorMode();
            break;
    }
}

void GenesisPatternDemo::handleMouse(int x, int y, int button) {
    // Update rotation based on mouse movement
    if (button == 0) {  // Left button
        view_.rotation += x * rotation_sensitivity_;
    }
    // Update zoom based on mouse movement
    else if (button == 1) {  // Right button
        float zoom_delta = y * zoom_sensitivity_;
        view_.zoom *= (1.0f + zoom_delta);
        view_.zoom = std::max(min_zoom_, std::min(view_.zoom, max_zoom_));
    }
}

} // namespace workbench
} // namespace sep
