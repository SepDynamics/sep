#pragma once

#include "engine/dag_graph.h"
#include "apps/workbench/signal_generator/signal_strategy.h"
#include "apps/workbench/signal_generator/signal_types.h"

#include <memory>

namespace sep::workbench {

class QuantumSignalGenerator {
public:
    QuantumSignalGenerator();

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
};

} // namespace sep::workbench
