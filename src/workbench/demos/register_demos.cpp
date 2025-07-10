#include "../../workbench_demo_adapter.hpp"
#include "workbench/demos/audio_visualizer.hpp"
#include "workbench/demos/cosmo_demo.hpp"
#include "workbench/demos/cosmo_sim.hpp"
#include "workbench/demos/demo_manager.hpp"
#include "workbench/demos/digital_physics_demo.hpp"
#include "workbench/demos/drug_discovery_demo.hpp"
#include "workbench/demos/flocking_demo.hpp"
#include "workbench/demos/genesis_pattern.hpp"
#include "workbench/demos/memory_garden.hpp"
#include "workbench/demos/neural_demo.hpp"

namespace sep
{
    namespace workbench
    {
        void registerDemos()
        {
            auto& manager = DemoManager::getInstance();
            
            // Register all available demos with their factory functions
            manager.registerDemo("genesis", 
                [] { return std::make_unique<GenesisPatternDemo>(); });
            
            manager.registerDemo("neural", 
                [] { return std::make_unique<NeuralDemo>(); });
            
            manager.registerDemo("memory", 
                [] { return std::make_unique<MemoryGardenDemo>(); });
            
            manager.registerDemo("flocking", 
                [] { return std::make_unique<FlockingDemo>(); });
            
            manager.registerDemo("cosmo", 
                [] { return std::make_unique<CosmoDemo>(); });
            
            manager.registerDemo("cosmo_sim",
                [] { return std::make_unique<CosmoSim>(); });
            
            manager.registerDemo("physics", 
                [] { return std::make_unique<DigitalPhysicsDemo>(); });
            
            manager.registerDemo("drug", 
                [] { return std::make_unique<DrugDiscoveryDemo>(); });
            
            manager.registerDemo("audio", 
                [] { return std::make_unique<AudioVisualizerDemo>(); });
        }
    }  // namespace workbench
}  // namespace sep