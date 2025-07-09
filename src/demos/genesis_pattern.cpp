#include "genesis_pattern.hpp"
#include "core/engine.h"
#include "blender/cycles_renderer.hpp"
#include "quantum/pattern_processor.hpp"
#include "memory/quantum_coherence_manager.h"

namespace sep {
namespace workbench {

void GenesisPatternDemo::init() {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    pattern_processor_ = std::make_unique<PatternProcessor>(engine_);
    coherence_manager_ = std::make_unique<QuantumCoherenceManager>();

    // Initialize from config
    evolution_rate_ = genesis_config.initial_pattern.evolution_rate;
    coherence_threshold_ = genesis_config.initial_pattern.coherence_threshold;
    
    initializePatterns();
}

void GenesisPatternDemo::initializePatterns() {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    // Initialize base quantum state patterns
    QuantumState initial_state;
    initial_state.evolution_rate = evolution_rate_;
    initial_state.energy_level = 1.0f;
    initial_state.coupling_strength = 0.5f;
    initial_state.dimensions = {
        genesis_config.initial_pattern.dimensions[0],
        genesis_config.initial_pattern.dimensions[1],
        genesis_config.initial_pattern.dimensions[2]
    };

    pattern_processor_->initializeState(initial_state);
    coherence_manager_->setCoherenceThreshold(coherence_threshold_);
}

void GenesisPatternDemo::update(float dt) {
    if (auto_evolve_) {
        evolvePatterns(dt);
    }
    updateVisualization();
}

void GenesisPatternDemo::evolvePatterns(float dt) {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    // Process pattern evolution with configured rate
    auto result = pattern_processor_->evolvePatterns(dt * evolution_rate_ * genesis_config.evolution.rate_multiplier);
    
    // Update coherence metrics
    coherence_manager_->updateCoherence(result);
    
    // Update metrics
    metrics_.coherence = result.overall_coherence;
    metrics_.pattern_count = result.pattern_count;
    metrics_.evolution_rate = evolution_rate_ * genesis_config.evolution.rate_multiplier;
    metrics_.iterations = genesis_config.evolution.iterations_per_frame;
    
    // Trigger visualization update if coherence changes significantly
    if (result.coherence_delta > genesis_config.visualization.coherence_threshold) {
        updateVisualization();
    }
}

void GenesisPatternDemo::updateVisualization() {
    if (!renderer_) return;

    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    // Update renderer with current pattern state
    auto pattern_state = pattern_processor_->getCurrentState();
    
    // Configure visualization parameters
    renderer_->setRotation(view_.rotation);
    renderer_->setZoom(view_.zoom);
    renderer_->setWireframe(view_.wireframe);
    
    // Configure visualization modes from config
    renderer_->setColorMode(genesis_config.visualization.color_mode);
    renderer_->setEmissionMode(genesis_config.visualization.emission_mode);
    renderer_->setRoughnessMode(genesis_config.visualization.roughness_mode);
    
    // Render updated pattern state
    renderer_->renderPatternState(pattern_state);
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
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    switch (key) {
        case ' ':  // Space - toggle auto evolution
            auto_evolve_ = !auto_evolve_;
            break;
        case 'w':  // Toggle wireframe
            view_.wireframe = !view_.wireframe;
            break;
        case '+':  // Increase evolution rate
            evolution_rate_ *= genesis_config.evolution.rate_step;
            evolution_rate_ = std::min(evolution_rate_, genesis_config.evolution.max_rate);
            break;
        case '-':  // Decrease evolution rate
            evolution_rate_ *= 1.0f / genesis_config.evolution.rate_step;
            evolution_rate_ = std::max(evolution_rate_, genesis_config.evolution.min_rate);
            break;
        case 'r':  // Reset view and parameters
            view_.rotation = 0.0f;
            view_.zoom = 1.0f;
            evolution_rate_ = genesis_config.initial_pattern.evolution_rate;
            break;
        case 'c':  // Cycle color modes
            renderer_->cycleColorMode();
            break;
    }
}

void GenesisPatternDemo::handleMouse(int x, int y, int button) {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    // Update rotation based on mouse movement
    if (button == 0) {  // Left button
        view_.rotation += x * genesis_config.controls.rotation_sensitivity;
    }
    // Update zoom based on mouse movement
    else if (button == 1) {  // Right button
        float zoom_delta = y * genesis_config.controls.zoom_sensitivity;
        view_.zoom *= (1.0f + zoom_delta);
        view_.zoom = std::max(genesis_config.controls.min_zoom,
                             std::min(view_.zoom, genesis_config.controls.max_zoom));
    }
}

} // namespace workbench
} // namespace sep