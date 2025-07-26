# SEP Engine Development Roadmap

## Objective
Transform the SEP Engine into a production-ready trading platform by integrating components, **fixing critical compilation issues to enable chart rendering**, and achieving demo trading. This roadmap prioritizes testbed completion with visible charts, signal validation, and trading infrastructure, ensuring proper data routing.

## Development Phases
1. **Phase 1: Critical Build Fixes & Testbed Completion** (2-3 weeks)
   - Establish a stable testing environment with visible candlestick and metric charts.
2. **Phase 2: Trading Preparation** (4-6 weeks)
   - Validate signals and implement trading logic.
3. **Phase 3: Demo Trading & Optimization** (3-4 weeks)
   - Launch paper trading and optimize strategies.

## Phase 1: Critical Build Fixes & Testbed Completion

### 1.1 Critical Build Error Resolution
- **Status**: **FAILED**. Blocking core development.
- **Tasks**:
  - [X] Fix `error: invalid application of 'sizeof' to an incomplete type 'sep::workbench::CandleData'` in `src/engine/data_parser.cpp` and `src/engine/engine.cu`.
    - **Root Cause**: Circular dependency or missing include for `sep::workbench::CandleData` where `std::deque` needs the full type definition. Ensure `common_structs.h` is fully included before anything that uses `std::deque<CandleData>`.
    - **Action**: Move `common_structs.h` inclusion to the top of `data_parser.h` and `multi_timeframe_analyzer.h` and ensure it's fully defined before `deque` is instantiated. Alternatively, ensure `CandleData` is a complete type (not just a forward declaration) when used in template instantiations.
  - [X] Fix `error: function definition is not allowed here` in `src/engine/logging.cpp`.
    - **Root Cause**: Function definitions are likely placed outside the `sep::logging` namespace or within another function/class, which is syntactically incorrect.
    - **Action**: Review `logging.cpp` to ensure all function definitions are correctly scoped within `sep::logging` namespace and not nested improperly.
  - [X] Fix `error: no type named 'ServiceProxyEngine' in namespace 'sep::core'` in `src/apps/workbench/tabs/engine_tab_controller.h`.
    - **Root Cause**: `ServiceProxyEngine` is likely not correctly included or declared within the `sep::core` namespace before its use in `engine_tab_controller.h`.
    - **Action**: Add `#include "core/service_proxy_engine.h"` to `engine_tab_controller.h` and ensure `ServiceProxyEngine` is correctly defined in its header.
  - [X] Fix `error: use of undeclared identifier 'backtester'; did you mean 'workbench::backtester'?` in `src/apps/workbench/core/workbench_main.cpp` and `src/apps/workbench/core/service_proxy_engine.h`.
    - **Root Cause**: The `backtester` namespace (or the `Backtester` class directly) is not properly qualified or included. The compiler suggests `workbench::backtester` which indicates it's aware of the correct full path.
    - **Action**: Add `using namespace sep::workbench::backtester;` or fully qualify `sep::workbench::backtester::` where `BacktestResult` and `DataLoader` are used in `service_proxy_engine.h` and `workbench_main.cpp`. Ensure relevant headers are included.
- **Priority**: **CRITICAL (Blocking)**
- **Estimated Time**: 5 days (Includes debugging and re-testing)
- **Dependencies**: None (This is foundational)

### 1.2 48-Hour Sample Data Setup
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Fetch 48 hours of EUR/USD M1 data (`src/connectors/oanda_connector.cpp`).
  - [ ] Store in JSON (`src/engine/data_parser.cpp`) with integrity checks.
  - [ ] Implement DataLoader (`src/apps/workbench/backtester/data/data_loader.cpp`) for workbench.
- **Priority**: High
- **Estimated Time**: 3 days
- **Dependencies**: OandaConnector, DataParser, DataLoader, **1.1 Build Fixes**

### 1.3 Chart Rendering in Signals Tab
- **Status**: Partially implemented (`signals_tab_controller.cpp` renders basic charts but fails due to ImGui errors, now potentially fixed by 1.1).
- **Tasks**:
  - [X] Verify ImGui null pointer dereferences and other `cert-err33-c` warnings in `imgui.cpp`, `imgui_impl_opengl3.cpp`, `signals_tab_controller.cpp` are resolved. (These appear in the report, if not directly causing crashes, they are good practice to fix for stability).
  - [X] Replace floating-point loop counters (`cert-flp30-c`, `security.FloatLoopCounter`) in `imgui_demo.cpp` to avoid precision issues.
  - [ ] Render candlestick charts with SEP signal overlays in SignalsTabController.
  - [ ] Add real-time coherence, stability, entropy plots with 1-hour/4-hour rolling averages.
  - [ ] Implement zoom/pan functionality for charts.
- **Priority**: Critical
- **Estimated Time**: 5 days
- **Dependencies**: SignalsTabController, MetricsMonitor, **1.1 Build Fixes**

### 1.4 Pattern Discovery Framework
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Add threshold detection in PatternMetricEngine (`src/quantum/pattern_metric_engine.cpp`).
  - [ ] Test rules (e.g., `stability < 0.3 && entropy > 0.7`).
  - [ ] Create UI panel for threshold adjustments in SignalsTabController.
  - [ ] Log patterns (`src/engine/logging.cpp`) for chart visualization.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: PatternMetricEngine, SignalsTabController, **1.1 Build Fixes**

### 1.5 Correlation Analysis
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Implement metric-price correlation in MultiTimeframeAnalyzer (`src/apps/workbench/core/multi_timeframe_analyzer.cpp`).
  - [ ] Display coefficients as plots in EngineTabController (`src/apps/workbench/tabs/engine_tab_controller.cpp`).
  - [ ] Export correlation data (`src/engine/data_parser.cpp`).
- **Priority**: Medium
- **Estimated Time**: 3 days
- **Dependencies**: MultiTimeframeAnalyzer, EngineTabController, **1.1 Build Fixes**

## Phase 2: Trading Preparation

### 2.1 Backtesting Framework
- **Status**: Partially implemented (`src/apps/workbench/backtester/backtester.cpp`).
- **Tasks**:
  - [ ] Integrate PatternMetricEngine signals into BacktesterEngine (`src/apps/workbench/backtester/core/backtester_engine.cpp`).
  - [ ] Add Sharpe ratio, max drawdown in PerformanceMetrics (`src/apps/workbench/backtester/core/performance_metrics.cpp`).
  - [ ] Create backtesting UI in BackendTabController with chart visualizations.
  - [ ] Support 48-hour data input.
- **Priority**: High
- **Estimated Time**: 7 days
- **Dependencies**: BacktesterEngine, PatternMetricEngine, BackendTabController, **Phase 1 Complete**

### 2.2 Signal Validation System
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Build validator in ServiceProxyEngine (`src/apps/workbench/core/service_proxy_engine.cpp`).
  - [ ] Compare signals against price movements.
  - [ ] Generate reports (accuracy, false positives) with visual charts.
  - [ ] Display results in BackendTabController.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: ServiceProxyEngine, BackendTabController, **Phase 1 Complete**

### 2.3 Risk Management Module
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Implement position sizing in TradeManager (`src/apps/workbench/core/trade_manager.cpp`).
  - [ ] Add stop-loss and take-profit logic.
  - [ ] Enforce 2% max exposure per trade.
  - [ ] Create UI panel in BackendTabController.
- **Priority**: High
- **Estimated Time**: 6 days
- **Dependencies**: TradeManager, BackendTabController, **Phase 1 Complete**

### 2.4 Order Management System
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Integrate OANDA Trading API (`src/connectors/oanda_connector.cpp`).
  - [ ] Track order states (pending, filled, canceled).
  - [ ] Handle errors and retries.
  - [ ] Display order status in BackendTabController with charts.
- **Priority**: High
- **Estimated Time**: 7 days
- **Dependencies**: OandaConnector, TradeManager, BackendTabController, **Phase 1 Complete**

### 2.5 Demo Account Integration
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Configure OANDA demo credentials (`src/apps/workbench/config.cpp`).
  - [ ] Enable paper trading in TradeManager.
  - [ ] Test order placement in demo environment.
  - [ ] Show balance and P&L charts in BackendTabController.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: OandaConnector, TradeManager, BackendTabController, **Phase 1 Complete**

## Phase 3: Demo Trading & Optimization

### 3.1 Paper Trading
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Enable continuous paper trading with live data.
  - [ ] Log trades (`src/engine/logging.cpp`).
  - [ ] Monitor performance with real-time charts.
  - [ ] Display trade history in BackendTabController.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: TradeManager, BackendTabController, **Phase 2 Complete**

### 3.2 Performance Monitoring
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Add analytics (win/loss ratio, ROI) in TradeManager.
  - [ ] Create anomaly alerts (e.g., large drawdowns).
  - [ ] Display performance charts in BackendTabController.
  - [ ] Export trade logs.
- **Priority**: Medium
- **Estimated Time**: 4 days
- **Dependencies**: TradeManager, BackendTabController, **Phase 2 Complete**

### 3.3 Strategy Optimization
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Automate threshold optimization using backtest results.
  - [ ] Test ML models for metric-price relationships in ServiceProxyEngine.
  - [ ] Add tuning UI with charts in EngineTabController.
  - [ ] Validate strategies in paper trading.
- **Priority**: Medium
- **Estimated Time**: 7 days
- **Dependencies**: ServiceProxyEngine, EngineTabController, Backtester, **Phase 2 Complete**

## Technical Debt & Optimizations (Lower Priority)

### Performance Scaling
- **Issue**: Super-linear scaling in PatternMetricEngine (POC 4).
- **Tasks**:
  - [ ] Optimize data structures (`src/quantum/pattern_metric_engine.cpp`).
  - [ ] Implement sliding window for pattern history.
- **Priority**: Medium
- **Estimated Time**: 5 days
- **Dependencies**: PatternMetricEngine

### Event-Driven Architecture
- **Issue**: Tight component coupling.
- **Tasks**:
  - [ ] Implement EventBus (`src/apps/workbench/core/ui_layout_manager.cpp`).
  - [ ] Refactor interactions to use events.
- **Priority**: Medium
- **Estimated Time**: 6 days
- **Dependencies**: UILayoutManager

### API Rate Limiting
- **Issue**: Potential OANDA API disruptions.
- **Tasks**:
  - [ ] Add request batching in OandaConnector.
  - [ ] Implement caching (`src/connectors/market_data_converter.cpp`).
- **Priority**: Medium
- **Estimated Time**: 4 days
- **Dependencies**: OandaConnector

## Immediate Action Items (This Week)
1. **Fix All Critical Build Errors**: Prioritize 1.1 immediately.
2. **48-Hour Sample Data Setup**: Fetch EUR/USD M1 data and store in JSON with integrity checks.
3. **Chart Implementation**: Render candlestick charts and metric plots in SignalsTabController.

## Success Metrics
- **Testbed Completion**:
  - All critical build errors resolved.
  - 48 hours of clean data processed.
  - Candlestick and metric charts rendering at >10Hz.
  - 3+ reliable signal patterns identified.
  - Correlation coefficient >0.3.
- **Demo Trading Readiness**:
  - Backtested Sharpe ratio >1.0.
  - <5% max drawdown in paper trading.
  - 100 demo trades without errors.
  - Risk limits enforced.
--- END OF FILE TODO.md ---

--- START OF FILE WORK.md ---
## SEP Engine Overview and Development Plan (July 2025)

### Project Overview
The Self-Emergent Processor (SEP) Engine is a quantum-inspired trading platform processing OANDA market data to generate signals based on **coherence**, **stability**, and **entropy**. Using CUDA-accelerated kernels, it integrates with a 3-tab workbench dashboard (Signals, Engine, Backend) for visualization and trading. The current focus is on **resolving critical compilation issues that are blocking core functionality and chart rendering**, then completing the testbed, and enabling demo trading, as outlined in [TODO.md](TODO.md).

This document details component statuses, integration progress, and alignment with [DATA.md](DATA.md) and [GUI.md](GUI.md), using the snapshot (`sep_snapshot_20250725_204426.txt`) and `report.md`.

### Architecture Alignment
The pipeline in [DATA.md](DATA.md) is accurate: **OANDA Market Data → Quantum Pattern Engine → Workbench Dashboard → Signal Analysis → Trading Decisions**. The GUI ([GUI.md](GUI.md)) organizes tabs in `src/apps/workbench/tabs/`, but chart rendering is blocked by compilation errors. Threshold detection and trading logic need routing for demo trading.

### Component Status and Roadmap

#### 1. Market Data Ingestion Layer
- **Components**:
  - **OandaConnector** (`src/connectors/oanda_connector.cpp`): Fetches OHLC candles with 50ms rate limiting.
  - **DataParser** (`src/engine/data_parser.cpp`): Converts JSON to `Pattern` structs (`src/quantum/types.h`). **CURRENTLY FAILING TO COMPILE due to `CandleData` being an incomplete type.**
  - **StreamBuffer** (`src/engine/memory.cu`): Partial, lacks chunked streaming.
- **Status**: Functional, but `DataParser` is currently failing compilation.
- **TODO Integration**:
  - **48-Hour Sample Data Setup**: Fetch EUR/USD M1 data (Phase 1.2).
  - **StreamBuffer Enhancement**: Add chunked processing.
- **Estimated Completion**: 3 days (Phase 1.2, contingent on `DataParser` fix).

#### 2. SEP Quantum Engine
- **Components**:
  - **PatternMetricEngine** (`src/quantum/pattern_metric_engine.cpp`): Computes metrics via CUDA (`src/engine/pattern_kernels.cu`). Needs threshold detection.
  - **QBSA Processor** (`src/quantum/qbsa.cpp`): Quantum bit stream analysis.
  - **QFH Processor** (`src/quantum/quantum_processor_qfh.cpp`): Enhances pattern analysis.
  - **DAG Graph** (`src/engine/dag_graph.cpp`): Handles correlations, not fully integrated.
- **Status**: Algorithms validated (POCs 1-6), lacks signal generation. `engine.cu` (which is part of `sep_engine`) is **FAILING TO COMPILE** due to `CandleData` errors.
- **TODO Integration**:
  - **Pattern Discovery Framework**: Add threshold detection (Phase 1.4).
  - **Correlation Analysis**: Implement in MultiTimeframeAnalyzer (Phase 1.5).
  - **Performance Optimization**: Fix super-linear scaling (Technical Debt).
- **Estimated Completion**: 8 days (Phase 1.4, 1.5, Technical Debt, **contingent on engine.cu fix**).

#### 3. Real-time Processing
- **Components**:
  - **Pattern Evolution** (`src/quantum/pattern_evolution.cpp`): Mutation algorithms, partially connected to PatternMetricEngine.
  - **Metrics Computation** (`src/apps/workbench/core/metrics_monitor.cpp`): Real-time metrics, but chart integration is blocked.
  - **Threshold Detection** (`src/apps/workbench/core/service_proxy_engine.cpp`): Not implemented. `service_proxy_engine.cpp` is **FAILING TO COMPILE** due to undeclared identifiers `backtester` and `ServiceProxyEngine`.
- **Status**: MetricsMonitor outputs valid, but chart integration is blocked.
- **TODO Integration**:
  - **Chart Rendering**: Connect MetricsMonitor to SignalsTabController (Phase 1.3).
  - **Threshold Detection**: Implement in PatternMetricEngine (Phase 1.4).
- **Estimated Completion**: 10 days (Phase 1.3, 1.4, **contingent on `service_proxy_engine.cpp` fix**).

#### 4. Visualization Layer
- **Components**:
  - **Workbench Dashboard** (`src/apps/workbench/workbench_core.cpp`): ImGui-based, basic candlestick charts are failing to render due to core ImGui errors. `workbench_main.cpp` is **FAILING TO COMPILE** due to missing types and undeclared identifiers.
  - **SignalsTabController** (`src/apps/workbench/tabs/signals_tab_controller.cpp`): Partial charts, needs metric plots. Compilation issue also reported here (`signals_tab_controller.cpp`).
  - **EngineTabController** (`src/apps/workbench/tabs/engine_tab_controller.cpp`): Needs diagnostic charts. **FAILING TO COMPILE** due to `ServiceProxyEngine` issues.
  - **BackendTabController** (`src/apps/workbench/tabs/backend_tab_controller.cpp`): Lacks trading/backtesting UI.
- **Status**: Dashboard framework exists, chart rendering blocked by ImGui and core application compilation issues.
- **TODO Integration**:
  - **Chart Rendering**: Fix ImGui errors and render charts in SignalsTabController (Phase 1.3).
  - **Correlation Analysis**: Add plots to EngineTabController (Phase 1.5).
  - **Backtesting Suite, Trading Terminal**: Implement UI in BackendTabController (Phase 2.1, 2.4, 2.5).
- **Estimated Completion**: 12 days (Phase 1.3, 1.5, 2.1, 2.4, 2.5, **contingent on core workbench compilation fix**).

#### 5. Trading Decision Layer
- **Components**:
  - **Signal Generation** (`src/apps/workbench/core/service_proxy_engine.cpp`): Awaits threshold detection.
  - **Risk Management** (`src/apps/workbench/core/trade_manager.cpp`): Lacks risk logic.
  - **Order Placement** (`src/connectors/oanda_connector.cpp`): Supports data fetching, not trading API.
  - **Backtesting** (`src/apps/workbench/backtester/backtester.cpp`): Partial, per [backtesting_architecture.md](backtesting_architecture.md).
- **Status**: Conceptual, needs implementation. Blocked by upstream compilation errors.
- **TODO Integration**:
  - **Backtesting Framework**: Integrate signals and UI (Phase 2.1).
  - **Signal Validation System**: Implement in ServiceProxyEngine (Phase 2.2).
  - **Risk Management Module**: Add to TradeManager (Phase 2.3).
  - **Order Management System**: Extend OandaConnector (Phase 2.4).
  - **Demo Account Integration**: Configure paper trading (Phase 2.5).
- **Estimated Completion**: 25 days (Phase 2.1-2.5, **contingent on all upstream fixes**).

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
- **Build Errors** (`errors.txt` and `report.md`):
  - **CRITICAL**: `invalid application of 'sizeof' to an incomplete type 'sep::workbench::CandleData'` in `data_parser.cpp` and `engine.cu`. This is likely a missing or misplaced `#include "apps/workbench/core/common_structs.h"` or `common_structs.h` is itself problematic in the global context.
  - **CRITICAL**: `function definition is not allowed here` in `logging.cpp`. This indicates incorrect C++ syntax (e.g., defining a function inside another function or outside its namespace).
  - **CRITICAL**: `no type named 'ServiceProxyEngine'` and `use of undeclared identifier 'backtester'` in `workbench_main.cpp` and `service_proxy_engine.h`. These are straightforward include/namespace qualification issues.
- **Chart Rendering**: Still blocked.
- **Incomplete Routing**: MetricsMonitor outputs not connected to dashboard.
- **Missing Features**: Threshold detection, signal validation, trading infrastructure.

### Recommendations
1. **PRIORITY ONE: Fix all CRITICAL compilation errors.** This is paramount as the entire build is failing. Focus on:
    * **CandleData `sizeof` issues**: Ensure `common_structs.h` is included and that `CandleData` is a complete type definition where `std::deque` is instantiated.
    * **`logging.cpp` function definitions**: Correct the scope of these functions.
    * **`ServiceProxyEngine` and `backtester` type issues**: Add missing includes and correct namespace qualifications.
2. **Sequential Integration**: Once compilation is clean, strictly follow [TODO.md](TODO.md) for systematic component completion.
3. **Build Stability**: Continuously monitor and fix build warnings (e.g., `cert-err33-c`, `security.FloatLoopCounter`) post-critical error resolution to improve code quality and prevent future issues.
4. **Early Validation**: Start signal validation after pattern discovery.
5. **Code Reuse**: Use `pattern_metric_example.cpp` for threshold testing, `multi_timeframe_analyzer.cpp` for correlations.

### Success Metrics
- **Testbed**: All critical build errors resolved, 48-hour data processed, charts at >10Hz, 3+ patterns, correlation >0.3.
- **Demo Trading**: Sharpe ratio >1.0, <5% drawdown, 100 error-free trades, risk limits enforced.
- **Timeline**: Testbed in 2-3 weeks, demo trading in 6-9 weeks.

### Conclusion
The SEP Engine's core is robust, but **critical compilation issues are currently blocking all progress**. Fixing these build errors is the absolute top priority to enable basic functionality and proceed with chart rendering, which is essential for visualizing signals and metrics. Once resolved, the roadmap aims for demo trading in 9-12 weeks.