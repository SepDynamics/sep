
---

### TODO.md

```markdown
# SEP Engine Development Roadmap

## Objective
Transform the SEP Engine into a production-ready trading platform by integrating core components, addressing minimal build errors, and enabling demo trading. This roadmap prioritizes testbed completion, signal validation, and trading infrastructure development, ensuring proper data routing through the verified pipeline.

## Development Phases
1. **Phase 1: Testbed Completion** (2-3 weeks)
   - Establish a stable testing environment with clean data and dashboard integration.
2. **Phase 2: Trading Preparation** (4-6 weeks)
   - Validate signals, implement risk management, and prepare for demo trading.
3. **Phase 3: Demo Trading & Optimization** (3-4 weeks)
   - Launch paper trading and optimize strategies based on performance.

## Phase 1: Testbed Completion

### 1.1 48-Hour Sample Data Setup
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Fetch 48 hours of EUR/USD M1 data via OandaConnector (`src/connectors/oanda_connector.cpp`).
  - [ ] Store in JSON files with DataParser (`src/engine/data_parser.cpp`).
  - [ ] Add integrity checks for missing candles or invalid prices.
  - [ ] Create data loader for workbench (`src/apps/workbench/core/service_connector.cpp`).
- **Priority**: High
- **Estimated Time**: 3 days
- **Dependencies**: OandaConnector, DataParser

### 1.2 Enhanced Metrics Display
- **Status**: Partially implemented (`src/apps/workbench/core/metrics_monitor.cpp` not fully integrated).
- **Tasks**:
  - [ ] Connect MetricsMonitor to SignalsTabController (`src/apps/workbench/tabs/signals_tab_controller.cpp`).
  - [ ] Render coherence, stability, entropy in real-time ImGui plots.
  - [ ] Add 1-hour and 4-hour rolling averages for metrics.
  - [ ] Visualize thresholds with dashed lines.
- **Priority**: High
- **Estimated Time**: 4 days
- **Dependencies**: MetricsMonitor, SignalsTabController

### 1.3 Pattern Discovery Framework
- **Status**: Not implemented (missing threshold logic).
- **Tasks**:
  - [ ] Implement threshold detection in PatternMetricEngine (`src/quantum/pattern_metric_engine.cpp`).
  - [ ] Test signal rules (e.g., `stability < 0.3 && entropy > 0.7`).
  - [ ] Add UI panel for threshold adjustment in SignalsTabController.
  - [ ] Log patterns to file (`src/engine/logging.cpp`).
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: PatternMetricEngine, SignalsTabController

### 1.4 Correlation Analysis
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Calculate metric-price correlations in MultiTimeframeAnalyzer (`src/apps/workbench/core/multi_timeframe_analyzer.cpp`).
  - [ ] Display coefficients in EngineTabController (`src/apps/workbench/tabs/engine_tab_controller.cpp`).
  - [ ] Export correlation data (`src/engine/data_parser.cpp`).
- **Priority**: Medium
- **Estimated Time**: 3 days
- **Dependencies**: MultiTimeframeAnalyzer, EngineTabController

## Phase 2: Trading Preparation

### 2.1 Backtesting Framework
- **Status**: Partially implemented (`src/apps/workbench/backtester/backtester.cpp` lacks signal integration).
- **Tasks**:
  - [ ] Integrate PatternMetricEngine signals into BacktesterEngine (`src/apps/workbench/backtester/core/backtester_engine.cpp`).
  - [ ] Add metrics (Sharpe ratio, max drawdown) in PerformanceMetrics (`src/apps/workbench/backtester/core/performance_metrics.cpp`).
  - [ ] Create UI panel in BackendTabController (`src/apps/workbench/tabs/backend_tab_controller.cpp`).
  - [ ] Support 48-hour data input.
- **Priority**: High
- **Estimated Time**: 7 days
- **Dependencies**: BacktesterEngine, PatternMetricEngine, BackendTabController

### 2.2 Signal Validation System
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Build validator in ServiceProxyEngine (`src/apps/workbench/core/service_proxy_engine.cpp`).
  - [ ] Compare signals against price movements.
  - [ ] Generate reports (accuracy, false positives).
  - [ ] Display results in BackendTabController.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: ServiceProxyEngine, BackendTabController

### 2.3 Risk Management Module
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Implement position sizing in TradeManager (`src/apps/workbench/core/trade_manager.cpp`).
  - [ ] Add stop-loss and take-profit logic.
  - [ ] Enforce 2% max exposure per trade.
  - [ ] Create UI panel in BackendTabController.
- **Priority**: High
- **Estimated Time**: 6 days
- **Dependencies**: TradeManager, BackendTabController

### 2.4 Order Management System
- **Status**: Not implemented (TradeManager lacks order logic).
- **Tasks**:
  - [ ] Integrate OANDA Trading API in OandaConnector (`src/connectors/oanda_connector.cpp`).
  - [ ] Track order states (pending, filled, canceled).
  - [ ] Handle errors and retries.
  - [ ] Display order status in BackendTabController.
- **Priority**: High
- **Estimated Time**: 7 days
- **Dependencies**: OandaConnector, TradeManager, BackendTabController

### 2.5 Demo Account Integration
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Configure OANDA demo credentials (`src/apps/workbench/config.cpp`).
  - [ ] Enable paper trading in TradeManager.
  - [ ] Test order placement in demo environment.
  - [ ] Show demo balance and P&L in BackendTabController.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: OandaConnector, TradeManager, BackendTabController

## Phase 3: Demo Trading & Optimization

### 3.1 Paper Trading
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Enable continuous paper trading with live data.
  - [ ] Log trades to file (`src/engine/logging.cpp`).
  - [ ] Monitor performance in real-time.
  - [ ] Display trade history in BackendTabController.
- **Priority**: High
- **Estimated Time**: 5 days
- **Dependencies**: TradeManager, BackendTabController

### 3.2 Performance Monitoring
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Add analytics (win/loss ratio, ROI) in TradeManager.
  - [ ] Create alerts for anomalies (e.g., large drawdowns).
  - [ ] Display metrics in BackendTabController.
  - [ ] Export trade logs.
- **Priority**: Medium
- **Estimated Time**: 4 days
- **Dependencies**: TradeManager, BackendTabController

### 3.3 Strategy Optimization
- **Status**: Not implemented.
- **Tasks**:
  - [ ] Automate threshold optimization using backtest results.
  - [ ] Test ML models for metric-price relationships in ServiceProxyEngine.
  - [ ] Add UI panel for tuning in EngineTabController.
  - [ ] Validate strategies in paper trading.
- **Priority**: Medium
- **Estimated Time**: 7 days
- **Dependencies**: ServiceProxyEngine, EngineTabController, Backtester

## Technical Debt & Optimizations

### Build Error Fixes
- **Issue**: Static analysis warnings (`cert-err33-c`, `security.FloatLoopCounter` in ImGui files).
- **Tasks**:
  - [ ] Cast ignored return values to `void` in `imgui_impl_opengl3.cpp`, `signals_tab_controller.cpp`.
  - [ ] Address null pointer dereferences in `imgui.cpp`.
- **Priority**: Medium
- **Estimated Time**: 2 days
- **Dependencies**: None

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
  - [ ] Implement EventBus in UILayoutManager (`src/apps/workbench/core/ui_layout_manager.cpp`).
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
1. **48-Hour Sample Data Setup**:
   - Fetch EUR/USD M1 data and store in JSON.
   - Implement integrity checks.
2. **Metrics Display Integration**:
   - Connect MetricsMonitor to SignalsTabController.
   - Render ImGui plots for metrics.
3. **Threshold Detection**:
   - Add threshold logic to PatternMetricEngine.
   - Test basic signal rules.

## Success Metrics
- **Testbed Completion**:
  - 48 hours of clean data processed.
  - Metrics updating at >10Hz in dashboard.
  - 3+ reliable signal patterns identified.
  - Correlation coefficient >0.3.
- **Demo Trading Readiness**:
  - Backtested Sharpe ratio >1.0.
  - <5% max drawdown in paper trading.
  - 100 demo trades without errors.
  - Risk limits enforced.
