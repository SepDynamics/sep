#pragma once

#include "apps/workbench/signal_generator/signal_types.h"
#include "engine/dag_graph.h"

namespace sep::workbench {

class SignalStrategy {
public:
    virtual ~SignalStrategy() = default;
    virtual SignalResult generateSignal(const dag::DagNode& pattern) = 0;
};

} // namespace sep::workbench
