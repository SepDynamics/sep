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
    -   [ ] **Refactor Core Data Structs**: Move `CandleData`, `TickData`, `SEPSignalData`, and other core financial types from `apps/workbench/core/common_structs.h` to a new, GUI-independent header: `src/common/financial_data_types.h`.
    -   [ ] **Decouple Engine from GUI**: Update all files that include `common_structs.h` for core types to use the new `financial_data_types.h` header instead. This will eliminate the `fatal error: 'imgui.h' file not found` in `engine.cu` and `data_parser.cpp`.
    -   [ ] **Correct ConfigManager Namespace**: In `service_connector.cpp`, change `sep::workbench::ConfigManager` to the correct `sep::config::ConfigManager`.
    -   [ ] **Fix Undeclared Identifiers**:
        -   In `signals_tab_controller.h`, define or include headers for `TechnicalIndicator`, `TrendLine`, `EnhancedHoverInfo`, and `ChartZoom` in a GUI-specific context.
        -   In `workbench_main.cpp` and `engine_tab_controller.h`, add necessary includes for `ServiceProxyEngine`, `backtester`, etc., and qualify with correct namespaces (e.g., `sep::core::ServiceProxyEngine`).
    -   [ ] **Resolve `implot.h` Dependency**: Ensure `implot.h` is correctly integrated into the build system (e.g., via `third_party`) and included in `signals_tab_controller.cpp`.
    -   [ ] **Fix Typo in PatternMetricEngine**: In `pattern_metric_engine.cpp`, correct the `undeclared identifier 's'` typo within the signal generation loop.
    -   [ ] **Add Missing Standard Header**: Add `#include <cstdint>` to `emitterutils.cpp` to define `uint16_t` and `uint32_t`.
    -   [ ] **Refactor Event System**: Create a central event header (e.g., `apps/workbench/core/events.h`) defining `OrderUpdateEvent` and `globalEventBus`, and include it in `oanda_connector.cpp` to resolve related errors.

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
    -   [ ] Address the `[CRITICAL]` `clang-diagnostic-error` in `emitterutils.cpp` (`uint16_t` undeclared). This is part of the main build fix.
    -   [ ] Investigate and address `[HIGH]` severity issues in `imgui` source code (`null dereference`, `incorrect roundings`). These may require patching or updating the library.
    -   [ ] Address `[MEDIUM]` `cert-err33-c` (ignored `sprintf` return value) and `security.FloatLoopCounter` warnings in `imgui_demo.cpp` to improve code quality. This is a lower priority than fixing the build.
```