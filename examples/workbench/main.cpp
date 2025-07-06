#include "blender/cycles_renderer.h"
#include "quantum/processor.h"
#include "core/manager.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Initialize configuration and quantum processor
    sep::config::ConfigManager::getInstance().initialize(0, nullptr);
    auto q_processor = sep::quantum::createProcessor({});

    // Initialize renderer
    sep::blender::ccl::CyclesRenderer renderer;
    if (renderer.initialize() != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize Cycles renderer." << std::endl;
        return 1;
    }

    // Create genesis pattern
    sep::quantum::Pattern p;
    p.id = "genesis_pattern";
    p.quantum_state.coherence = 0.1f;
    p.quantum_state.stability = 0.1f;
    q_processor->addPattern(p);

    // Simple loop
    while (!renderer.shouldClose()) {
        q_processor->processPattern(p.id);
        auto updated = q_processor->getPattern(p.id);

        std::vector<sep::pattern::PatternData> visual;
        sep::pattern::PatternData d;
        d.position = glm::vec4(0,0,0,1);
        d.coherence = updated.quantum_state.coherence;
        visual.push_back(d);

        renderer.createSceneFromPatterns(visual);
        renderer.renderScene({});
        renderer.present();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
