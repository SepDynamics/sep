# SEP Engine Development Roadmap

## Current Objective: **Resolve Post-Refactoring Build Errors**

The primary goal is to restore the project to a compilable state. The critical architectural refactoring is complete, but it has introduced a new set of build errors. All feature development remains blocked until these are fixed.

## Phase 1: Critical Build Fixes & Testbed Completion

This phase is entirely focused on fixing the new build errors and establishing a stable foundation for future work.

### 1.1: Critical Build Error Resolution (Top Priority)
-   **Status**: **IN PROGRESS**. The major architectural flaw is fixed, but cleanup is required.
-   **Priority**: **CRITICAL (BLOCKING)**
-   **Estimated Time**: 2-4 days
-   **Tasks**:
    -   [x] **Decouple Core Logic from GUI**:
        -   **Action**: Create `src/common/financial_data_types.h` and move shared data structures into it. (DONE)
        -   **Action**: Remove the include chain that caused backend components to depend on `imgui.h`. (DONE)
    -   [ ] **Fix Data Type and API Mismatches**:
        -   **Action**: Correct all `assigning to 'uint64_t' from incompatible type 'std::chrono::time_point'` errors. Implement a standard utility to convert `time_point` to a `uint64_t` epoch value (e.g., milliseconds or nanoseconds) and use it in `data_parser.cpp` and `oanda_connector.cpp`.
        -   **Action**: Update all modules to use the correct members for refactored structs like `sep::common::SEPSignalData` and `sep::common::CorrelationMetrics`. This will fix errors like `no member named 'coherence'` in `service_connector.cpp` and `signals_tab_controller.cpp`.
        -   **Action**: Fix the `no member named 'parseTimestamp' in namespace 'sep::common'` error by implementing or moving the utility function into `financial_data_types.h/cpp`.
        -   **Action**: Correct the API call in `service_connector.cpp` that fails with `no member named 'oanda' in 'sep::config::ConfigManager'`.
    -   [ ] **Standardize Namespaces and Includes**:
        -   **Action**: Resolve all `unknown type name 'OrderInfo'` and `unknown type name 'CandleData'` errors. Ensure `common/financial_data_types.h` is included and that types are referenced via `sep::common::`.
        -   **Action**: Fix the `no viable conversion` error in `engine_tab_controller.cpp` by ensuring all `CorrelationMetrics` objects use the `sep::common` namespace.
    -   [ ] **Correct Constructor Calls**:
        -   **Action**: Fix the `no matching constructor for initialization of 'sep::common::CandleData'` error in `json_data_parser.cpp` and `signals_tab_controller.cpp` by providing the correct arguments.
    -   [ ] **Address Minor Compilation and Static Analysis Errors**:
        -   **Action**: Add `#include <cstdint>` to `emitterutils.cpp` to resolve the `CRITICAL` static analysis finding for `uint16_t` and `uint32_t` undeclared identifiers.
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

## Phase 2: Trading Preparation (Post-Build-Fix)

### 2.1: Backtesting Framework
-   **Status**: **BLOCKED**
-   **Priority**: High
-   **Dependencies**: **Phase 1 Complete**
-   **Tasks**:
    -   [ ] Integrate `PatternMetricEngine` signals into the `Backtester`.
    -   [ ] Implement UI for backtesting in the `BackendTab`.

## Technical Debt & Static Analysis Issues (Lower Priority)

-   **Static Analysis Findings**: The static analysis report (`report.md`) found numerous `HIGH` and `MEDIUM` severity issues, primarily in third-party libraries.
-   **Tasks**:
    -   [ ] Address `[CRITICAL]` `undeclared identifier` error in `emitterutils.cpp` by including `<cstdint>`. (Moved to Phase 1.1)
    -   [ ] Investigate and address `[HIGH]` severity issues in `imgui` source code (`bugprone-incorrect-roundings`, `bugprone-sizeof-expression`). These may require patching or updating the library.
    -   [ ] Review `[MEDIUM]` `bugprone-undefined-memory-manipulation` warnings (`memset` on non-TriviallyCopyable types) in `imgui` and replace with proper C++ initialization.
