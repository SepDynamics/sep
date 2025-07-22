
#include "audio_visualizer.hpp"
#include "cosmo_demo.hpp"
#include "cosmo_sim.hpp"
#include "demo_manager.hpp"
#include "digital_physics_demo.hpp"
#include "drug_discovery_demo.hpp"
#include "flocking_demo.hpp"
#include "genesis_pattern.hpp"
#include "memory_garden.hpp"
#include "neural_demo.hpp"

namespace sep::workbench
{
    void registerDemos()
    {
        auto& manager = DemoManager::getInstance();

        // Register all available demos with their factory functions
        manager.registerDemo("genesis",
                             [] { return std::make_unique<sep::workbench::GenesisPatternDemo>(); });

        manager.registerDemo("neural",
                             [] { return std::make_unique<sep::workbench::NeuralDemo>(); });

        manager.registerDemo("memory",
                             [] { return std::make_unique<sep::workbench::MemoryGardenDemo>(); });

        manager.registerDemo("flocking",
                             [] { return std::make_unique<sep::workbench::FlockingDemo>(); });

        manager.registerDemo("cosmo", [] { return std::make_unique<sep::workbench::CosmoDemo>(); });

        manager.registerDemo("cosmo_sim",
                             [] { return std::make_unique<sep::workbench::CosmoSim>(); });

        manager.registerDemo("physics",
                             [] { return std::make_unique<sep::workbench::DigitalPhysicsDemo>(); });

        manager.registerDemo("drug",
                             [] { return std::make_unique<sep::workbench::DrugDiscoveryDemo>(); });

        manager.registerDemo(
            "audio", [] { return std::make_unique<sep::workbench::AudioVisualizerDemo>(); });
    }
}  // namespace sep::workbench
