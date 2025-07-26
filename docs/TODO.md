
# SEP Engine Development Roadmap

## Current Objective: **Resolve Critical Build Failures**

The primary goal is to restore the project to a compilable state. All other development, including chart rendering and feature integration, is blocked until the build errors are fixed. This roadmap is now realigned to prioritize build stabilization above all else.

## Phase 1: Critical Build Fixes & Testbed Completion

This phase is entirely focused on fixing the build and establishing a stable foundation for future work.

### 1.1: Critical Build Error Resolution (Top Priority)
-   **Status**: **BLOCKED**. The project fails to compile due to multiple, cascading errors.
-   **Priority**: **CRITICAL (BLOCKING)**
-   **Estimated Time**: 3-5 days
-   **Tasks**:
    -   [ ] **Decouple Core Logic from GUI**:
        -   **Action**: Create `src/common/financial_data_types.h` and move `CandleData`, `SEPSignalData`, and `CorrelationMetrics` into it.
        -   **Action**: Identify and remove the include chain that causes `oanda_connector.cpp` to depend on `ui_layout_manager.h` and thus `imgui.h`. Backend components must not depend on GUI libraries.
        -   **Action**: Update `data_parser.cpp` to include the new common types header to resolve `incomplete type 'sep::workbench::CorrelationMetrics'` errors.
    -   [ ] **Fix Namespace and Refactoring Mismatches**:
        -   **Action**: Standardize all modules on `sep::common::CandleData` to resolve the namespace conflicts in `multi_timeframe_analyzer.cpp`, `backtester.cpp`, and `json_data_parser.cpp`.
        -   **Action**: In `multi_timeframe_analyzer.cpp`, update the function signatures for `ingestMarketData`, `ingestHistoricalData`, `resampleCandles`, and `analyzeTimeframe` to match their declarations in the header file.
        -   **Action**: In `data_loader_test.cpp`, update method calls from `loadData` and `getCandleData` to their current correct names (`load_data` and `get_data`).
        -   **Action**: Correct the `std::chrono::time_point` assignment errors in `data_parser.cpp` and `oanda_connector.cpp` by properly parsing the string timestamps.
    -   [ ] **Resolve Missing Header Includes**:
        -   **Action**: Fix the `'backtester/data_loader.h' file not found` errors in workbench files by correcting the `target_include_directories` in `apps/workbench/CMakeLists.txt`.
        -   **Action**: Ensure `implot` is properly configured as a dependency so `implot.h` can be included in GUI components without errors.
    -   [ ] **Address Minor Compilation Errors**:
        -   **Action**: Add `#include <cstdint>` to `emitterutils.cpp` to resolve `uint16_t` and `uint32_t` undeclared identifier errors.
        -   **Action**: Fix any remaining typos or minor syntax errors identified in the build log.

### 1.2: 48-Hour Sample Data Setup
-   **Status**: **BLOCKED**
-   **Priority**: High (Post-Build-Fix)
-   **Dependencies**: **Phase 1.1 Complete**
-   **Tasks**:
    -   [ ] Verify `OandaConnector` can fetch and save 48 hours of EUR/USD M1 data.
    -   [ ] Implement `DataLoader` to load this sample data into the workbench.

### 1.3: Chart Rendering in Signals Tab
-   **Status**: **BLOCKED**
-   **Priority**: Critical (Post-Build-Fix)
-   **Dependencies**: **Phase 1.1 Complete**
-   **Tasks**:
    -   [ ] Render candlestick charts using `implot` in `SignalsTabController`.
    -   [ ] Add real-time plots for coherence, stability, and entropy.
    -   [ ] Implement zoom/pan functionality.

### 1.4: Pattern Discovery Framework
-   **Status**: **BLOCKED**
-   **Priority**: High (Post-Build-Fix)
-   **Dependencies**: **Phase 1.1 Complete**
-   **Tasks**:
    -   [ ] Implement threshold detection in `PatternMetricEngine` based on `SignalThresholds`.
    -   [ ] Create UI panel in `SignalsTab` for adjusting thresholds.
    -   [ ] Visualize generated signals as overlays on the candlestick chart.

## Phase 2: Trading Preparation (Post-Build-Fix)

### 2.1: Backtesting Framework
-   **Status**: **BLOCKED**
-   **Priority**: High
-   **Dependencies**: **Phase 1 Complete**
-   **Tasks**:
    -   [ ] Integrate `PatternMetricEngine` signals into the `Backtester`.
    -   [ ] Implement UI for backtesting in the `BackendTab`.

### 2.2: Risk & Order Management
-   **Status**: **BLOCKED**
-   **Priority**: High
-   **Dependencies**: **Phase 1 Complete**
-   **Tasks**:
    -   [ ] Implement position sizing, stop-loss, and take-profit logic in `TradeManager`.
    -   [ ] Integrate OANDA Trading API for order placement and tracking.

## Technical Debt & Static Analysis Issues (Lower Priority)

-   **Static Analysis Findings**: The static analysis report (`report.md`) found numerous `HIGH` and `MEDIUM` severity issues, primarily in third-party libraries like `imgui` and `yaml-cpp`.
-   **Tasks**:
    -   [ ] Investigate and address `[HIGH]` severity issues in `imgui` source code (`bugprone-incorrect-roundings`, `bugprone-sizeof-expression`). These may require patching or updating the library.
    -   [ ] Address `[MEDIUM]` `cert-err33-c` (ignored `fprintf`/`sprintf` return value) warnings in `imgui` to improve code quality.
    -   [ ] Review `[MEDIUM]` `bugprone-undefined-memory-manipulation` warnings (`memset` on non-TriviallyCopyable types) in `imgui` and replace with proper C++ initialization.