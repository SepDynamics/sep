## SEP Engine Overview and Development Plan (July 2025)

### Project Overview
The Self-Emergent Processor (SEP) Engine is a quantum-inspired trading platform processing OANDA market data to generate signals based on **coherence**, **stability**, and **entropy**. Using CUDA-accelerated kernels, it integrates with a 3-tab workbench dashboard (Signals, Engine, Backend) for visualization and trading. The focus is on resolving compilation issues to render charts, completing the testbed, and enabling demo trading, as outlined in [TODO.md](TODO.md).

This document details component statuses, integration progress, and alignment with [DATA.md](DATA.md) and [GUI.md](GUI.md), using the snapshot (`sep_snapshot_20250725_204426.txt`) and `report.md`.

### Architecture Alignment
The pipeline in [DATA.md](DATA.md) is accurate: **OANDA Market Data → Quantum Pattern Engine → Workbench Dashboard → Signal Analysis → Trading Decisions**. The GUI ([GUI.md](GUI.md)) organizes tabs in `src/apps/workbench/tabs/`, but chart rendering is blocked by ImGui errors. Threshold detection and trading logic need routing for demo trading.

### Component Status and Roadmap

#### 1. Market Data Ingestion Layer
- **Components**:
  - **OandaConnector** (`src/connectors/oanda_connector.cpp`): Fetches OHLC candles with 50ms rate limiting.
  - **DataParser** (`src/engine/data_parser.cpp`): Converts JSON to `Pattern` structs (`src/quantum/types.h`). Needs integrity checks.
  - **StreamBuffer** (`src/engine/memory.cu`): Partial, lacks chunked streaming.
- **Status**: Functional, missing 48-hour data setup.
- **TODO Integration**:
  - **48-Hour Sample Data Setup**: Fetch EUR/USD M1 data (Phase 1.1).
  - **StreamBuffer Enhancement**: Add chunked processing.
- **Estimated Completion**: 3 days (Phase 1.1).

#### 2. SEP Quantum Engine
- **Components**:
  - **PatternMetricEngine** (`src/quantum/pattern_metric_engine.cpp`): Computes metrics via CUDA (`src/engine/pattern_kernels.cu`). Needs threshold detection.
  - **QBSA Processor** (`src/quantum/qbsa.cpp`): Quantum bit stream analysis.
  - **QFH Processor** (`src/quantum/quantum_processor_qfh.cpp`): Enhances pattern analysis.
  - **DAG Graph** (`src/engine/dag_graph.cpp`): Handles correlations, not fully integrated.
- **Status**: Algorithms validated (POCs 1-6), lacks signal generation.
- **TODO Integration**:
  - **Pattern Discovery Framework**: Add threshold detection (Phase 1.3).
  - **Correlation Analysis**: Implement in MultiTimeframeAnalyzer (Phase 1.4).
  - **Performance Optimization**: Fix super-linear scaling (Technical Debt).
- **Estimated Completion**: 8 days (Phase 1.3, 1.4, Technical Debt).

#### 3. Real-time Processing
- **Components**:
  - **Pattern Evolution** (`src/quantum/pattern_evolution.cpp`): Mutation algorithms, partially connected to PatternMetricEngine.
  - **Metrics Computation** (`src/apps/workbench/core/metrics_monitor.cpp`): Real-time metrics, not integrated with charts.
  - **Threshold Detection** (`src/apps/workbench/core/service_proxy_engine.cpp`): Not implemented.
- **Status**: MetricsMonitor outputs valid, but chart integration is blocked.
- **TODO Integration**:
  - **Chart Rendering**: Connect MetricsMonitor to SignalsTabController (Phase 1.2).
  - **Threshold Detection**: Implement in PatternMetricEngine (Phase 1.3).
- **Estimated Completion**: 10 days (Phase 1.2, 1.3).

#### 4. Visualization Layer
- **Components**:
  - **Workbench Dashboard** (`src/apps/workbench/workbench_core.cpp`): ImGui-based, basic candlestick charts failing to render.
  - **SignalsTabController** (`src/apps/workbench/tabs/signals_tab_controller.cpp`): Partial charts, needs metric plots.
  - **EngineTabController** (`src/apps/workbench/tabs/engine_tab_controller.cpp`): Needs diagnostic charts.
  - **BackendTabController** (`src/apps/workbench/tabs/backend_tab_controller.cpp`): Lacks trading/backtesting UI.
- **Status**: Dashboard framework exists, chart rendering blocked by ImGui issues.
- **TODO Integration**:
  - **Chart Rendering**: Fix ImGui errors and render charts in SignalsTabController (Phase 1.2).
  - **Correlation Analysis**: Add plots to EngineTabController (Phase 1.4).
  - **Backtesting Suite, Trading Terminal**: Implement UI in BackendTabController (Phase 2.1, 2.4, 2.5).
- **Estimated Completion**: 12 days (Phase 1.2, 1.4, 2.1, 2.4, 2.5).

#### 5. Trading Decision Layer
- **Components**:
  - **Signal Generation** (`src/apps/workbench/core/service_proxy_engine.cpp`): Awaits threshold detection.
  - **Risk Management** (`src/apps/workbench/core/trade_manager.cpp`): Lacks risk logic.
  - **Order Placement** (`src/connectors/oanda_connector.cpp`): Supports data fetching, not trading API.
  - **Backtesting** (`src/apps/workbench/backtester/backtester.cpp`): Partial, per [backtesting_architecture.md](backtesting_architecture.md).
- **Status**: Conceptual, needs implementation.
- **TODO Integration**:
  - **Backtesting Framework**: Integrate signals and UI (Phase 2.1).
  - **Signal Validation System**: Implement in ServiceProxyEngine (Phase 2.2).
  - **Risk Management Module**: Add to TradeManager (Phase 2.3).
  - **Order Management System**: Extend OandaConnector (Phase 2.4).
  - **Demo Account Integration**: Configure paper trading (Phase 2.5).
- **Estimated Completion**: 25 days (Phase 2.1-2.5).

#### 6. Demo Trading & Optimization
- **Components**:
  - **Paper Trading** (`src/apps/workbench/core/trade_manager.cpp`): Not implemented.
  - **Performance Monitoring** (`src/apps/workbench/core/trade_manager.cpp`): Not implemented.
  - **Strategy Optimization** (`src/apps/workbench/core/service_proxy_engine.cpp`): Not implemented.
- **Status**: Awaits Phase 2.
- **TODO Integration**:
  - **Paper Trading**: Enable with live data (Phase 3.1).
  - **Performance Monitoring**: Add analytics (Phase 3.2).
  - **Strategy Optimization**: Implement tuning (Phase 3.3).
- **Estimated Completion**: 16 days (Phase 3.1-3.3).

### Current Implementation Gaps
- **Build Errors** (`report.md`):
  - **High-Priority**: Null pointer dereferences in `imgui.cpp` (lines 6168, 6307, 11627, 12216).
  - **Medium-Priority**: `cert-err33-c` in `imgui_impl_opengl3.cpp`, `signals_tab_controller.cpp`.
  - **Fix Plan**: Prioritize null pointer fixes, cast returns to `void` (3 days).
- **Chart Rendering**: Blocked by ImGui errors, preventing candlestick and metric visualization.
- **Incomplete Routing**: MetricsMonitor outputs not connected to dashboard.
- **Missing Features**: Threshold detection, signal validation, trading infrastructure.

### Recommendations
1. **Chart Rendering Priority**: Fix ImGui errors to enable candlestick and metric charts.
2. **Sequential Integration**: Follow [TODO.md](TODO.md) for systematic component completion.
3. **Build Stability**: Address high-priority errors post-chart rendering.
4. **Early Validation**: Start signal validation after pattern discovery.
5. **Code Reuse**: Use `pattern_metric_example.cpp` for threshold testing, `multi_timeframe_analyzer.cpp` for correlations.

### Success Metrics
- **Testbed**: 48-hour data processed, charts at >10Hz, 3+ patterns, correlation >0.3.
- **Demo Trading**: Sharpe ratio >1.0, <5% drawdown, 100 error-free trades, risk limits enforced.
- **Timeline**: Testbed in 2-3 weeks, demo trading in 6-9 weeks.

### Conclusion
The SEP Engine's core is robust, but chart rendering issues and incomplete routing hinder progress. Fixing ImGui errors and integrating charts are critical to visualize signals and metrics, paving the way for demo trading in 9-12 weeks.