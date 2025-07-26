# SEP Engine Trading Platform - Integration Phase

## Current Status: Workbench Integration & Chart Rendering - **Compilation Blockers Identified**

The SEP Engine is a quantum-inspired trading platform designed to process streaming OANDA market data, generating predictive signals based on **coherence**, **stability**, and **entropy**. The project is currently in a critical phase of integrating core components into a 3-tab workbench dashboard ([GUI.md](GUI.md)). However, **severe compilation errors are preventing the application from building and launching, directly impacting chart rendering and core engine functionality.** Resolving these build issues is the absolute top priority.

## Phase Objectives

### 1. **Resolve Critical Build Errors** ✅
- **TOP PRIORITY**: Address `fatal error: 'imgui.h' file not found` and related "incomplete type" issues, which are symptoms of GUI headers bleeding into core libraries.
- Fix namespace and undeclared identifier errors (`sep::workbench::Config`, `backtester`).
- Resolve missing `<cstdint>` includes.

### 2. **Workbench Integration & Chart Rendering** 📊
- Render real-time candlestick charts with SEP signal overlays in the Signals Tab.
- Display coherence, stability, and entropy metrics in interactive ImGui plots across all tabs.
- Enable 48-hour historical data analysis with pause/resume functionality for live feeds.

### 3. **Pattern Discovery** 🔍
- Implement and validate threshold combinations for predictive signals (e.g., `stability < 0.3 && entropy > 0.7` for sell signals).
- Analyze 24-hour rolling contexts for pattern consistency.
- Log and visualize detected patterns in the Signals and Engine Tabs.

### 4. **Validation Framework** ✅
- Test signal accuracy against historical price movements using the backtesting framework.
- Measure correlations between metrics and market direction in the Engine Tab.
- Build confidence in signal reliability before demo trading.

## Current Architecture

The data pipeline, detailed in [DATA.md](DATA.md), is verified:
**OANDA Market Data → Quantum Pattern Engine → Workbench Dashboard → Signal Analysis → Trading Decisions**

The workbench GUI ([GUI.md](GUI.md)) is being refactored into three tabs:
- **Signals Tab**: Candlestick charts, signal overlays, and metric plots for market analysis.
- **Engine Tab**: Quantum diagnostics with metric graphs and pattern analysis tools.
- **Backend Tab**: Trading terminal, backtesting interface, and system monitoring.

## Key Components Status

-   **OandaConnector** (`src/connectors/oanda_connector.cpp`): Fetches real-time and historical OHLC candles with 50ms rate limiting. **FUNCTIONAL, but indirectly affected by `imgui.h` include issue.**
-   **PatternMetricEngine** (`src/quantum/pattern_metric_engine.cpp`): CUDA-accelerated metric computation (coherence, stability, entropy). **FUNCTIONAL, but blocked from building by `engine.cu` which depends on `common_structs.h`.**
-   **MetricsMonitor** (`src/apps/workbench/core/metrics_monitor.cpp`): Real-time metric generation. **FUNCTIONAL, but UI integration blocked.**
-   **DataParser** (`src/engine/data_parser.cpp`): Converts OANDA JSON to quantum patterns. **FUNCTIONAL, but blocked from building by `common_structs.h` dependency.**
-   **ServiceConnector** (`src/apps/workbench/core/service_connector.cpp`): Manages connection to SEP API/local engine. **COMPILATION BLOCKED by `sep::workbench::Config` access issue.**
-   **WorkbenchEngine** (`src/apps/workbench/core/workbench_core.cpp`): Main application loop and GUI manager. **COMPILATION BLOCKED by multiple type/include issues.**

## Next Milestones (Contingent on Build Fixes)

1.  **Fix All Critical Build Errors**: Enable successful compilation of the entire project. This is the **single most important blocking item**.
2.  **Chart Rendering**: Implement robust candlestick and metric charts in Signals Tab.
3.  **48-Hour Data Sample**: Create EUR/USD M1 dataset for testing and visualization.
4.  **Dashboard Integration**: Connect metrics and signals to all tabs with interactive plots.
5.  **Threshold Detection**: Add visual indicators for pattern crossings in Signals Tab.
6.  **Demo Trading**: Enable paper trading with OANDA demo account.

## Build & Run

```bash
./build.sh                    # Build with verified components (currently fails)
./run_workbench.sh            # Launch integrated dashboard (currently fails)
```

## Key Files

-   [`DATA.md`](DATA.md): Pipeline and data flow architecture.
-   [`GUI.md`](GUI.md): Workbench GUI architecture and visual layout.
-   [`TODO.md`](TODO.md): Detailed development roadmap.
-   [`WORK.md`](WORK.md): Component status and integration details.
-   [`backtesting_architecture.md`](backtesting_architecture.md): Backtesting framework design.
-   [`../src/apps/workbench/`](../src/apps/workbench/): Dashboard and tab code.
-   [`../examples/pattern_metric_example.cpp`](../examples/pattern_metric_example.cpp): Metric analysis example.
