#include "demos/demo_manager.hpp"
#include "../../src/demos/genesis_pattern.hpp"
#include "../../src/demos/annealing_demo.hpp"
#include "../../src/demos/cosmo_demo.hpp"
#include "../../src/demos/flocking_demo.hpp"
#include "../../src/demos/neural_demo.hpp"
#include "../../src/demos/drug_discovery_demo.hpp"
#include "../../src/demos/digital_physics_demo.hpp"
#include "../../src/demos/memory_garden.hpp"
#include "../../src/window.h"
#include "../../src/sep_engine_wrapper.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        return 1;
    }

    sep::workbench::Window window(1280, 720, "SEP Workbench");
    if (!window.initialize()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return 1;
    }
    window.makeContextCurrent();

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return 1;
    }

    auto engine = std::make_unique<sep::EngineWrapper>(sep::config::EngineConfig{});
    if (!engine->initialize()) {
        std::cerr << "Failed to init engine" << std::endl;
        return 1;
    }

    sep::workbench::DemoManager& manager = sep::workbench::DemoManager::getInstance();
    manager.initialize(engine.get(), nullptr);

    manager.registerDemo("genesis", [] { return std::make_unique<sep::workbench::GenesisPatternDemo>(); });
    manager.registerDemo("annealing", [] { return std::make_unique<sep::workbench::AnnealingDemo>(); });
    manager.registerDemo("cosmos", [] { return std::make_unique<sep::workbench::CosmoDemo>(); });
    manager.registerDemo("flocking", [] { return std::make_unique<sep::workbench::FlockingDemo>(); });
    manager.registerDemo("neural", [] { return std::make_unique<sep::workbench::NeuralDemo>(); });
    manager.registerDemo("drug_discovery", [] { return std::make_unique<sep::workbench::DrugDiscoveryDemo>(); });
    manager.registerDemo("digital_physics", [] { return std::make_unique<sep::workbench::DigitalPhysicsDemo>(); });
    manager.registerDemo("memory_garden", [] { return std::make_unique<sep::workbench::MemoryGardenDemo>(); });

    manager.switchToDemo("genesis");

    while (!window.shouldClose()) {
        manager.update(1.0f / 60.0f);
        manager.render();
        window.swapBuffers();
        window.pollEvents();
    }

    manager.cleanup();
    window.cleanup();
    glfwTerminate();
    return 0;
}
