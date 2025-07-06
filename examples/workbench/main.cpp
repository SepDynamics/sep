#include "blender/cycles_renderer.h"
#include "quantum/processor.h"
#include "core/manager.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    sep::config::ConfigManager::getInstance().initialize(0, nullptr);
    auto q_processor = sep::quantum::createProcessor({});

    sep::blender::ccl::CyclesRenderer renderer;
    if (renderer.initialize() != sep::SEPResult::SUCCESS) {
        std::cerr << "Failed to initialize Cycles renderer." << std::endl;
        return 1;
    }

    sep::quantum::Pattern p;
    p.id = "genesis_pattern";
    p.quantum_state.coherence = 0.1f;
    p.quantum_state.stability = 0.1f;
    q_processor->addPattern(p);

    while (!renderer.shouldClose()) {
        q_processor->processAll();
        auto updated = q_processor->getPattern(p.id);

        std::vector<sep::pattern::PatternData> vis_data;
        sep::pattern::PatternData d;
        d.position = updated.position;
        d.coherence = updated.quantum_state.coherence;
        d.stability = updated.quantum_state.stability;
        d.entropy = updated.quantum_state.entropy;
        vis_data.push_back(d);

        renderer.createSceneFromPatterns(vis_data);
        sep::blender::ccl::CyclesRenderer::RenderParams params;
        renderer.renderScene(params);
        renderer.present();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
