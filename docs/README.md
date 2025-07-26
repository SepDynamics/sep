# SEP Engine Trading Platform - Integration Phase

## Current Status: Workbench Integration & Pattern Analysis

The SEP Engine is a quantum-inspired trading platform that processes streaming financial market data from OANDA to generate predictive trading signals using three core metrics: **coherence** (pattern consistency), **stability** (pattern persistence), and **entropy** (data unpredictability). After verifying the quantum processing pipeline ([DATA.md](DATA.md)), the project is now focused on integrating components into the workbench dashboard and developing a robust pattern discovery framework to enable demo trading.

## Phase Objectives

### 1. **Workbench Integration** 🎯
- Display real-time SEP engine metrics (coherence, stability, entropy) in the dashboard.
- Visualize correlations between quantum metrics and market price movements.
- Implement 48-hour historical data analysis with the ability to pause live feeds.

### 2. **Pattern Discovery** 🔍
- Identify and validate threshold combinations for predictive signals (e.g., `stability < 0.3 && entropy > 0.7` for sell signals).
- Analyze rolling 24-hour contexts to ensure pattern consistency across timeframes.
- Log and visualize detected patterns for iterative refinement.

### 3. **Validation Framework** ✅
- Test the predictive accuracy of quantum metrics against historical price movements.
- Measure correlation coefficients between metric changes and market direction.
- Build confidence in signal reliability before enabling demo trading.

## Current Architecture

The verified data pipeline follows the flow outlined in [DATA.md](DATA.md):
**OANDA Market Data** → **Quantum Pattern Engine** → **Workbench Dashboard** → **Signal Analysis** → **Trading Decisions**

The workbench GUI is being refactored into a 3-tab architecture ([GUI.md](GUI.md)):
- **Signals Tab**: Real-time signal visualization and market analysis.
- **Engine Tab**: Quantum engine diagnostics and pattern analysis.
- **Backend Tab**: Trading operations, backtesting, and system administration.

## Key Components Verified ✅

- **OandaConnector** (`src/connectors/oanda_connector.cpp`): Authentic market data integration with rate limiting (50ms minimum).
- **PatternMetricEngine** (`src/quantum/pattern_metric_engine.cpp`): CUDA-accelerated quantum processing for coherence, stability, and entropy.
- **MetricsMonitor** (`src/apps/workbench/core/metrics_monitor.cpp`): Real-time metric computation.
- **DataParser** (`src/engine/data_parser.cpp`): Converts OANDA JSON OHLC to quantum patterns.

## Next Milestones

1. **48-Hour Data Sample**: Create a controlled EUR/USD M1 dataset for analysis.
2. **Dashboard Integration**: Render real-time metrics and signals in the Signals Tab.
3. **Threshold Detection**: Implement visual indicators for pattern threshold crossings.
4. **Predictive Testing**: Validate signal accuracy with backtesting and correlation analysis.
5. **Demo Trading**: Enable paper trading with OANDA demo account integration.

## Build & Run

```bash
./build.sh                    # Build with verified components
./run_workbench.sh            # Launch integrated dashboard