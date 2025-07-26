#pragma once

#include "engine/dag_graph.h"
#include "apps/workbench/signal_generator/signal_strategy.h"
#include "apps/workbench/signal_generator/signal_types.h"
#include "memory/memory_tier_manager.hpp"

#include <memory>

namespace sep::workbench {

class QuantumSignalGenerator {
public:
    QuantumSignalGenerator();

    // Supply MemoryTierManager for DAG access
    void setMemoryManager(sep::memory::MemoryTierManager* manager);

    // Set the active signal generation strategy
    void setStrategy(std::unique_ptr<SignalStrategy> strategy);

    // Generate a signal from a DagNode
    SignalResult generateSignal(const dag::DagNode& pattern);

    // Tick the simulation
    void tick();

    // Get the current signal
    SignalResult getCurrentSignal() const;

    // Get the current strategy
    SignalStrategy* getStrategy() const;

private:
    std::unique_ptr<SignalStrategy> strategy_;
    SignalResult last_signal_;
    sep::memory::MemoryTierManager* memory_manager_{nullptr};
};

} // namespace sep::workbench
