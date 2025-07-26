# SEP Engine Development Roadmap

## Current Objective: Implement Core Features and Address Technical Debt

The project has successfully resolved all post-refactoring compilation errors and now has a stable, buildable foundation. The primary objective is to implement the core features of the trading workbench and address high-priority technical debt identified by static analysis.

## Phase 1: Feature Implementation & Stabilization

This phase focuses on bringing the core functionalities of the workbench to life and improving code quality.

### 1.1: Critical Build Error Resolution
-   **Status**: **DONE**
-   **Summary**: The architectural decoupling of the backend from the GUI is complete. All resulting compilation errors related to type mismatches, API changes, and namespace issues have been resolved. The project now compiles successfully.

### 1.2: Implement Chart Rendering (In Progress)
-   **Status**: **IN PROGRESS**
-   **Priority**: **CRITICAL (BLOCKING)**
-   **Tasks**:
    -   [ ] Render candlestick charts in `SignalsTabController` using `implot`.
    -   [ ] Add real-time plots below the main chart for Coherence, Stability, and Entropy metrics.
    -   [ ] Implement interactive features like zoom, pan, and crosshair for chart analysis.

### 1.3: Integrate Live Data Feed
-   **Status**: **BLOCKED**
-   **Priority**: High
-   **Dependencies**: **Phase 1.2 Complete**
-   **Tasks**:
    -   [ ] Connect the `OandaConnector`'s live price stream to the `MultiTimeframeAnalyzer`.
    -   [ ] Ensure the `SignalsTabController` chart updates in real-time with new data.
    -   [ ] Feed live data into the `PatternMetricEngine` to generate real-time signals.

### 1.4: Backtesting Framework Implementation
-   **Status**: **BLOCKED**
-   **Priority**: High
-   **Dependencies**: **Phase 1.3 Complete**
-   **Tasks**:
    -   [ ] Fully integrate the `PatternMetricEngine` signals into the `Backtester` logic.
    -   [ ] Implement the UI for loading data, running backtests, and viewing results in the `BackendTabController`.
    -   [ ] Display performance metrics (P&L, Win Rate, Sharpe Ratio) and an equity curve chart.

## Phase 2: Technical Debt and Static Analysis Cleanup

This phase runs in parallel with feature development and focuses on improving code quality based on the `report.md` findings.

### 2.1: Critical Static Analysis Fixes
-   **Status**: **IN PROGRESS**
-   **Priority**: High
-   **Tasks**:
    -   [ ] **`emitterutils.cpp`**: Address `CRITICAL` `undeclared identifier` errors by including `<cstdint>`.
    -   [x] **`glm` Experimental Extensions**: `glm_config.h` is now force-included for all targets so `GLM_ENABLE_EXPERIMENTAL` is always defined.
    -   [ ] **PipeWire Null Dereference**: Investigate the `HIGH` severity null pointer dereference in `/usr/include/spa-0.2/spa/pod/parser.h`. This is likely an external library issue but should be understood.

### 2.2: High-Priority Code Quality Improvements
-   **Status**: **NOT STARTED**
-   **Priority**: Medium
-   **Tasks**:
    -   [x] **`imgui` / `implot` Issues**: Updated vendor versions to ImGui `1.92.1` and ImPlot `0.17`. Remaining warnings from these libraries are documented but accepted.
    -   [ ] **`sep` Source Code**:
        -   Address `clang-diagnostic-double-promotion` warnings across the codebase (e.g., in `data_parser.cpp`, `oanda_connector.cpp`, `sep_demo_app.cpp`) to ensure floating-point precision is handled correctly.
        -   Fix `deadcode.DeadStores` and `unused-parameter` warnings to improve code clarity and remove unnecessary variables.
        -   Review and fix potential sign-comparison issues (`clang-diagnostic-sign-compare`) in `oanda_connector.cpp`.