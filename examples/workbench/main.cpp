#include "core/manager.h"
#include "blender/cycles_renderer.h"
#include "quantum/processor.h"
#include "demos/genesis_pattern.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

int main() {
    // 1. Initialize Core Systems
    sep::config::ConfigManager::getInstance().initialize(0, nullptr);
    auto& engine = sep::core::Engine::getInstance();
    engine.init(sep::config::ConfigManager::getInstance().getAPIConfig());

    // 2. Initialize the Renderer
    sep::blender::ccl::CyclesRenderer renderer;
    if (renderer.initialize() != sep::SEPResult::SUCCESS) {
        std::cerr << "FATAL: Could not initialize Cycles renderer." << std::endl;
        return 1;
    }

    // 3. Initialize and run the first demo
    auto demo = std::make_unique<sep::workbench::GenesisPatternDemo>();
    demo->init(engine, &renderer);

    std::cout << "Workbench is running. Close the window to exit." << std::endl;

    // 4. Main Loop
    while (!renderer.shouldClose()) {
        demo->update(0.016f);
        demo->render();
        renderer.present();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    demo->cleanup();
    engine.shutdown();
    std::cout << "Workbench shut down cleanly." << std::endl;

    return 0;
}
