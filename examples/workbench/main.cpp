#include <memory>
#include <stdexcept>
#include <iostream>

// Include our wrapper header instead of direct includes
#include "sep_engine_wrapper.h"

// Include demo headers
#include "demo_manager.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/audio_visualizer.hpp"
#include "demos/memory_garden.hpp"
#include "demos/annealing_demo.hpp"

using namespace sep;
using namespace sep::workbench;

// Forward declarations
void initializeEngine();
void initializeRenderer();
void registerDemos();
void mainLoop();
void cleanup();

// Global state - use the same types for both demo and non-demo modes
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
#ifdef SEP_WORKBENCH_DEMO
    // In demo mode, use simplified initialization
    g_engine = std::make_unique<Engine>();
    
    // Set default values
    g_engine->setCudaEnabled(false);
    g_engine->setMetricsEnabled(false);
    g_engine->setLogLevel(0);
    
    if (!g_engine->initialize()) {
        throw std::runtime_error("Failed to initialize SEP engine");
    }
#else
    // Use the correct ConfigManager class with proper namespace
    const auto& config_manager = sep::core::config::ConfigManager::getInstance();
    const auto& engine_config = config_manager.getEngineConfig();

    g_engine = std::make_unique<Engine>();
    g_engine->setCudaEnabled(engine_config.cuda_enabled);
    g_engine->setMetricsEnabled(engine_config.metrics_enabled);
    g_engine->setLogLevel(engine_config.log_level);

    if (!g_engine->initialize()) {
        throw std::runtime_error("Failed to initialize SEP engine");
    }
#endif
}

void initializeRenderer() {
#ifdef SEP_WORKBENCH_DEMO
    // In demo mode, use simplified initialization
    g_renderer = std::make_unique<CyclesRenderer>();
    
    // Set default values
    g_renderer->setWindowTitle("SEP Workbench Demo");
    g_renderer->setWindowSize(1280, 720);
    g_renderer->setFullscreen(false);
    g_renderer->setVSync(true);
    g_renderer->setSamples(64);
    g_renderer->setDenoising(true);
    g_renderer->setDevice("CPU");
    
    if (!g_renderer->initialize()) {
        throw std::runtime_error("Failed to initialize Cycles renderer");
    }
#else
    // Load configuration
    auto& config_manager = sep::core::config::ConfigManager::getInstance();
    if (!config_manager.load("config.json")) {
        throw std::runtime_error("Failed to load configuration");
    }

    // Initialize renderer with config settings
    g_renderer = std::make_unique<CyclesRenderer>();
    
    const auto& window = config_manager.getWindowConfig();
    g_renderer->setWindowTitle(window.title);
    g_renderer->setWindowSize(window.width, window.height);
    g_renderer->setFullscreen(window.fullscreen);
    g_renderer->setVSync(window.vsync);

    const auto& renderer = config_manager.getRendererConfig();
    g_renderer->setSamples(renderer.cycles.samples);
    g_renderer->setDenoising(renderer.cycles.denoising);
    g_renderer->setDevice(renderer.cycles.device);

    if (!g_renderer->initialize()) {
        throw std::runtime_error("Failed to initialize Cycles renderer");
    }
#endif
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
    demo_manager.registerDemo("memory", []() {
        return std::make_unique<MemoryGardenDemo>();
    });
    demo_manager.registerDemo("annealing", []() {
        return std::make_unique<AnnealingDemo>();
    });

    // Start with Genesis Pattern demo
    if (!demo_manager.switchToDemo("genesis")) {
        throw std::runtime_error("Failed to start Genesis Pattern demo");
    }
}

void mainLoop() {
    auto& demo_manager = DemoManager::getInstance();
    float dt = 1.0f / 60.0f; // Target 60 FPS

    while (true) {
        // Process window events and input
        if (g_renderer->shouldClose()) {
            break;
        }

        // Handle keyboard input
        if (g_renderer->hasKeyEvent()) {
            unsigned char key = g_renderer->getLastKey();
            demo_manager.handleKeyboard(key);
        }

        // Handle mouse input
        if (g_renderer->hasMouseEvent()) {
            int x, y, button;
            g_renderer->getLastMouseEvent(x, y, button);
            demo_manager.handleMouse(x, y, button);
        }

        // Update and render current demo
        demo_manager.update(dt);
        demo_manager.render();

        // Swap buffers and poll events
        g_renderer->present();
    }
}

void cleanup() {
    auto& demo_manager = DemoManager::getInstance();
    demo_manager.cleanup();
    g_renderer.reset();
    g_engine.reset();
}