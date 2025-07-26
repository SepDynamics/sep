# SEP Engine Workbench GUI Architecture

## Current Problem Analysis: **CRITICAL BUILD FAILURE**

The SEP Workbench GUI is currently **unbuildable and non-functional**. Critical compilation errors, primarily stemming from an architectural flaw where core engine components depend on GUI-specific headers (`imgui.h`), prevent the application from launching. Chart rendering, a key feature, is completely blocked.

The immediate goal is to refactor the code to resolve these build errors. The proposed 3-tab architecture remains the target design *after* the build is stabilized.

### Primary Compilation Blockers
-   **Fatal Header Conflict**: Core files like `data_parser.cpp` and `engine.cu` fail to build because they indirectly include `imgui.h` via `apps/workbench/core/common_structs.h`. This is the most severe issue.
-   **Missing `implot.h`**: The `signals_tab_controller.cpp` fails because the `implot.h` header for charting is missing or not configured in the build system.
-   **Undeclared Types**: `signals_tab_controller.h` uses undeclared types like `TechnicalIndicator` and `TrendLine`. These GUI-specific types need to be defined within the workbench library, not in a shared header.
-   **Namespace/API Errors**: The `service_connector.cpp` and `workbench_main.cpp` have multiple errors due to incorrect namespace usage (`sep::config::ConfigManager`) and missing includes for `ServiceProxyEngine` and `backtester`.
-   **Static Analysis Warnings**: The `report.md` file highlights numerous high and medium severity issues in third-party libraries (`imgui`, `yaml-cpp`) that should be addressed post-build-fix to ensure stability.

## Proposed 3-Tab Architecture (Post-Build-Fix)

The GUI will be organized into three distinct tabs to separate concerns and improve usability.

### Tab 1: **SIGNALS** - Trading & Market Analysis
-   **Purpose**: Clean, interactive interface for signal visualization and market analysis.
-   **Layout**: Candlestick chart (70%) with SEP signal overlays. Metric plots for Coherence, Stability, and Entropy (30%).

### Tab 2: **ENGINE** - SEP Engine Diagnostics
-   **Purpose**: Detailed quantum diagnostics and performance monitoring.
-   **Layout**: Time series plots for core metrics, pattern frequency histograms, and a correlation matrix.

### Tab 3: **BACKEND** - Trading Operations & Backtesting
-   **Purpose**: Manage trading, backtesting, and system health.
-   **Layout**: Trading terminal for order placement, a backtesting suite with performance charts, and a system monitor for API/data status.

## Detailed Refactoring Plan

### Phase 1: Critical Build Fixes (Immediate Priority)
This phase focuses exclusively on making the project compilable again.

#### 1.1: Decouple Core Logic from GUI
-   **Action**: Create `src/common/financial_data_types.h`. Move all non-GUI data structures (`CandleData`, `TickData`, `SEPSignalData`, etc.) from `apps/workbench/core/common_structs.h` into this new header.
-   **Action**: Update all core engine files (`data_parser.h`, `engine.cu`, etc.) to include `financial_data_types.h` instead of `common_structs.h`.
-   **Action**: Strip `imgui.h` from `common_structs.h`. This header should now only contain structs specific to the GUI, like `ChartZoom` or `EnhancedHoverInfo`.

#### 1.2: Fix Namespace and Include Errors
-   **Action**: In `service_connector.cpp`, change `workbench::Config::getInstance()` to the correct `sep::config::ConfigManager::getInstance()`.
-   **Action**: In `workbench_main.cpp`, `engine_tab_controller.h`, etc., add the necessary `#include` statements for `ServiceProxyEngine`, `backtester`, `DataLoader` and qualify them with their correct namespaces (e.g., `sep::core::ServiceProxyEngine`).

#### 1.3: Resolve Missing Dependencies and Typos
-   **Action**: Integrate `implot` as a third-party dependency and ensure `implot.h` is correctly included in `signals_tab_controller.cpp`.
-   **Action**: Correct the `undeclared identifier 's'` typo in `pattern_metric_engine.cpp`.
-   **Action**: Add `#include <cstdint>` in `emitterutils.cpp`.

### Phase 2: Tab-Specific Implementation (Post-Build-Fix)
-   **Action**: Implement the `SignalsTabController` with candlestick charts and metric plots.
-   **Action**: Implement the `EngineTabController` with diagnostic panels.
-   **Action**: Implement the `BackendTabController` with trading and backtesting UI.

## Implementation Priority

1.  **Critical Build Errors (IMMEDIATE)**: Address all compilation errors listed in Phase 1 of the refactoring plan. **This is the single highest priority and blocks all other work.**
2.  **Chart Rendering** (Post-Build-Fix): Implement robust candlestick charts in `SignalsTabController`.
3.  **Data Flow Integration** (Post-Build-Fix): Connect the OANDA connector and `PatternMetricEngine` to the GUI tabs.
4.  **Backend & Engine Tabs** (Post-Build-Fix): Build out the UI for the remaining tabs.

## Success Metrics
-   **Compilation**: The project builds successfully with zero errors.
-   **Chart Rendering**: Candlestick charts are visible and update at >10Hz.
-   **Tab Separation**: UI components are logically separated with clear responsibilities.
-   **Maintainability**: Core engine can be modified and built without affecting the GUI.

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
-   **SignalAnalysisPanel**: Displays real-time SEP signals, confidence levels, and traditional indicator comparisons.
-   **MarketChartRenderer**: Renders candlestick charts with signal overlays, zoom/pan, and trend lines.
-   **SignalHistoryTracker**: Tracks signal performance and displays historical data in a table.

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
-   **QuantumMetricsDashboard**: Plots coherence, stability, entropy, and pattern evolution.
-   **PatternAnalysisPanel**: Displays pattern stats, correlations, and quantum state visualizations.
-   **EngineDiagnostics**: Monitors CUDA performance, memory usage, and bottlenecks.
-   **ResearchTools**: Supports parameter tuning and data export.

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
-   **TradingTerminal**: Manages order placement, position tracking, and risk controls.
-   **BacktestingSuite**: Provides strategy backtesting with performance charts.
-   **SystemAdmin**: Manages OANDA connections and system health.
-   **InfrastructureMonitor**: Tracks API limits and data quality.

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

### Phase 1: Critical Build Fixes & Chart Rendering
#### 1.1 Fix Critical Compilation Issues (Reiteration from TODO.md)
-   **Action**: Address `error: invalid application of 'sizeof' to an incomplete type 'sep::workbench::CandleData'` by ensuring `src/apps/workbench/core/common_structs.h` is properly and fully included before its types are used in template instantiations (e.g., `std::deque<CandleData>`) in `data_parser.h` and `engine.cu`. This requires **moving core data structures out of GUI-specific headers**.
-   **Action**: Resolve `error: function definition is not allowed here` in `src/engine/logging.cpp` by ensuring functions are defined within the correct namespace scope and not improperly nested.
-   **Action**: Fix `error: no member named 'Config' in namespace 'sep::workbench'` in `src/apps/workbench/core/service_connector.cpp` by changing `workbench::Config::getInstance()` to `sep::config::ConfigManager::getInstance()`.
-   **Action**: Resolve `error: use of undeclared identifier 'backtester'` and `no type named 'ServiceProxyEngine'` in `src/apps/workbench/core/workbench_main.cpp` and `src/apps/workbench/tabs/engine_tab_controller.h` by adding necessary `#include` statements and qualifying namespaces.
-   **Action**: Add `#include <cstdint>` to `src/build/_deps/yaml-cpp-src/src/emitterutils.cpp` to resolve `uint16_t` and `uint32_t` undeclared identifier errors.

#### 1.2 Break Down TradingHUD (Post-Compilation Fixes)
```cpp
// Current: Monolithic TradingHUD
class TradingHUD { /* 549+ lines */ };

// Proposed: Specialized Components (implemented as separate classes/panels)
class SignalAnalysisPanel; // For Signals Tab
class MarketChartRenderer; // For Signals Tab
class TradingTerminal;     // For Backend Tab
class EngineMonitor;       // For Engine Tab
// ... and others as needed for the detailed breakdown
```

#### 1.3 Consolidate OANDA Integration (Post-Compilation Fixes)
```cpp
// Proposed: Centralized Service (OandaConnector is already a good start)
class OandaService { // Facade or wrapper around OandaConnector
    // Provides market data and trading execution services
    // Will be managed by ServiceConnector
};
```

### Phase 2: Tab-Specific Implementation (Post-Compilation Fixes)
#### 2.1 SIGNALS Tab Architecture
```cpp
// SignalsTabController will orchestrate rendering of its child panels
class SignalsTabController {
private:
    std::unique_ptr<SignalAnalysisPanel> signal_panel_;
    std::unique_ptr<MarketChartRenderer> chart_renderer_;
    std::unique_ptr<SignalHistoryTracker> history_tracker_;
    // ... other dependencies like OandaService or MetricsMonitor shared pointers
public:
    void render(); // Delegates rendering to sub-panels
};
```

#### 2.2 ENGINE Tab Architecture
```cpp
// EngineTabController will orchestrate rendering of its child panels
class EngineTabController {
private:
    std::unique_ptr<QuantumMetricsDashboard> metrics_dashboard_;
    std::unique_ptr<PatternAnalysisPanel> pattern_panel_;
    std::unique_ptr<EngineDiagnostics> diagnostics_;
public:
    void render(); // Delegates rendering to sub-panels
};
```

#### 2.3 BACKEND Tab Architecture
```cpp
// BackendTabController will orchestrate rendering of its child panels
class BackendTabController {
private:
    std::unique_ptr<TradingTerminal> trading_terminal_;
    std::unique_ptr<BacktestingSuite> backtester_;
    std::unique_ptr<SystemAdmin> system_admin_;
public:
    void render(); // Delegates rendering to sub-panels
};
```

### Phase 3: Data Flow Architecture (Post-Compilation Fixes)
#### 3.1 Event-Driven Communication
```cpp
// MarketDataService, SignalManager, etc., will publish/subscribe via EventBus
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
// Centralized services providing data access to all tabs
class MarketDataService; // Provides historical & real-time market data
class SignalManager;     // Distributes processed signals
class MetricsDataService; // Provides engine metrics
```

## Implementation Priority
1.  **Critical Build Errors (IMMEDIATE)**: Address all critical compilation errors listed in Section 1.1 of the refactoring plan. **This is the single highest priority.**
2.  **Chart Rendering** (Critical, post-build fixes): Implement robust candlestick and metric charts in `MarketChartRenderer` and integrate into `SignalsTabController`.
3.  **SIGNALS Tab** (Immediate Value, post-charting): Extract signal processing from `TradingHUD` into `SignalAnalysisPanel` and integrate into `SignalsTabController`.
4.  **BACKEND Tab** (Infrastructure): Build `TradingTerminal` and `BacktestingSuite` components for `BackendTabController`.
5.  **ENGINE Tab** (Diagnostics): Refactor `MetricsDashboard` into `QuantumMetricsDashboard` and `EngineDiagnostics` for `EngineTabController`.

## UI/UX Design Principles
-   **SIGNALS Tab**: Clean charts, real-time updates, color-coded signals, zoom/pan controls.
-   **ENGINE Tab**: Dense metric plots, draggable panels, export options.
-   **BACKEND Tab**: Reliable status indicators, safety-focused trading UI.

## Migration Path
1.  **Fix Build Issues**: Address all critical and high-priority compilation errors to enable a clean build.
2.  **SIGNALS Tab**: Implement candlestick and metric charts.
3.  **BACKEND Tab**: Build trading and backtesting UI.
4.  **ENGINE Tab**: Add diagnostic plots.
5.  **Integration**: Connect tabs via EventBus.

## Success Metrics
-   **Chart Rendering**: Candlestick and metric charts visible at >10Hz after compilation is fixed.
-   **Tab Separation**: Components <200 lines, non-overlapping responsibilities.
-   **Maintainability**: Independent updates per tab.
-   **User Experience**: Intuitive chart interactions.
-   **Performance**: Low resource usage for real-time rendering.

## Current Status
-   **Implemented**: Basic dashboard framework (`workbench_core.cpp`), partial `SignalsTabController`.
-   **Blocked**: **CRITICAL compilation errors preventing the entire application from building and running.**
-   **Next Steps**: Focus entirely on resolving the build errors listed in Section 1.1 of the refactoring plan.