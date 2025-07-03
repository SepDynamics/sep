#include "memory_garden.hpp"
#include "quantum/processor.h"
#include "quantum/data.hpp"
#include <glm/glm.hpp>

namespace sep {
namespace workbench {

void MemoryGardenDemo::init() {
    pattern_processor_ = std::make_unique<sep::pattern::PatternProcessor>();
    coherence_manager_ = std::make_unique<sep::memory::QuantumCoherenceManager>(sep::memory::QuantumCoherenceManager::Config{});
    spawnInitial();
}

void MemoryGardenDemo::spawnInitial() {
    sep::pattern::PatternData p;
    p.position = glm::vec4(0.0f);
    pattern_processor_->addPattern(p);
}

void MemoryGardenDemo::update(float dt) {
    (void)dt;
    if (auto_evolve_) pattern_processor_->evolvePatterns();
    if (renderer_) {
        const auto& patterns = pattern_processor_->getPatterns();
        renderer_->createSceneFromPatterns(patterns);
        sep::blender::ccl::CyclesRenderer::RenderParams params;
        params.width = 640; params.height = 480; params.samples = 16;
        renderer_->renderScene(params);
    }
}

void MemoryGardenDemo::render() {}

void MemoryGardenDemo::cleanup() {
    pattern_processor_.reset();
    coherence_manager_.reset();
}

void MemoryGardenDemo::handleKeyboard(unsigned char key) {
    if (key == ' ') auto_evolve_ = !auto_evolve_;
}

void MemoryGardenDemo::handleMouse(int, int, int) {}

} // namespace workbench
} // namespace sep
