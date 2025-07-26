# SEP Engine Workbench GUI Architecture

## Current Problem Analysis: **BUILD FAILING - Post-Refactoring Cleanup**

The SEP Workbench GUI is currently **unbuildable and non-functional**. The previous critical architectural flaw (backend components depending on `imgui.h`) has been **resolved**. However, this major refactoring has introduced a new set of compilation errors related to API changes, namespace inconsistencies, and outdated data structure usage.

Chart rendering and all other GUI features remain completely blocked until these new build errors are fixed.

### Primary Compilation Blockers
-   **Fatal Header Conflict**: **RESOLVED.** The decoupling of core logic from GUI headers was successful. Backend files no longer include `imgui.h`.
-   **Namespace Conflicts & Missing Includes**: The primary issue is now inconsistent use of namespaces. Code frequently attempts to use types like `sep::connectors::OrderInfo` when they have been moved to `sep::common::OrderInfo`. This causes numerous `unknown type name` errors.
-   **Refactoring Mismatches**: The codebase is filled with errors resulting from the refactoring:
    -   **Incorrect Struct Members**: Code in `service_connector.cpp` and `signals_tab_controller.cpp` attempts to access members of `SEPSignalData` and `CorrelationMetrics` that no longer exist.
    -   **Incorrect Constructors**: `json_data_parser.cpp` fails to build because it calls a constructor for `sep::common::CandleData` with the wrong number of arguments.
    -   **API Changes**: The `ConfigManager` API was changed, but not all call sites were updated.
-   **Data Type Mismatches**: A widespread issue is the incorrect assignment of `std::chrono::time_point` objects to `uint64_t` variables in `data_parser.cpp`, causing type errors.

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
This phase focuses exclusively on making the project compilable again by cleaning up after the major refactoring.

#### 1.1: Decouple Core Logic from GUI
-   **Status**: **DONE**. All non-GUI data structures (`CandleData`, `SEPSignalData`, `CorrelationMetrics`) have been moved to `src/common/financial_data_types.h`. Backend components no longer depend on GUI headers.

#### 1.2: Fix Post-Refactoring Errors
-   **Action**: **Fix Timestamp Handling**: Create a utility function to convert `std::chrono::time_point` to `uint64_t` and use it in `data_parser.cpp` and `oanda_connector.cpp` to resolve assignment errors.
-   **Action**: **Standardize Namespaces**: Go through all compilation errors and ensure shared types like `OrderInfo`, `CandleData`, and `CorrelationMetrics` are included from `common/financial_data_types.h` and referenced via the `sep::common` namespace.
-   **Action**: **Update Data Structure Usage**: Correct all member access for refactored structs like `SEPSignalData` to use the new fields (`signal_type`, `timestamp`, etc.).
-   **Action**: **Correct Constructor and API Calls**: Fix the `CandleData` constructor call in `json_data_parser.cpp`. Update code using `ConfigManager` to match its new API.

### Phase 2: Tab-Specific Implementation (Post-Build-Fix)
-   **Action**: Implement the `SignalsTabController` with candlestick charts and metric plots using `implot`.
-   **Action**: Implement the `EngineTabController` with diagnostic panels for engine metrics.
-   **Action**: Implement the `BackendTabController` with UI for trading and backtesting.

## Implementation Priority

1.  **Critical Build Errors (IMMEDIATE)**: Address all compilation errors listed in Phase 1.2 of the refactoring plan. **This is the single highest priority and blocks all other work.**
2.  **Chart Rendering** (Post-Build-Fix): Implement robust candlestick charts in `SignalsTabController`.
3.  **Data Flow Integration** (Post-Build-Fix): Connect the OANDA connector and `PatternMetricEngine` to the GUI tabs.
4.  **Backend & Engine Tabs** (Post-Build-Fix): Build out the UI for the remaining tabs.

## Success Metrics
-   **Compilation**: The project builds successfully with zero errors.
-   **Chart Rendering**: Candlestick charts are visible and update at >10Hz.
-   **Tab Separation**: UI components are logically separated with clear responsibilities.
-   **Maintainability**: Core engine can be modified and built without affecting the GUI.
