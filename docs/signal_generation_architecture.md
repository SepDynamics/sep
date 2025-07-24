# Testable Signal Generation Strategy Framework Architecture

## 1. Overview

This document outlines the architecture for a testable signal generation framework integrated into the SEP Engine. The framework is designed to leverage the existing quantum processing capabilities of the engine and provide a real-time interface for testing and validating signal generation strategies.

## 2. Component Design

### 2.1. `QuantumSignalGenerator`

The `QuantumSignalGenerator` is a new component responsible for converting quantum patterns from the `DagGraph` into trading signals. It will be located in `src/workbench/signal_generator/`.

**Class Structure:**

```cpp
// src/workbench/signal_generator/quantum_signal_generator.h

#pragma once

#include "engine/dag_graph.h"
#include "signal_strategy.h"

namespace sep::workbench {

class QuantumSignalGenerator {
public:
    QuantumSignalGenerator();

    // Set the active signal generation strategy
    void setStrategy(std::unique_ptr<SignalStrategy> strategy);

    // Generate a signal from a DagNode
    SignalResult generateSignal(const dag::DagNode& pattern);

private:
    std::unique_ptr<SignalStrategy> strategy_;
};

} // namespace sep::workbench
```

### 2.2. `SignalStrategy`

The `SignalStrategy` is an interface that defines the logic for a specific signal generation strategy. This allows for different strategies to be implemented and tested.

**Class Structure:**

```cpp
// src/workbench/signal_generator/signal_strategy.h

#pragma once

#include "engine/dag_graph.h"

namespace sep::workbench {

enum class SignalType { BUY, SELL, HOLD };

struct SignalResult {
    SignalType type;
    float confidence;
    std::string reason;
};

class SignalStrategy {
public:
    virtual ~SignalStrategy() = default;
    virtual SignalResult generateSignal(const dag::DagNode& pattern) = 0;
};

} // namespace sep::workbench
```

### 2.3. `LogisticChaosStrategy`

The `LogisticChaosStrategy` is a concrete implementation of the `SignalStrategy` interface, using the logistic chaos mathematics from the forex demo.

**Class Structure:**

```cpp
// src/workbench/signal_generator/logistic_chaos_strategy.h

#pragma once

#include "signal_strategy.h"

namespace sep::workbench {

class LogisticChaosStrategy : public SignalStrategy {
public:
    SignalResult generateSignal(const dag::DagNode& pattern) override;
};

} // namespace sep::workbench
```

### 2.4. `SignalTestingPanel`

The `SignalTestingPanel` is a new ImGui panel in the workbench for real-time visualization and testing of signal generation strategies.

**Class Structure:**

```cpp
// src/workbench/panels/signal_testing_panel.h

#pragma once

#include "workbench/core/demo_panel.h"
#include "workbench/signal_generator/quantum_signal_generator.h"

namespace sep::workbench {

class SignalTestingPanel : public DemoPanel {
public:
    SignalTestingPanel(QuantumSignalGenerator* signal_generator);

    void render() override;

private:
    void renderSignalChart();
    void renderStrategyConfig();
    void renderBacktestResults();
    void renderLiveTesting();

    QuantumSignalGenerator* signal_generator_;
};

} // namespace sep::workbench
```

## 3. Data Flow

The data flow for the signal generation process is as follows:

```
Market Data -> DataParser -> Quantum Processor -> DAG Graph -> QuantumSignalGenerator -> SignalTestingPanel
      |             |                 |                  |                  |                      |
    CANDLE      Patterns          Coherence          Metrics            BUY/SELL/HOLD          Visuals
```

1.  **Market Data**: Raw market data (e.g., CANDLE format) is fed into the `DataParser`.
2.  **DataParser**: The `DataParser` converts the raw data into quantum patterns.
3.  **Quantum Processor**: The `Quantum Processor` evolves the patterns and calculates coherence.
4.  **DAG Graph**: The `DagGraph` stores the patterns and their relationships, and calculates metrics such as `tail_risk`, `alpha`, and `correlation`.
5.  **QuantumSignalGenerator**: The `QuantumSignalGenerator` uses a `SignalStrategy` to convert the `DagNode` metrics into trading signals.
6.  **SignalTestingPanel**: The `SignalTestingPanel` visualizes the generated signals and allows for real-time testing and configuration of strategies.

## 4. Integration with Existing Components

*   **`DagGraph`**: The `QuantumSignalGenerator` will read `DagNode` data from the `DagGraph`, including the existing `tail_risk`, `alpha`, and `correlation` metrics.
*   **`WorkbenchEngine`**: The `SignalTestingPanel` will be integrated into the `WorkbenchEngine`'s main loop for rendering.
*   **ImGui**: The `SignalTestingPanel` will use ImGui for its user interface.

## 5. Testing Framework

*   **Real-time Testing**: The `SignalTestingPanel` will provide a real-time view of generated signals, allowing for immediate feedback on strategy performance.
*   **Backtesting**: The framework will be designed to support backtesting by feeding historical data through the pipeline. The `SignalTestingPanel` will include a view for displaying backtesting results.
*   **Strategy Switching**: The `QuantumSignalGenerator`'s `setStrategy` method will allow for easy switching between different signal generation strategies for comparison.

## 6. Strategy Parameter Configuration

The `SignalTestingPanel` will provide a UI for configuring the parameters of the active `SignalStrategy`. This will allow for real-time tuning of parameters such as coherence thresholds, risk limits, and position sizing. The configuration will be serialized to and from a JSON file.