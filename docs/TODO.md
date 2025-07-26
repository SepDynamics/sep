# SEP Engine Development Roadmap

## Objective
Transform the SEP Engine into a production-ready trading platform by integrating components, **fixing critical compilation issues to enable chart rendering**, and achieving demo trading. This roadmap prioritizes testbed completion with visible charts, signal validation, and trading infrastructure, ensuring proper data routing.

## Development Phases
1.  **Phase 1: Critical Build Fixes & Testbed Completion** (2-3 weeks)
    *   Establish a stable testing environment with visible candlestick and metric charts.
2.  **Phase 2: Trading Preparation** (4-6 weeks)
    *   Validate signals and implement trading logic.
3.  **Phase 3: Demo Trading & Optimization** (3-4 weeks)
    *   Launch paper trading and optimize strategies.

## Phase 1: Critical Build Fixes & Testbed Completion

### 1.1 Critical Build Error Resolution
-   **Status**: **BLOCKED**. Current compilation status is critical due to:
    *   `fatal error: 'imgui.h' file not found` in core libraries like `engine.cu` and `data_parser.cpp` (due to `common_structs.h` dependency).
    *   `error: no member named 'Config' in namespace 'sep::workbench'` in `service_connector.cpp`.
    *   `error: function definition is not allowed here` in `logging.cpp`.
    *   `error: use of undeclared identifier 'backtester'` in `workbench_main.cpp` and `service_proxy_engine.h`.
    *   `use of undeclared identifier 'uint16_t'` / `uint32_t` in `emitterutils.cpp`.
-   **Tasks**:
    *   [ ] **Refactor Common Structs**: Move core data structures like `CandleData`, `TickData`, `OrderBookLevel`, `OrderBook`, `PendingOrder`, `TradeHistory` from `src/apps/workbench/core/common_structs.h` to a new, neutral header (e.g., `src/common/financial_data_types.h`).
    *   [ ] **Remove ImGui from Core**: Ensure `src/apps/workbench/core/common_structs.h` no longer includes `imgui.h` and is stripped down to only GUI-specific structs or removed if no common GUI structs remain. Update all files that previously included `common_structs.h` to instead include `financial_data_types.h` or the new specific GUI structs.
    *   [ ] **Correct ConfigManager Access**: In `src/apps/workbench/core/service_connector.cpp`, change `workbench::Config::getInstance()` to `sep::config::ConfigManager::getInstance()`.
    *   [ ] **Fix Logging Function Scope**: Review `src/engine/logging.cpp` to ensure all function definitions (`logPatternDetected`, `logSignalDetected`, `logTrade`, `logAnomaly`) are correctly scoped within the `sep::logging` namespace and not improperly nested.
    *   [ ] **Resolve Undeclared Identifiers**: Add necessary `#include` statements (`#include "apps/workbench/core/service_proxy_engine.h"`, `#include "apps/workbench/backtester/backtester.h"`, `#include "apps/workbench/backtester/data/data_loader.h"`) and qualify namespaces (`sep::core::ServiceProxyEngine`, `sep::workbench::backtester::BacktestResult`, `sep::workbench::backtester::DataLoader`) where these types are used in `src/apps/workbench/core/workbench_main.cpp` and `src/apps/workbench/tabs/engine_tab_controller.h`.
    *   [ ] **Add Missing Cstdint Include**: In `src/build/_deps/yaml-cpp-src/src/emitterutils.cpp`, add `#include <cstdint>`.
-   **Priority**: **CRITICAL (Blocking)**
-   **Estimated Time**: 5-7 days (Debugging complex interdependencies)
-   **Dependencies**: None (Foundational)

### 1.2 48-Hour Sample Data Setup
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Fetch 48 hours of EUR/USD M1 data (`src/connectors/oanda_connector.cpp`).
    *   [ ] Store in JSON (`src/engine/data_parser.cpp`) with integrity checks.
    *   [ ] Implement DataLoader (`src/apps/workbench/backtester/data/data_loader.cpp`) for workbench.
-   **Priority**: High
-   **Estimated Time**: 3 days
-   **Dependencies**: OandaConnector, DataParser, DataLoader, **Phase 1.1 Complete**

### 1.3 Chart Rendering in Signals Tab
-   **Status**: Partially implemented (`signals_tab_controller.cpp` has chart rendering logic, but current build errors prevent execution).
-   **Tasks**:
    *   [ ] Render candlestick charts with SEP signal overlays in `SignalsTabController`.
    *   [ ] Add real-time coherence, stability, and entropy plots with 1-hour/4-hour rolling averages.
    *   [ ] Implement zoom/pan functionality for charts.
-   **Priority**: Critical
-   **Estimated Time**: 5 days
-   **Dependencies**: SignalsTabController, MetricsMonitor, **Phase 1.1 Complete**

### 1.4 Pattern Discovery Framework
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Add threshold detection in PatternMetricEngine (`src/quantum/pattern_metric_engine.cpp`).
    *   [ ] Implement rules for signal generation (e.g., `stability < 0.3 && entropy > 0.7` for sell signals).
    *   [ ] Create UI panel for threshold adjustments in SignalsTabController.
    *   [ ] Log patterns (`src/engine/logging.cpp`) for chart visualization.
-   **Priority**: High
-   **Estimated Time**: 5 days
-   **Dependencies**: PatternMetricEngine, SignalsTabController, **Phase 1.1 Complete**

### 1.5 Correlation Analysis
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Implement metric-price correlation in MultiTimeframeAnalyzer (`src/apps/workbench/core/multi_timeframe_analyzer.cpp`).
    *   [ ] Display coefficients as plots in EngineTabController (`src/apps/workbench/tabs/engine_tab_controller.cpp`).
    *   [ ] Export correlation data (`src/engine/data_parser.cpp`).
-   **Priority**: Medium
-   **Estimated Time**: 3 days
-   **Dependencies**: MultiTimeframeAnalyzer, EngineTabController, **Phase 1.1 Complete**

## Phase 2: Trading Preparation

### 2.1 Backtesting Framework
-   **Status**: Partially implemented (`src/apps/workbench/backtester/backtester.cpp`).
-   **Tasks**:
    *   [ ] Integrate PatternMetricEngine signals into BacktesterEngine (`src/apps/workbench/backtester/core/backtester_engine.cpp`).
    *   [ ] Add Sharpe ratio, max drawdown in PerformanceMetrics (`src/apps/workbench/backtester/core/performance_metrics.cpp`).
    *   [ ] Create backtesting UI in BackendTabController with chart visualizations.
    *   [ ] Support 48-hour data input.
-   **Priority**: High
-   **Estimated Time**: 7 days
-   **Dependencies**: BacktesterEngine, PatternMetricEngine, BackendTabController, **Phase 1 Complete**

### 2.2 Signal Validation System
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Build validator in ServiceProxyEngine (`src/apps/workbench/core/service_proxy_engine.cpp`).
    *   [ ] Compare signals against price movements.
    *   [ ] Generate reports (accuracy, false positives) with visual charts.
    *   [ ] Display results in BackendTabController.
-   **Priority**: High
-   **Estimated Time**: 5 days
-   **Dependencies**: ServiceProxyEngine, BackendTabController, **Phase 1 Complete**

### 2.3 Risk Management Module
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Implement position sizing in TradeManager (`src/apps/workbench/core/trade_manager.cpp`).
    *   [ ] Add stop-loss and take-profit logic.
    *   [ ] Enforce 2% max exposure per trade.
    *   [ ] Create UI panel in BackendTabController.
-   **Priority**: High
-   **Estimated Time**: 6 days
-   **Dependencies**: TradeManager, BackendTabController, **Phase 1 Complete**

### 2.4 Order Management System
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Integrate OANDA Trading API (`src/connectors/oanda_connector.cpp`).
    *   [ ] Track order states (pending, filled, canceled).
    *   [ ] Handle errors and retries.
    *   [ ] Display order status in BackendTabController with charts.
-   **Priority**: High
-   **Estimated Time**: 7 days
-   **Dependencies**: OandaConnector, TradeManager, BackendTabController, **Phase 1 Complete**

### 2.5 Demo Account Integration
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Configure OANDA demo credentials (`src/apps/workbench/config.cpp`).
    *   [ ] Enable paper trading in TradeManager.
    *   [ ] Test order placement in demo environment.
    *   [ ] Show balance and P&L charts in BackendTabController.
-   **Priority**: High
-   **Estimated Time**: 5 days
-   **Dependencies**: OandaConnector, TradeManager, BackendTabController, **Phase 1 Complete**

## Phase 3: Demo Trading & Optimization

### 3.1 Paper Trading
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Enable continuous paper trading with live data.
    *   [ ] Log trades (`src/engine/logging.cpp`).
    *   [ ] Monitor performance with real-time charts.
    *   [ ] Display trade history in BackendTabController.
-   **Priority**: High
-   **Estimated Time**: 5 days
-   **Dependencies**: TradeManager, BackendTabController, **Phase 2 Complete**

### 3.2 Performance Monitoring
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Add analytics (win/loss ratio, ROI) in TradeManager.
    *   [ ] Create anomaly alerts (e.g., large drawdowns).
    *   [ ] Display performance charts in BackendTabController.
    *   [ ] Export trade logs.
-   **Priority**: Medium
-   **Estimated Time**: 4 days
-   **Dependencies**: TradeManager, BackendTabController, **Phase 2 Complete**

### 3.3 Strategy Optimization
-   **Status**: Not implemented.
-   **Tasks**:
    *   [ ] Automate threshold optimization using backtest results.
    *   [ ] Test ML models for metric-price relationships in ServiceProxyEngine.
    *   [ ] Add tuning UI with charts in EngineTabController.
    *   [ ] Validate strategies in paper trading.
-   **Priority**: Medium
-   **Estimated Time**: 7 days
-   **Dependencies**: ServiceProxyEngine, EngineTabController, Backtester, **Phase 2 Complete**

## Technical Debt & Optimizations (Lower Priority)

### Performance Scaling
-   **Issue**: Super-linear scaling in PatternMetricEngine (POC 4).
-   **Tasks**:
    *   [ ] Optimize data structures (`src/quantum/pattern_metric_engine.cpp`).
    *   [ ] Implement sliding window for pattern history.
-   **Priority**: Medium
-   **Estimated Time**: 5 days
-   **Dependencies**: PatternMetricEngine

### Event-Driven Architecture
-   **Issue**: Tight component coupling.
-   **Tasks**:
    *   [ ] Implement EventBus (`src/apps/workbench/core/ui_layout_manager.cpp`).
    *   [ ] Refactor interactions to use events.
-   **Priority**: Medium
-   **Estimated Time**: 6 days
-   **Dependencies**: UILayoutManager

### API Rate Limiting
-   **Issue**: Potential OANDA API disruptions.
-   **Tasks**:
    *   [ ] Add request batching in OandaConnector.
    *   [ ] Implement caching (`src/connectors/market_data_converter.cpp`).
-   **Priority**: Medium
-   **Estimated Time**: 4 days
-   **Dependencies**: OandaConnector

## Immediate Action Items (This Week)
1.  **Fix All Critical Build Errors**: Prioritize **Phase 1.1** immediately. This is the **single most important blocking item**.
2.  **48-Hour Sample Data Setup**: Fetch EUR/USD M1 data and store in JSON with integrity checks (**Phase 1.2**).
3.  **Chart Implementation**: Render candlestick charts and metric plots in SignalsTabController (**Phase 1.3**).

## Success Metrics
-   **Testbed Completion**:
    *   All critical build errors resolved.
    *   48 hours of clean data processed.
    *   Candlestick and metric charts rendering at >10Hz.
    *   3+ reliable signal patterns identified.
    *   Correlation coefficient >0.3.
-   **Demo Trading Readiness**:
    *   Backtested Sharpe ratio >1.0.
    *   <5% max drawdown in paper trading.
    *   100 demo trades without errors.
    *   Risk limits enforced.
