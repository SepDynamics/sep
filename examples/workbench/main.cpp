#include "blender/cycles_renderer.h"
#include "quantum/processor.h"
#include "quantum/data.hpp"
#include "core/manager.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 1. Initialize Engine & Config
    sep::config::ConfigManager::getInstance().initialize(0, nullptr);
    auto q_processor = sep::quantum::createProcessor({});

    // 2. Initialize Renderer
    sep::blender::ccl::CyclesRenderer renderer;
    if (renderer.initialize() != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize Cycles renderer." << std::endl;
        return 1;
    }

    // 3. Create a single "Genesis Pattern"
    sep::quantum::Pattern p;
    p.id = "genesis_pattern";
    p.quantum_state.coherence = 0.1f;
    p.quantum_state.stability = 0.1f;
    q_processor->addPattern(p);

    // 4. Main Loop
    for (int i = 0; i < 60; ++i) {
        q_processor->processPattern(p.id);
        auto updated_pattern = q_processor->getPattern(p.id);

        std::vector<sep::pattern::PatternData> visual_data;
        sep::pattern::PatternData d{};
        d.position = glm::vec4(0,0,0,1);
        d.coherence = updated_pattern.quantum_state.coherence;
        visual_data.push_back(d);

        renderer.createSceneFromPatterns(visual_data);
        renderer.renderScene({});

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
