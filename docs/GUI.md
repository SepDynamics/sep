# SEP Engine Workbench GUI Architecture

## Current Problem Analysis

The SEP Workbench GUI, implemented in `src/apps/workbench/workbench_core.cpp`, suffers from disorganized structure and overlapping responsibilities, hindering maintainability and scalability. The refactoring plan addresses these issues by adopting a 3-tab architecture to streamline user interaction and component integration.

### Current Architecture Issues
- **TradingHUD** (`src/apps/workbench/core_old/trading_hud.cpp`, 549+ lines): Monolithic, handling charting, signals, trading, and risk management.
- **MetricsDashboard** (`src/apps/workbench/core_old/metrics_dashboard.cpp`): Duplicates signal and OANDA integration from TradingHUD.
- **Overlapping OANDA Integration**: Multiple components access OandaConnector independently.
- **Tight Coupling**: Components lack clear separation, complicating updates.
- **Build Warnings**: ImGui-related issues (`cert-err33-c`, `security.FloatLoopCounter`) in `signals_tab_controller.cpp` and `imgui.cpp`.

### Component Responsibility Overlap
```
TradingHUD:
├── Chart rendering & technical analysis
├── SEP signal overlays
├── Order management & trading
├── Performance tracking
├── Risk management
├── Multi-timeframe analysis
└── Account management

MetricsDashboard:
├── SEP metrics visualization
├── OANDA integration
├── Technical indicators
└── Historical analysis
```

## Proposed 3-Tab Architecture

### Tab 1: **SIGNALS** - Trading Signals & Market Analysis
**Purpose**: Provide a focused interface for signal visualization and market analysis.

#### Primary Components:
- **SignalAnalysisPanel**: Displays real-time SEP signals (coherence, stability, entropy), signal strength, and traditional vs. SEP comparisons.
- **MarketChartRenderer**: Renders clean candlestick charts with SEP signal overlays and basic trend lines.
- **SignalHistoryTracker**: Tracks signal performance, win/loss ratios, and threshold events.

#### Relocated/Refactored Components:
```
FROM TradingHUD → SIGNALS TAB:
├── SEP signal processing
├── Simplified chart rendering
├── Signal interpretation
├── Multi-timeframe analysis
├── Signal performance metrics

FROM MetricsDashboard → SIGNALS TAB:
├── Traditional vs. SEP comparison
├── Signal confidence calculations
├── Historical signal analysis
```

### Tab 2: **ENGINE** - SEP Engine Diagnostics & Analysis
**Purpose**: Offer deep insights into quantum engine internals for tuning and research.

#### Primary Components:
- **QuantumMetricsDashboard**: Graphs coherence, stability, entropy, and pattern evolution.
- **PatternAnalysisPanel**: Displays pattern detection stats, persistence, and correlations.
- **EngineDiagnostics**: Monitors CUDA kernel performance, memory usage, and bottlenecks.
- **ResearchTools**: Supports parameter tuning, A/B testing, and data export.

#### Relocated/Refactored Components:
```
NEW ENGINE TAB:
├── MetricsMonitor functionality
├── Pattern analysis from MetricsDashboard
├── SEP engine diagnostics from TradingHUD
├── Memory monitoring
├── Quantum processor status
└── Research tools
```

### Tab 3: **BACKEND** - Trading Operations & Infrastructure
**Purpose**: Manage trading execution, backtesting, and system operations.

#### Primary Components:
- **TradingTerminal**: Handles order placement, position tracking, and risk controls.
- **BacktestingSuite**: Interfaces for strategy backtesting and performance analysis.
- **SystemAdmin**: Manages OANDA connections, data feeds, and system health.
- **InfrastructureMonitor**: Tracks database, API rate limits, and data quality.

#### Relocated/Refactored Components:
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

### Phase 1: Extract and Separate Components
#### 1.1 Break Down TradingHUD Monolith
```cpp
// Current: TradingHUD (monolithic)
class TradingHUD { /* everything */ };

// Proposed: Specialized Components
class SignalAnalysisPanel {      // SIGNALS TAB
    // Signal processing and visualization
};
class MarketChartRenderer {      // SIGNALS TAB
    // Chart rendering and overlays
};
class TradingTerminal {          // BACKEND TAB
    // Order and risk management
};
class EngineMonitor {            // ENGINE TAB
    // Quantum diagnostics
};
```

#### 1.2 Consolidate OANDA Integration
```cpp
// Current: Scattered OANDA connectors
// TradingHUD, MetricsDashboard, TradeManager

// Proposed: Centralized Service
class OandaService {
    // Unified connectivity and rate limiting
    void subscribeToTicks(TickSubscriber* subscriber);
    void subscribeToCandles(CandleSubscriber* subscriber);
};
```

### Phase 2: Component Organization by Tab
#### 2.1 SIGNALS Tab Architecture
```cpp
class SignalsTabController {
private:
    std::unique_ptr<SignalAnalysisPanel> signal_panel_;
    std::unique_ptr<MarketChartRenderer> chart_renderer_;
    std::unique_ptr<SignalHistoryTracker> history_tracker_;
    std::shared_ptr<OandaService> oanda_service_;
public:
    void render();
    void updateSignals();
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
    void render();
    void updateEngineMetrics();
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
    void render();
    void updateTradingData();
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

// Events
struct SEPSignalEvent { /* signal data */ };
struct OrderExecutedEvent { /* order details */ };
```

#### 3.2 Shared Data Services
```cpp
class MarketDataService {
    // Manages historical and real-time data
};
class SignalManager {
    // Processes and distributes signals
};
```

## Implementation Priority
1. **SIGNALS Tab** (Immediate Value):
   - Extract signal processing from TradingHUD.
   - Implement SignalAnalysisPanel and MarketChartRenderer.
2. **BACKEND Tab** (Critical Infrastructure):
   - Extract trading operations.
   - Build TradingTerminal and integrate TradeManager.
3. **ENGINE Tab** (Advanced Features):
   - Refactor MetricsDashboard for diagnostics.
   - Add research tools.

## UI/UX Design Principles
- **SIGNALS Tab**: Clean, real-time updates, color-coded signals, progressive disclosure.
- **ENGINE Tab**: Information-dense, customizable views, advanced controls, export capabilities.
- **BACKEND Tab**: Reliable, clear status indicators, audit trail, safety-first design.

## Migration Path
1. **Architecture Setup**: Create tab controllers and EventBus.
2. **SIGNALS Tab**: Extract signal processing and charts.
3. **BACKEND Tab**: Build trading terminal.
4. **ENGINE Tab**: Refactor diagnostics.
5. **Integration & Testing**: Connect tabs, optimize performance.

## Success Metrics
- **Reduced Complexity**: TradingHUD split into <200-line components.
- **Clear Separation**: Non-overlapping tab responsibilities.
- **Maintainability**: Independent component updates.
- **User Experience**: Task-focused tabs.
- **Performance**: Reduced resource usage.

## Current Status
- **Implemented**: Basic dashboard (`workbench_core.cpp`), partial SignalsTabController (`signals_tab_controller.cpp`).
- **In Progress**: Metrics integration (Phase 1.2 in [TODO.md](TODO.md)).
- **Next Steps**: Complete Signals Tab, address ImGui warnings (`report.md`).

This architecture will transform the GUI into a scalable, user-friendly interface aligned with the SEP Engine's trading objectives.

