#include "core/manager.h"
#include "blender/cycles_renderer.h"
#include "quantum/processor.h"
#include "demos/genesis_pattern.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Initialize configuration and engine
    sep::config::ConfigManager::getInstance().initialize(0, nullptr);
    auto& engine = sep::core::Engine::getInstance();
    engine.init(sep::config::ConfigManager::getInstance().getAPIConfig());

    // Initialize renderer
    sep::blender::ccl::CyclesRenderer renderer;
    if (renderer.initialize() != sep::SEPResult::SUCCESS) {
        std::cerr << "FATAL: Could not initialize Cycles renderer." << std::endl;
        return 1;
    }

    // Setup demo
    auto genesis_demo = std::make_unique<sep::workbench::GenesisPatternDemo>();
    genesis_demo->init(engine, &renderer);

    std::cout << "Workbench is running. Close the window to exit." << std::endl;

    // Main loop
    while (!renderer.shouldClose()) {
        genesis_demo->update(0.016f);
        genesis_demo->render();
        renderer.present();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    genesis_demo->cleanup();
    engine.shutdown();
    std::cout << "Workbench shut down cleanly." << std::endl;
    return 0;
}
