#include <iostream>
#include <string>
#include <map>
#include "demo_manager.hpp"
#include "workbench/demos/genesis_pattern.hpp"
#include "workbench/demos/neural_demo.hpp"
#include "workbench/demos/memory_garden.hpp"
#include "workbench/demos/flocking_demo.hpp"
#include "workbench/demos/cosmo_demo.hpp"
#include "workbench/demos/cosmo_sim.hpp"
#include "workbench/demos/digital_physics_demo.hpp"
#include "workbench/demos/drug_discovery_demo.hpp"
#include "workbench/demos/audio_visualizer.hpp"

namespace sep {
namespace workbench {

// Demo info - maps keyboard shortcuts to demo names
static const std::map<char, std::string> demo_keys = {
    {'1', "genesis"},
    {'2', "neural"},
    {'3', "memory"},
    {'4', "flocking"},
    {'5', "cosmo"},
    {'6', "cosmo_sim"},
    {'7', "physics"},
    {'8', "drug"},
    {'9', "audio"}
};

void registerDemos() {
    auto& manager = DemoManager::getInstance();
    
    // Register all demos
    std::cout << "Registering demos...\n";
    
    // Genesis Pattern Demo
    std::cout << "  1: Genesis Pattern Demo\n";
    manager.registerDemo("genesis", std::make_unique<GenesisPatternDemo>());
    
    // Neural Demo
    std::cout << "  2: Neural Demo\n";
    manager.registerDemo("neural", std::make_unique<NeuralDemo>());
    
    // Memory Garden Demo
    std::cout << "  3: Memory Garden Demo\n";
    manager.registerDemo("memory", std::make_unique<MemoryGarden>());
    
    // Flocking Demo
    std::cout << "  4: Flocking Demo\n";
    manager.registerDemo("flocking", std::make_unique<FlockingDemo>());
    
    // Cosmo Demo
    std::cout << "  5: Cosmo Demo\n";
    manager.registerDemo("cosmo", std::make_unique<CosmoDemo>());
    
    // Cosmo Sim
    std::cout << "  6: Cosmo Sim Demo\n";
    manager.registerDemo("cosmo_sim", std::make_unique<CosmoSim>());
    
    // Digital Physics Demo
    std::cout << "  7: Digital Physics Demo\n";
    manager.registerDemo("physics", std::make_unique<DigitalPhysicsDemo>());
    
    // Drug Discovery Demo
    std::cout << "  8: Drug Discovery Demo\n";
    manager.registerDemo("drug", std::make_unique<DrugDiscoveryDemo>());
    
    // Audio Visualizer Demo
    std::cout << "  9: Audio Visualizer Demo\n";
    manager.registerDemo("audio", std::make_unique<AudioVisualizerDemo>());
    
    std::cout << "Demos registered. Press 1-9 to switch between demos.\n";
}

} // namespace workbench
} // namespace sep
