# SEP Engine Workbench GUI Architecture

## Current Status: Build Successful, Ready for UI Implementation

The SEP Workbench GUI is now **compiling successfully**. The major architectural refactoring to decouple backend components from GUI headers is complete, and the resulting compilation errors have been resolved. The foundation is now stable and ready for the implementation of the planned user interface.

The primary focus is now on building out the user-facing components, starting with the critical data visualization charts in the **Signals Tab**.

## 3-Tab Architecture

The GUI is organized into three distinct tabs, each managed by its own controller to ensure a clean separation of concerns.

### Tab 1: **SIGNALS** - Trading & Market Analysis
-   **Purpose**: Provide a clean, interactive interface for signal visualization and market analysis.
-   **Layout**: A large area for an interactive candlestick chart with SEP signal overlays, alongside smaller plots for Coherence, Stability, and Entropy.

### Tab 2: **ENGINE** - SEP Engine Diagnostics
-   **Purpose**: Offer a deep dive into the quantum engine's performance and internal state.
-   **Layout**: Time series plots for core metrics, pattern frequency histograms, and a correlation matrix to visualize relationships between different data streams.

### Tab 3: **BACKEND** - Trading Operations & Backtesting
-   **Purpose**: Manage data sources, trade execution, and backtesting.
-   **Layout**: A trading terminal for order placement, a backtesting suite with performance charts, and a system monitor for API and data source status.

## Development Plan

### Phase 1: Critical Build Fixes & Foundation
This phase focused on creating a stable, compilable application.

#### 1.1: Decouple Core Logic from GUI
-   **Status**: **DONE**. Shared data structures (`CandleData`, `SEPSignalData`, etc.) were moved to `src/common/financial_data_types.h`. Backend components no longer depend on GUI headers.

#### 1.2: Fix Post-Refactoring Errors
-   **Status**: **DONE**. All build errors related to timestamp handling, namespace issues, outdated struct members, and incorrect API calls have been resolved.

### Phase 2: Tab-Specific Implementation (Current Focus)
This phase involves building out the UI and logic for each of the three main tabs.

-   **Action (In Progress)**: Implement the `SignalsTabController` with interactive candlestick charts and metric plots using `implot`.
-   **Action (Next)**: Implement the `EngineTabController` with diagnostic panels for engine metrics.
-   **Action (Next)**: Implement the `BackendTabController` with UI for trading and backtesting.

## Implementation Priority

1.  **Chart Rendering**: The highest priority is to implement robust, interactive candlestick charts in the `SignalsTabController` to visualize market data.
2.  **Data Flow Integration**: Connect the `OandaConnector` and `PatternMetricEngine` to the GUI tabs to display live data and SEP signals.
3.  **Backend & Engine Tabs**: Build out the UI for the remaining tabs, focusing on backtesting and engine diagnostics.
4.  **Static Analysis Cleanup**: Address the high-priority technical debt identified in `report.md` to improve code quality.

## Success Metrics
-   **Compilation**: The project builds successfully with zero errors.
-   **Chart Rendering**: Candlestick charts are visible and update at a fluid frame rate (>30Hz).
-   **Interactivity**: Users can zoom, pan, and inspect data points on the charts.
-   **Tab Separation**: UI components are logically separated into their respective controllers with clear responsibilities.
-   **Maintainability**: The core engine can be modified and built independently of the GUI.