# SEP Engine Workbench GUI Architecture

## Current Problem Analysis
The SEP Workbench GUI (`src/apps/workbench/workbench_core.cpp`) is disorganized, with overlapping responsibilities and compilation issues preventing chart rendering. The refactoring plan adopts a 3-tab architecture with a focus on clear visual layouts and robust chart rendering to address these issues.

### Current Architecture Issues
- **TradingHUD** (`src/apps/workbench/core_old/trading_hud.cpp`, 549+ lines): Monolithic, handling charts, signals, and trading.
- **MetricsDashboard** (`src/apps/workbench/core_old/metrics_dashboard.cpp`): Duplicates signal and OANDA integration.
- **Chart Rendering Failure**: ImGui errors (`core.NullDereference` in `imgui.cpp`) prevent chart display.
- **Build Warnings**: `cert-err33-c`, `security.FloatLoopCounter` in `signals_tab_controller.cpp`, `imgui_demo.cpp`.
- **Tight Coupling**: Components lack separation, complicating updates.

### Component Responsibility Overlap
```
TradingHUD:
├── Candlestick chart rendering
├── SEP signal overlays
├── Order management
├── Performance tracking
├── Risk management
├── Multi-timeframe analysis
└── Account management

MetricsDashboard:
├── Metric visualization
├── OANDA integration
├── Technical indicators
└── Historical analysis
```

## Proposed 3-Tab Architecture

### Tab 1: **SIGNALS** - Trading Signals & Market Analysis
**Purpose**: Provide a clean, interactive interface for signal visualization and market analysis, prioritizing chart rendering.

#### Visual Layout
```plaintext
+-----------------------------------------------+
| Signals Tab                                   |
+-----------------------------------------------+
| Candlestick Chart (70% width)                 |
| - EUR/USD M1 candles, zoom/pan enabled        |
| - SEP signal overlays (color-coded arrows)    |
| - Support/resistance lines                    |
|-----------------------------------------------|
| Metric Plots (30% width, vertical stack)      |
| - Coherence plot (0-1 scale, 1H/4H averages)  |
| - Stability plot (0-1 scale, 1H/4H averages)  |
| - Entropy plot (0-1 scale, 1H/4H averages)    |
| - Threshold indicators (dashed lines)         |
|-----------------------------------------------|
| Signal History Panel (bottom, collapsible)    |
| - Table: Signal type, timestamp, confidence    |
| - Win/loss ratio, frequency stats             |
+-----------------------------------------------+
```

#### Primary Components
- **SignalAnalysisPanel**: Displays real-time SEP signals, confidence levels, and traditional indicator comparisons.
- **MarketChartRenderer**: Renders candlestick charts with signal overlays, zoom/pan, and trend lines.
- **SignalHistoryTracker**: Tracks signal performance and displays historical data in a table.

#### Relocated/Refactored Components
```
FROM TradingHUD → SIGNALS TAB:
├── Candlestick chart rendering
├── SEP signal overlays
├── Signal interpretation
├── Multi-timeframe analysis
├── Signal performance metrics

FROM MetricsDashboard → SIGNALS TAB:
├── Metric visualization
├── Traditional vs. SEP comparison
├── Historical signal analysis
```

### Tab 2: **ENGINE** - SEP Engine Diagnostics & Analysis
**Purpose**: Offer detailed quantum diagnostics with interactive metric and pattern plots.

#### Visual Layout
```plaintext
+-----------------------------------------------+
| Engine Tab                                    |
+-----------------------------------------------+
| Metric Dashboard (50% width)                  |
| - Coherence, stability, entropy time series   |
| - Rolling averages (1H/4H)                    |
| - Pattern emergence rate plot                 |
|-----------------------------------------------|
| Pattern Analysis Panel (30% width)            |
| - Pattern frequency histogram                 |
| - Correlation matrix (metrics vs. price)      |
| - Quantum state visualization (scatter plot)  |
|-----------------------------------------------|
| Diagnostics Panel (20% width, collapsible)    |
| - CUDA kernel performance (ms per cycle)      |
| - Memory usage (STM/MTM/LTM breakdown)       |
| - Bottleneck alerts (red/yellow indicators)   |
+-----------------------------------------------+
```

#### Primary Components
- **QuantumMetricsDashboard**: Plots coherence, stability, entropy, and pattern evolution.
- **PatternAnalysisPanel**: Displays pattern stats, correlations, and quantum state visualizations.
- **EngineDiagnostics**: Monitors CUDA performance, memory usage, and bottlenecks.
- **ResearchTools**: Supports parameter tuning and data export.

#### Relocated/Refactored Components
```
NEW ENGINE TAB:
├── MetricsMonitor functionality
├── Pattern analysis from MetricsDashboard
├── Engine diagnostics from TradingHUD
├── Memory monitoring
├── Quantum processor status
└── Research tools
```

### Tab 3: **BACKEND** - Trading Operations & Infrastructure
**Purpose**: Manage trading, backtesting, and system operations with performance charts.

#### Visual Layout
```plaintext
+-----------------------------------------------+
| Backend Tab                                   |
+-----------------------------------------------+
| Trading Terminal (50% width)                  |
| - Order placement form (buy/sell, size)       |
| - Position tracking table (P&L, status)       |
| - Risk controls (stop-loss, take-profit)      |
|-----------------------------------------------|
| Backtesting Suite (30% width)                 |
| - Strategy selection dropdown                 |
| - Performance charts (Sharpe, drawdown)       |
| - Historical trade visualization              |
|-----------------------------------------------|
| System Monitor (20% width, collapsible)       |
| - OANDA connection status (green/red)        |
| - API rate limit gauge                       |
| - Data quality alerts                        |
+-----------------------------------------------+
```

#### Primary Components
- **TradingTerminal**: Manages order placement, position tracking, and risk controls.
- **BacktestingSuite**: Provides strategy backtesting with performance charts.
- **SystemAdmin**: Manages OANDA connections and system health.
- **InfrastructureMonitor**: Tracks API limits and data quality.

#### Relocated/Refactored Components
```
FROM TradingHUD → BACKEND TAB:
├── Order management
├── Position tracking
├── Account management
├── Risk management
├── Performance analytics

NEW BACKEND COMPONENTS:
├── Enhanced backtesting
├── System administration
├── Infrastructure monitoring
└── Configuration management
```

## Detailed Refactoring Plan

### Phase 1: Chart Rendering & Component Separation
#### 1.1 Fix ImGui Compilation Issues
```cpp
// Address null pointer dereferences in imgui.cpp
if (window != nullptr) {
    window->RootWindowForNav->Flags; // Line 6168
}
```

#### 1.2 Break Down TradingHUD
```cpp
// Current: Monolithic TradingHUD
class TradingHUD { /* 549+ lines */ };

// Proposed: Specialized Components
class SignalAnalysisPanel { // SIGNALS TAB
    void renderSignals();
};
class MarketChartRenderer { // SIGNALS TAB
    void renderCandlestickChart();
};
class TradingTerminal { // BACKEND TAB
    void manageOrders();
};
class EngineMonitor { // ENGINE TAB
    void renderDiagnostics();
};
```

#### 1.3 Consolidate OANDA Integration
```cpp
// Proposed: Centralized Service
class OandaService {
    void subscribeToCandles(CandleSubscriber* subscriber);
};
```

### Phase 2: Tab-Specific Implementation
#### 2.1 SIGNALS Tab Architecture
```cpp
class SignalsTabController {
private:
    std::unique_ptr<SignalAnalysisPanel> signal_panel_;
    std::unique_ptr<MarketChartRenderer> chart_renderer_;
    std::unique_ptr<SignalHistoryTracker> history_tracker_;
    std::shared_ptr<OandaService> oanda_service_;
public:
    void render() {
        ImGui::Begin("Signals");
        chart_renderer_->renderCandlestickChart();
        signal_panel_->renderSignals();
        history_tracker_->renderHistory();
        ImGui::End();
    }
};
```

#### 2.2 ENGINE Tab Architecture
```cpp
class EngineTabController {
private:
    std::unique_ptr<QuantumMetricsDashboard> metrics_dashboard_;
    std::unique_ptr<PatternAnalysisPanel> pattern_panel_;
    std::unique_ptr<EngineDiagnostics> diagnostics_;
public:
    void render() {
        ImGui::Begin("Engine");
        metrics_dashboard_->renderMetricPlots();
        pattern_panel_->renderPatternStats();
        diagnostics_->renderDiagnostics();
        ImGui::End();
    }
};
```

#### 2.3 BACKEND Tab Architecture
```cpp
class BackendTabController {
private:
    std::unique_ptr<TradingTerminal> trading_terminal_;
    std::unique_ptr<BacktestingSuite> backtester_;
    std::unique_ptr<SystemAdmin> system_admin_;
public:
    void render() {
        ImGui::Begin("Backend");
        trading_terminal_->renderOrders();
        backtester_->renderBacktestCharts();
        system_admin_->renderStatus();
        ImGui::End();
    }
};
```

### Phase 3: Data Flow Architecture
#### 3.1 Event-Driven Communication
```cpp
class EventBus {
public:
    template<typename EventType>
    void publish(const EventType& event);
    template<typename EventType>
    void subscribe(std::function<void(const EventType&)> handler);
};
```

#### 3.2 Shared Data Services
```cpp
class MarketDataService {
    // Manages OANDA data for chart rendering
};
class SignalManager {
    // Distributes signals to tabs
};
```

## Implementation Priority
1. **Chart Rendering** (Critical):
   - Fix ImGui errors to render candlestick and metric charts.
   - Implement SignalAnalysisPanel and MarketChartRenderer.
2. **SIGNALS Tab** (Immediate Value):
   - Extract signal processing from TradingHUD.
   - Add interactive chart controls.
3. **BACKEND Tab** (Infrastructure):
   - Build TradingTerminal and BacktestingSuite.
4. **ENGINE Tab** (Diagnostics):
   - Refactor MetricsDashboard for diagnostic plots.

## UI/UX Design Principles
- **SIGNALS Tab**: Clean charts, real-time updates, color-coded signals, zoom/pan controls.
- **ENGINE Tab**: Dense metric plots, draggable panels, export options.
- **BACKEND Tab**: Reliable status indicators, safety-focused trading UI.

## Migration Path
1. **Fix Build Issues**: Address ImGui errors for chart rendering.
2. **SIGNALS Tab**: Implement candlestick and metric charts.
3. **BACKEND Tab**: Build trading and backtesting UI.
4. **ENGINE Tab**: Add diagnostic plots.
5. **Integration**: Connect tabs via EventBus.

## Success Metrics
- **Chart Rendering**: Candlestick and metric charts visible at >10Hz.
- **Tab Separation**: Components <200 lines, non-overlapping responsibilities.
- **Maintainability**: Independent updates per tab.
- **User Experience**: Intuitive chart interactions.
- **Performance**: Low resource usage for real-time rendering.

## Current Status
- **Implemented**: Basic dashboard (`workbench_core.cpp`), partial SignalsTabController.
- **Blocked**: Chart rendering due to ImGui errors.
- **Next Steps**: Fix build issues, complete Signals Tab charts.

This architecture ensures a scalable, chart-focused GUI aligned with trading objectives.