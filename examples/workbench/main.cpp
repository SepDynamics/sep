#include <memory>
#include <stdexcept>

#include "demo_manager.hpp"
#include "config.hpp"
#include "demos/genesis_pattern.hpp"
#include "demos/audio_visualizer.hpp"
#include "demos/memory_garden.hpp"
#include "demos/cosmo_demo.hpp"
#include "demos/cosmo_sim.hpp"
#include "demos/drug_discovery_demo.hpp"
#include "demos/flocking_demo.hpp"
#include "demos/neural_demo.hpp"
#include "demos/digital_physics_demo.hpp"
#include "demos/annealing_demo.hpp"

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
    const auto& config = workbench::Config::getInstance();
    const auto& engine_config = config.engine();

    g_engine = std::make_unique<Engine>();
    g_engine->setCudaEnabled(engine_config.cuda_enabled);
    g_engine->setMetricsEnabled(engine_config.metrics_enabled);
    // Convert string log level to int
    int log_level = 0;
    if (engine_config.log_level == "debug") log_level = 0;
    else if (engine_config.log_level == "info") log_level = 1;
    else if (engine_config.log_level == "warning") log_level = 2;
    else if (engine_config.log_level == "error") log_level = 3;
    g_engine->setLogLevel(log_level);

    if (!g_engine->initialize()) {
        throw std::runtime_error("Failed to initialize SEP engine");
    }
}

void initializeRenderer() {
    // Load configuration
    auto& config = workbench::Config::getInstance();
    if (!config.load("config.json")) {
        throw std::runtime_error("Failed to load configuration");
    }

    // Initialize renderer with config settings
    g_renderer = std::make_unique<CyclesRenderer>();
    
    const auto& window = config.window();
    g_renderer->setWindowTitle(window.title);
    g_renderer->setWindowSize(window.width, window.height);
    g_renderer->setFullscreen(window.fullscreen);
    g_renderer->setVSync(window.vsync);

    const auto& renderer_cfg = config.renderer();
    g_renderer->setSamples(renderer_cfg.cycles.samples);
    g_renderer->setDenoising(renderer_cfg.cycles.denoising);
    g_renderer->setDevice(renderer_cfg.cycles.device);

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
    demo_manager.registerDemo("memory", []() {
        return std::make_unique<MemoryGardenDemo>();
    });
    demo_manager.registerDemo("cosmo", []() {
        return std::make_unique<CosmoDemo>();
    });
    demo_manager.registerDemo("cosmo_sim", []() {
        return std::make_unique<CosmoSim>();
    });
    demo_manager.registerDemo("drug_discovery", []() {
        return std::make_unique<DrugDiscoveryDemo>();
    });
    demo_manager.registerDemo("flocking", []() {
        return std::make_unique<FlockingDemo>();
    });
    demo_manager.registerDemo("neural", []() {
        return std::make_unique<NeuralDemo>();
    });
    demo_manager.registerDemo("digital_physics", []() {
        return std::make_unique<DigitalPhysicsDemo>();
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
