# SEP Engine Workbench GUI Architecture

## Current Problem Analysis: **CRITICAL BUILD FAILURE**

The SEP Workbench GUI is currently **unbuildable and non-functional**. Critical compilation errors, primarily stemming from an architectural flaw where core engine components depend on GUI-specific headers (`imgui.h`), prevent the application from launching. Chart rendering, a key feature, is completely blocked.

The immediate goal is to refactor the code to resolve these build errors. The proposed 3-tab architecture remains the target design *after* the build is stabilized.

### Primary Compilation Blockers
-   **Fatal Header Conflict**: Core backend files like `oanda_connector.cpp` and `data_parser.cpp` fail to build because they indirectly include `imgui.h`. This is the most severe issue and points to an incorrect dependency chain.
-   **Missing/Incorrect Includes**: Multiple files fail due to missing headers (e.g., `'backtester/data_loader.h'`) or using forward declarations where full definitions are needed (leading to `incomplete type` errors).
-   **Refactoring Mismatches**: Numerous errors like `out-of-line definition does not match` and `no member named 'loadData'` indicate that source files were not updated after their corresponding headers were changed.
-   **Namespace Conflicts**: The codebase is inconsistent in its use of `sep::workbench::CandleData` vs. `sep::common::CandleData`, causing type mismatch errors.

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
-   **Action**: Create `src/common/financial_data_types.h`. Move all non-GUI data structures (`CandleData`, `SEPSignalData`) into this new header.
-   **Action**: Create a new GUI-specific types header (e.g., `apps/workbench/core/ui_types.h`) for structs that use ImGui types like `ChartZoom` and `EnhancedHoverInfo`.
-   **Action**: Update all core engine and connector files (`data_parser.h`, `oanda_connector.cpp`, etc.) to include `financial_data_types.h` and remove any direct or indirect dependency on GUI headers. This will eliminate the `fatal error: 'imgui.h' file not found` in backend components.
-   **Action**: Ensure full definitions of structs like `CorrelationMetrics` and `CandleData` are included before they are used to fix all `incomplete type` errors.

#### 1.2: Fix Namespace and Refactoring Errors
-   **Action**: Standardize on the `sep::common` namespace for shared financial data types across the entire project to resolve `no viable conversion` errors.
-   **Action**: Update the implementations in `multi_timeframe_analyzer.cpp` to match the function signatures declared in its header.
-   **Action**: Correct method names in test files, such as changing `dataLoader.loadData(...)` to `dataLoader.load_data(...)` in `data_loader_test.cpp`.

#### 1.3: Resolve Missing Dependencies and Includes
-   **Action**: Correct the include path for `backtester/data_loader.h` in all workbench files. This may require updating `target_include_directories` in the relevant `CMakeLists.txt`.
-   **Action**: Properly integrate `implot` as a third-party dependency so `implot.h` can be included in GUI components.

### Phase 2: Tab-Specific Implementation (Post-Build-Fix)
-   **Action**: Implement the `SignalsTabController` with candlestick charts and metric plots using `implot`.
-   **Action**: Implement the `EngineTabController` with diagnostic panels for engine metrics.
-   **Action**: Implement the `BackendTabController` with UI for trading and backtesting.

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