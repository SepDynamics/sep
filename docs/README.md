# SEP Engine Trading Platform - Build Failure Resolution Phase

## Current Status: **CRITICAL BUILD FAILURE** - All Development Blocked

The SEP Engine is a quantum-inspired trading platform designed to process OANDA market data and generate predictive trading signals. The project is currently **completely blocked by critical compilation errors** that prevent the application from building and launching. The previous phase of integrating the core engine with a 3-tab workbench GUI is halted.

**The absolute top priority is to resolve all build errors to create a stable, compilable foundation.** Until the application builds successfully, no further progress on chart rendering, data processing, or feature integration is possible.

## Primary Objective: Fix the Build

1.  **Resolve Header Dependency Conflicts**: ✅
    -   **Top Priority**: Refactor core data structures (e.g., `CandleData`) out of GUI-specific headers (`apps/workbench/core/common_structs.h`) and into a neutral, shared library (`src/common/financial_data_types.h`). This will fix the `fatal error: 'imgui.h' file not found` errors in the core engine and CUDA kernels.

2.  **Correct Namespace and API Usage**: ✅
    -   Fix incorrect `ConfigManager` access in `service_connector.cpp` (must be `sep::config::ConfigManager`).
    -   Resolve undeclared identifiers (`backtester`, `ServiceProxyEngine`, `TechnicalIndicator`, etc.) by adding the necessary `#include` directives and namespace qualifications.
    -   Address missing `globalEventBus` and `OrderUpdateEvent` usage in `oanda_connector.cpp` by creating and including a proper event-handling header.

3.  **Address Miscellaneous Compilation Errors**: ✅
    -   Fix the typo (`undeclared identifier 's'`) in `pattern_metric_engine.cpp`.
    -   Resolve the missing `implot.h` dependency for the Signals Tab.
    -   Add missing `<cstdint>` include in `emitterutils.cpp`.

## Architecture (Post-Build-Fix)

The intended architecture, detailed in [DATA.md](DATA.md) and [GUI.md](GUI.md), remains the goal once the project is compilable.

-   **Data Pipeline**: OANDA Market Data → Quantum Pattern Engine → Workbench Dashboard → Signal Analysis
-   **GUI**: A 3-tab workbench for Signals, Engine Diagnostics, and Backend Operations.

## Key Components Status (Compilation)

-   **OandaConnector** (`src/connectors/oanda_connector.cpp`): **BUILD FAILED.** Blocked by missing `globalEventBus` and `OrderUpdateEvent` types.
-   **PatternMetricEngine** (`src/quantum/pattern_metric_engine.cpp`): **BUILD FAILED.** Blocked by an `undeclared identifier 's'` typo. Also blocked by `engine.cu`'s dependency on the problematic `common_structs.h`.
-   **DataParser** (`src/engine/data_parser.cpp`): **BUILD FAILED.** Blocked by dependency on `common_structs.h` which incorrectly includes `imgui.h`.
-   **ServiceConnector** (`src/apps/workbench/core/service_connector.cpp`): **BUILD FAILED.** Blocked by incorrect namespace for `ConfigManager` and undeclared `DataLoader`.
-   **SignalsTabController** (`src/apps/workbench/tabs/signals_tab_controller.cpp`): **BUILD FAILED.** Blocked by missing `implot.h` and undeclared chart-related types.
-   **Workbench Core** (`src/apps/workbench/core/workbench_main.cpp`): **BUILD FAILED.** Blocked by multiple undeclared type errors stemming from header issues in its dependencies.

## Next Milestones (Contingent on Build Fixes)

1.  **Achieve a Clean Build**: Successfully compile the entire project with zero errors. **This is the only current milestone.**
2.  **Render Charts**: Implement candlestick and metric charts in the Signals Tab.
3.  **Integrate Live Data**: Connect the OANDA data feed to the charts.
4.  **Validate Signals**: Begin backtesting and validating signal generation logic.

## Build & Run (Currently Failing)

```bash
./build.sh          # Expected to FAIL until errors are resolved
./run_workbench.sh  # Cannot be run
```

## Key Files

-   [`DATA.md`](DATA.md): Data pipeline architecture and its current blocked state.
-   [`GUI.md`](GUI.md): Workbench GUI architecture and the refactoring plan to fix build issues.
-   [`TODO.md`](TODO.md): Detailed roadmap, with build fixes as the top priority.
```