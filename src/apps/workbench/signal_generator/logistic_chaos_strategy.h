#pragma once

#include "apps/workbench/signal_generator/signal_strategy.h"

namespace sep::workbench {

class LogisticChaosStrategy : public SignalStrategy {
public:
    SignalResult generateSignal(const dag::DagNode& pattern) override;

    void setR(float r);

private:
    float r_ = 3.9f;
};

} // namespace sep::workbench
