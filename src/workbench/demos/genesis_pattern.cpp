#include "genesis_pattern.hpp"
#include "core/engine.h"
#include "blender/cycles_renderer.hpp"
#include <quantum/quantum_processor.h>
#include <memory/quantum_coherence_manager.h>
#include <quantum/types.h>
#include "config.hpp"

namespace sep {
namespace workbench {

void GenesisPatternDemo::on_load() {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    pattern_processor_ = sep::quantum::createProcessor();
    sep::memory::QuantumCoherenceManager::Config cm_cfg{};
    coherence_manager_ = sep::memory::createQuantumCoherenceManager(cm_cfg);

    // Initialize from config
    evolution_rate_ = genesis_config.initial_pattern.evolution_rate;
    coherence_threshold_ = genesis_config.initial_pattern.coherence_threshold;
    
    initializePatterns();
}

void GenesisPatternDemo::initializePatterns()
{
    // Initialize base quantum pattern
    sep::quantum::Pattern pattern;
    pattern.id = "seed";
    pattern.position = glm::vec4(0.0f);
    pattern.quantum_state.evolution_rate = evolution_rate_;
    pattern.quantum_state.energy = 1.0f;
    pattern.quantum_state.coupling_strength = 0.5f;

    pattern_processor_->addPattern(pattern);
}

void GenesisPatternDemo::on_update(float dt) {
    if (auto_evolve_) {
        evolvePatterns(dt);
    }
    updateVisualization();
}

void GenesisPatternDemo::evolvePatterns(float)
{
    // Evolve all patterns in the processor
    auto batch = pattern_processor_->processAll();

    // Retrieve updated patterns and compute coherence metrics
    auto patterns = pattern_processor_->getPatterns();
    auto coherence = coherence_manager_->updateCoherence(patterns);

    // Update metrics
    metrics_.coherence = coherence.global_coherence;
    metrics_.pattern_count = patterns.size();
    metrics_.evolution_rate = evolution_rate_;
    metrics_.iterations += 1;

    // Trigger visualization update when significant migrations occur
    if (coherence.total_migrations > 0) {
        updateVisualization();
    }
}

void GenesisPatternDemo::updateVisualization() {
    if (!renderer_) return;

    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    // Convert processor patterns to positions for the renderer
    std::vector<glm::vec3> pattern_state;
    for (const auto& p : pattern_processor_->getPatterns()) {
        pattern_state.push_back(glm::vec3(p.position));
    }
    
    // Configure visualization parameters
    renderer_->setRotation(view_.rotation);
    renderer_->setZoom(view_.zoom);
    renderer_->setWireframe(view_.wireframe);
    
    // Configure visualization modes from config
    renderer_->setColorMode(genesis_config.visualization.color_mode);
    renderer_->setEmissionMode(genesis_config.visualization.emission_mode);
    renderer_->setRoughnessMode(0);
    
    // Render updated pattern state
    renderer_->renderPatternState(pattern_state);
}

void GenesisPatternDemo::on_render() {
    // Additional rendering handled by Cycles renderer
}

void GenesisPatternDemo::on_unload() {
    // Release resources
    pattern_processor_.reset();
    coherence_manager_.reset();
    renderer_ = nullptr;
}

void GenesisPatternDemo::on_key_press(int key) {
    const auto& config = Config::getInstance();
    const auto& genesis_config = config.genesis_pattern();

    switch (key) {
        case ' ':  // Space - toggle auto evolution
            auto_evolve_ = !auto_evolve_;
            break;
        case 'w':  // Toggle wireframe
            view_.wireframe = !view_.wireframe;
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

void GenesisPatternDemo::on_mouse(int, int, int button) { (void)button; }

} // namespace workbench
} // namespace sep
