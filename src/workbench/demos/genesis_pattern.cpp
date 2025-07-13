#include "genesis_pattern.hpp"

#include "blender/cycles_renderer.hpp"
#include "config.hpp"
#include "core/engine.h"
#include "core/types.h"
#include "memory/quantum_coherence_manager.h"
#include "quantum/quantum_processor.h"
namespace sep { namespace quantum { using Pattern = ::sep::Pattern; } }
#include "imgui.h"

namespace sep {
namespace workbench {

void GenesisPatternDemo::on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) {
    engine_ = engine;
    renderer_ = renderer;

    pattern_processor_ = sep::quantum::createProcessor();
    sep::memory::QuantumCoherenceManager::Config cm_cfg{};
    coherence_manager_ = sep::memory::createQuantumCoherenceManager(cm_cfg);

    initializePatterns();
}

void GenesisPatternDemo::initializePatterns()
{
    // Initialize base quantum pattern
    sep::Pattern pattern;
    pattern.id = "seed";
    pattern.position = glm::vec4(0.0f);
    pattern.quantum_state.energy = 1.0f;
    pattern.quantum_state.coupling_strength = 0.5f;

    pattern_processor_->addPattern(pattern);
}

void GenesisPatternDemo::on_update(float dt)
{
    evolvePatterns(dt);
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

void GenesisPatternDemo::on_ui_render() {
    // Draw ImGui controls for the demo
    ImGui::Begin("Genesis Pattern Controls");
    
    ImGui::Text("Pattern Count: %zu", metrics_.pattern_count);
    ImGui::Text("Global Coherence: %.3f", metrics_.coherence);
    ImGui::Text("Iterations: %zu", metrics_.iterations);
    
    ImGui::Separator();

    ImGui::Checkbox("Wireframe", &view_.wireframe);
    ImGui::SliderFloat("Rotation", &view_.rotation, 0.0f, 360.0f);
    ImGui::SliderFloat("Zoom", &view_.zoom, 0.1f, 5.0f);
    
    if (ImGui::Button("Reset View")) {
        view_.rotation = 0.0f;
        view_.zoom = 1.0f;
    }
    
    ImGui::End();
}

void GenesisPatternDemo::on_unload() {
    // Release resources
    pattern_processor_.reset();
    coherence_manager_.reset();
    renderer_ = nullptr;
}

void GenesisPatternDemo::on_key_press(int key)
{
    switch (key) {
        case ' ':  // Space - toggle auto evolution
            break;
        case 'w':  // Toggle wireframe
            view_.wireframe = !view_.wireframe;
            break;
        case 'r':  // Reset view and parameters
            view_.rotation = 0.0f;
            view_.zoom = 1.0f;
            break;
        case 'c':  // Cycle color modes
            renderer_->cycleColorMode();
            break;
    }
}

void GenesisPatternDemo::on_mouse(int, int, int button) { (void)button; }

} // namespace workbench
} // namespace sep
