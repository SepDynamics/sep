# SEP Engine Trading Platform - Integration Phase

## Current Status: Workbench Integration & Chart Rendering

The SEP Engine is a quantum-inspired trading platform that processes streaming OANDA market data to generate predictive trading signals based on **coherence** (pattern consistency), **stability** (pattern persistence), and **entropy** (data unpredictability). Following pipeline verification ([DATA.md](DATA.md)), the focus is on integrating components into the workbench dashboard with a 3-tab architecture ([GUI.md](GUI.md)), prioritizing chart rendering to visualize signals, metrics, and market data, and enabling demo trading. Compilation issues, particularly ImGui-related errors (`report.md`), are being addressed to ensure charts are rendered correctly.

## Phase Objectives

### 1. **Workbench Integration & Chart Rendering** 📊
- Render real-time candlestick charts with SEP signal overlays in the Signals Tab.
- Display coherence, stability, and entropy metrics in interactive ImGui plots across all tabs.
- Enable 48-hour historical data analysis with pause/resume functionality for live feeds.
- Resolve build errors (`cert-err33-c`, `security.FloatLoopCounter`) to ensure chart visibility.

### 2. **Pattern Discovery** 🔍
- Implement and validate threshold combinations for predictive signals (e.g., `stability < 0.3 && entropy > 0.7` for sell signals).
- Analyze 24-hour rolling contexts for pattern consistency.
- Log and visualize detected patterns in the Signals and Engine Tabs.

### 3. **Validation Framework** ✅
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

## Key Components Verified ✅

- **OandaConnector** (`src/connectors/oanda_connector.cpp`): Fetches real-time and historical OHLC candles with 50ms rate limiting.
- **PatternMetricEngine** (`src/quantum/pattern_metric_engine.cpp`): CUDA-accelerated metric computation (coherence, stability, entropy).
- **MetricsMonitor** (`src/apps/workbench/core/metrics_monitor.cpp`): Real-time metric generation, awaiting dashboard integration.
- **DataParser** (`src/engine/data_parser.cpp`): Converts OANDA JSON to quantum patterns.

## Next Milestones

1. **Chart Rendering**: Implement robust candlestick and metric charts in Signals Tab.
2. **48-Hour Data Sample**: Create EUR/USD M1 dataset for testing and visualization.
3. **Dashboard Integration**: Connect metrics and signals to all tabs with interactive plots.
4. **Threshold Detection**: Add visual indicators for pattern crossings in Signals Tab.
5. **Demo Trading**: Enable paper trading with OANDA demo account.

## Build & Run

```bash
./build.sh                    # Build with verified components
./run_workbench.sh            # Launch integrated dashboard
```

## Key Files

- [`DATA.md`](DATA.md): Pipeline and data flow architecture.
- [`GUI.md`](GUI.md): Workbench GUI architecture and visual layout.
- [`TODO.md`](TODO.md): Detailed development roadmap.
- [`WORK.md`](WORK.md): Component status and integration details.
- [`backtesting_architecture.md`](backtesting_architecture.md): Backtesting framework design.
- [`../src/apps/workbench/`](../src/apps/workbench/): Dashboard and tab code.
- [`../examples/pattern_metric_example.cpp`](../examples/pattern_metric_example.cpp): Metric analysis example.

## Build Issues
- **Static Analysis** (`report.md`):
  - **High-Priority**: Null pointer dereferences in `imgui.cpp` (e.g., `core.NullDereference` at lines 6168, 6307).
  - **Medium-Priority**: Ignored return values (`cert-err33-c`) in `imgui_impl_opengl3.cpp`, `signals_tab_controller.cpp`.
  - **Low-Priority**: Floating-point loop counters (`security.FloatLoopCounter`) in `imgui_demo.cpp`.
- **Plan**: Prioritize null pointer fixes, cast return values to `void`, and address floating-point issues to ensure chart rendering stability.