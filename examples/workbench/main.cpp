#include "sep_engine_wrapper.h"
#include "demo_manager.hpp"
#include "demos/genesis_pattern.hpp"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace sep;
using namespace sep::workbench;

int main() {
    // Create engine and renderer using the wrapper implementations.  In demo
    // mode these classes provide lightweight stubs so we can run the
    // workbench without the full engine stack.
    auto engine    = std::make_unique<Engine>();
    auto renderer  = std::make_unique<CyclesRenderer>();

    if (!engine->initialize()) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return 1;
    }

    if (!renderer->initialize()) {
        std::cerr << "FATAL: Could not initialize Cycles renderer." << std::endl;
        return 1;
    }

    // Register and start the Genesis demo
    auto& demo_manager = DemoManager::getInstance();
    demo_manager.initialize(engine.get(), renderer.get());
    demo_manager.registerDemo("genesis", [] {
        return std::make_unique<GenesisPatternDemo>();
    });
    if (!demo_manager.switchToDemo("genesis")) {
        std::cerr << "Failed to start demo" << std::endl;
        return 1;
    }

    std::cout << "Workbench is running. Close the window to exit." << std::endl;

    // Main loop using the stub event API provided by the renderer
    const float dt = 1.0f / 60.0f;
    while (!renderer->shouldClose()) {
        // Forward input events to the current demo
        if (renderer->hasKeyEvent()) {
            unsigned char key = renderer->getLastKey();
            demo_manager.handleKeyboard(key);
        }
        if (renderer->hasMouseEvent()) {
            int x, y, button;
            renderer->getLastMouseEvent(x, y, button);
            demo_manager.handleMouse(x, y, button);
        }

        demo_manager.update(dt);
        demo_manager.render();
        renderer->present();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    demo_manager.cleanup();
    renderer.reset();
    engine.reset();

    std::cout << "Workbench shut down cleanly." << std::endl;
    return 0;
}
