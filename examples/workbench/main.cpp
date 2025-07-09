#include "demos/demo_manager.hpp"
#include "../../src/demos/genesis_pattern.hpp"
#include "../../src/demos/annealing_demo.hpp"
#include "../../src/demos/cosmo_demo.hpp"
#include "../../src/demos/flocking_demo.hpp"
#include "../../src/demos/neural_demo.hpp"
#include "../../src/demos/drug_discovery_demo.hpp"
#include "../../src/demos/digital_physics_demo.hpp"
#include "../../src/demos/memory_garden.hpp"

int main() {
    auto& manager = sep::workbench::DemoManager::instance();

    manager.registerDemo("genesis", []() {
        return std::make_unique<sep::workbench::GenesisPatternDemo>();
    });

    manager.registerDemo("annealing", []() {
        return std::make_unique<sep::workbench::AnnealingDemo>();
    });

    manager.registerDemo("cosmos", []() {
        return std::make_unique<sep::workbench::CosmoDemo>();
    });

    manager.registerDemo("flocking", []() {
        return std::make_unique<sep::workbench::FlockingDemo>();
    });

    manager.registerDemo("neural", []() {
        return std::make_unique<sep::workbench::NeuralDemo>();
    });

    manager.registerDemo("drug_discovery", []() {
        return std::make_unique<sep::workbench::DrugDiscoveryDemo>();
    });

    manager.registerDemo("digital_physics", []() {
        return std::make_unique<sep::workbench::DigitalPhysicsDemo>();
    });

    manager.registerDemo("memory_garden", []() {
        return std::make_unique<sep::workbench::MemoryGardenDemo>();
    });

    manager.switchToDemo("genesis");

    // Placeholder for the application loop
    // In the real application this would call manager.update(dt) and manager.render()
    return 0;
}
