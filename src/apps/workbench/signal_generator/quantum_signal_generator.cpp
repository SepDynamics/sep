#include "quantum_signal_generator.h"

namespace sep::workbench {

QuantumSignalGenerator::QuantumSignalGenerator() {
    // No default strategy - signals must come from real market data processing
    strategy_ = nullptr;
}

void QuantumSignalGenerator::setStrategy(std::unique_ptr<SignalStrategy> strategy) {
    strategy_ = std::move(strategy);
}

SignalResult QuantumSignalGenerator::generateSignal(const dag::DagNode& pattern) {
    if (strategy_) {
        return strategy_->generateSignal(pattern);
    }
    return {SignalType::HOLD, 0.0f, "No strategy set"};
}

void QuantumSignalGenerator::tick() {
    if (strategy_) {
        // Use empty DAG node since engine connection not available yet
        dag::DagNode empty_node;
        last_signal_ = strategy_->generateSignal(empty_node);
    } else {
        last_signal_ = {SignalType::HOLD, 0.0f, "No strategy set"};
    }
}

SignalResult QuantumSignalGenerator::getCurrentSignal() const {
    return last_signal_;
}

SignalStrategy* QuantumSignalGenerator::getStrategy() const {
    return strategy_.get();
}

} // namespace sep::workbench
