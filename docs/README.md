# SEP Engine Trading Platform - Build Failure Resolution Phase

## Current Status: **CRITICAL BUILD FAILURE** - All Development Blocked

The SEP Engine is a quantum-inspired trading platform designed to process market data and generate predictive trading signals. The project is currently **completely blocked by critical compilation errors** that prevent the application from building and launching. The previous phase of integrating the core engine with a 3-tab workbench GUI is halted.

**The absolute top priority is to resolve all build errors to create a stable, compilable foundation.** Until the application builds successfully, no further progress on chart rendering, data processing, or feature integration is possible.

## Primary Objective: Fix the Build

1.  **Resolve Header Dependency Conflicts**: ✅
    -   **Top Priority**: Refactor core data structures (e.g., `CandleData`, `CorrelationMetrics`) out of GUI-specific headers and into neutral, shared libraries. This will fix the `fatal error: 'imgui.h' file not found` and `incomplete type` errors in the core engine and backend connectors.

2.  **Correct Refactoring Errors**: ✅
    -   Update source files (`.cpp`) to match their corresponding header (`.h`) declarations, resolving `out-of-line definition does not match` errors.
    -   Fix incorrect API usage and typos (e.g., `loadData` vs. `load_data` in `data_loader_test.cpp`).
    -   Resolve missing header includes (e.g., `'backtester/data_loader.h' file not found`) by correcting CMake `include_directories` or file paths.

3.  **Address Miscellaneous Compilation Errors**: ✅
    -   Fix namespace issues where types are used without correct qualification (e.g., `sep::workbench::CandleData` vs. `sep::common::CandleData`).
    -   Add missing standard headers like `<cstdint>` where required.

## Architecture (Post-Build-Fix)

The intended architecture, detailed in [DATA.md](DATA.md) and [GUI.md](GUI.md), remains the goal once the project is compilable.

-   **Data Pipeline**: Market Data → Data Ingestion Layer → Quantum Processing → Real-time Analysis → Trading & Visualization
-   **GUI**: A 3-tab workbench for Signals, Engine Diagnostics, and Backend Operations.

## Key Components Status (Compilation)

-   **DataParser** (`src/engine/data_parser.cpp`): **BUILD FAILED.** Blocked by `incomplete type` errors for `CorrelationMetrics` due to a missing header include.
-   **MultiTimeframeAnalyzer** (`src/apps/workbench/core/multi_timeframe_analyzer.cpp`): **BUILD FAILED.** Multiple errors due to signature mismatches, namespace confusion between `workbench::CandleData` and `common::CandleData`, and incomplete type access.
-   **OandaConnector** (`src/connectors/oanda_connector.cpp`): **BUILD FAILED.** Blocked by a fatal header dependency on `imgui.h`. A backend connector must not depend on a GUI library.
-   **Backtester & Tests** (`backtester.cpp`, `data_loader_test.cpp`): **BUILD FAILED.** Blocked by outdated API usage (`loadData` vs `load_data`) and namespace errors for `CandleData`.
-   **Workbench Core & Tabs** (`service_proxy_engine.h`, `landing_page.cpp`, etc.): **BUILD FAILED.** Multiple `file not found` errors for `backtester/data_loader.h`, indicating an include path or refactoring issue.

## Next Milestones (Contingent on Build Fixes)

1.  **Achieve a Clean Build**: Successfully compile the entire project with zero errors. **This is the only current milestone.**
2.  **Render Charts**: Implement candlestick and metric charts in the Signals Tab.
3.  **Integrate Live Data**: Connect the data feed to the charts and engine.
4.  **Validate Signals**: Begin backtesting and validating signal generation logic.

## Build & Run (Currently Failing)

```bash
# The following commands will fail until the build errors are resolved.
./build.sh
./run_workbench.sh
```

## Key Files

-   [`DATA.md`](DATA.md): Data pipeline architecture and its current blocked state.
-   [`GUI.md`](GUI.md): Workbench GUI architecture and the refactoring plan to fix build issues.
-   [`TODO.md`](TODO.md): Detailed roadmap, with build fixes as the top priority.
