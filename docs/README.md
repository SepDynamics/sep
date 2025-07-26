# SEP Engine Trading Platform - Post-Refactoring Cleanup Phase

## Current Status: **BUILD FAILING** - Post-Refactoring Cleanup Required

The SEP Engine is a quantum-inspired trading platform designed to process market data and generate predictive trading signals. The project has successfully resolved the critical architectural flaw where backend components depended on GUI libraries.

However, the build is **still failing due to numerous compilation errors** that emerged as a result of the major refactoring. The current top priority is to resolve these new errors to create a stable, compilable foundation for feature development.

## Primary Objective: Achieve a Clean Build

The previous objective to decouple core logic from the GUI has been **completed**. The new objective is to clean up all resulting API mismatches, type errors, and namespace conflicts.

1.  **Fix Timestamp and Data Type Mismatches**: A significant number of errors are due to incorrect assignments between `std::chrono::time_point` and `uint64_t`.
    -   **Top Priority**: Implement and use a consistent conversion utility to handle timestamps correctly across `data_parser.cpp`, `oanda_connector.cpp`, and other components.
    -   Ensure all data structures use their intended types (e.g., `uint64_t` for timestamps that will be stored or serialized).

2.  **Update Data Structure Usage**: The refactoring changed the members of core data structures like `SEPSignalData` and `CorrelationMetrics`.
    -   Update all code in `service_connector.cpp`, `workbench_core.cpp`, and `signals_tab_controller.cpp` to use the correct new member names (e.g., `signal_type` instead of older fields).
    -   Fix incorrect member access in `data_parser.cpp` for `CorrelationMetrics` (e.g., `coherence_pearson` no longer exists).

3.  **Resolve Namespace and Include Issues**: The move of data types to the `sep::common` namespace was successful but left many files with incorrect using-declarations or missing includes.
    -   Resolve all `unknown type name 'OrderInfo'` and `unknown type name 'CandleData'` errors by including `common/financial_data_types.h` and explicitly using the `sep::common` namespace.

4.  **Correct API and Constructor Mismatches**:
    -   Fix calls to outdated APIs, such as the missing `oanda` member in `ConfigManager`.
    -   Update `json_data_parser.cpp` and `signals_tab_controller.cpp` to use the correct constructor signature for `sep::common::CandleData`.

## Architecture (Post-Build-Fix)

The intended architecture, detailed in [DATA.md](DATA.md) and [GUI.md](GUI.md), remains the goal once the project is compilable.

-   **Data Pipeline**: Market Data → Data Ingestion Layer → Quantum Processing → Real-time Analysis → Trading & Visualization
-   **GUI**: A 3-tab workbench for **Signals**, **Engine Diagnostics**, and **Backend Operations**.

## Key Components Compilation Status

-   **DataParser** (`src/engine/data_parser.cpp`): **BUILD FAILED.** Multiple errors assigning `std::chrono::time_point` to `uint64_t`. Calls to a missing `sep::common::parseTimestamp` function. Accessing non-existent members of `CorrelationMetrics`.
-   **OandaConnector** (`src/connectors/oanda_connector.cpp`): **BUILD FAILED.** The critical dependency on `imgui.h` is **RESOLVED**. Now failing due to `unknown type name 'OrderInfo'`, indicating a namespace/include error after refactoring.
-   **ServiceConnector & Workbench Core** (`service_connector.cpp`, `workbench_core.cpp`): **BUILD FAILED.** Multiple errors from using outdated `SEPSignalData` and `CandleData` struct members. `ConfigManager` API has changed.
-   **Backtester & UI** (`json_data_parser.cpp`, `backtester_tab_controller.h`): **BUILD FAILED.** `CandleData` constructor call is incorrect. Namespace for `OrderInfo` is wrong.

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
