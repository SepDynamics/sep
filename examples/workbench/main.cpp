#include <memory>
#include <stdexcept>

#include "core/engine.h"
#include "blender/cycles_renderer.h"
#include "demo_manager.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/audio_visualizer.hpp"
#include "demos/memory_garden.hpp"

using namespace sep;
using namespace sep::workbench;

// Forward declarations
void initializeEngine();
void initializeRenderer();
void registerDemos();
void mainLoop();
void cleanup();

// Global state
std::unique_ptr<Engine> g_engine;
std::unique_ptr<CyclesRenderer> g_renderer;

int main() {
    try {
        initializeEngine();
        initializeRenderer();
        registerDemos();
        mainLoop();
        cleanup();
        return 0;
    }
    catch (const std::exception& e) {
        fprintf(stderr, "Fatal error: %s\n", e.what());
        return 1;
    }
}

void initializeEngine() {
    const auto& config = Config::getInstance();
    const auto& engine_config = config.engine();

    g_engine = std::make_unique<Engine>();
    g_engine->setCudaEnabled(engine_config.cuda_enabled);
    g_engine->setMetricsEnabled(engine_config.metrics_enabled);
    g_engine->setLogLevel(engine_config.log_level);

    if (!g_engine->initialize()) {
        throw std::runtime_error("Failed to initialize SEP engine");
    }
}

void initializeRenderer() {
    // Load configuration
    auto& config = Config::getInstance();
    if (!config.load("config.json")) {
        throw std::runtime_error("Failed to load configuration");
    }

    // Initialize renderer
    g_renderer = std::make_unique<CyclesRenderer>();

    if (!g_renderer->initialize()) {
        throw std::runtime_error("Failed to initialize Cycles renderer");
    }
}

void registerDemos() {
    auto& demo_manager = DemoManager::getInstance();
    demo_manager.initialize(g_engine.get(), g_renderer.get());

    // Register available demos
    demo_manager.registerDemo("genesis", []() {
        return std::make_unique<GenesisPatternDemo>();
    });
    demo_manager.registerDemo("audio", []() {
        return std::make_unique<AudioVisualizerDemo>();
    });
    demo_manager.registerDemo("garden", []() {
        return std::make_unique<MemoryGardenDemo>();
    });

    // Start with Genesis Pattern demo
    if (!demo_manager.switchToDemo("genesis")) {
        throw std::runtime_error("Failed to start Genesis Pattern demo");
    }
}

void mainLoop() {
    auto& demo_manager = DemoManager::getInstance();
    float dt = 1.0f / 60.0f; // Target 60 FPS
    for (int i = 0; i < 60; ++i) {
        demo_manager.update(dt);
        demo_manager.render();
    }
}

void cleanup() {
    auto& demo_manager = DemoManager::getInstance();
    demo_manager.cleanup();
    g_renderer.reset();
    g_engine.reset();
}