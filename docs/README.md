# SEP Engine - Quantum-Inspired Trading & Analysis Platform

## Current Status: Build Successful - Feature Development In Progress

The SEP Engine is a C++/CUDA-based platform designed for real-time analysis of financial market data using quantum-inspired algorithms. The project has successfully completed a major architectural refactoring, decoupling the core engine from the GUI and resolving all resulting compilation errors.

With a stable, compilable foundation, the project has now moved into the feature implementation phase. The immediate focus is on rendering market data, integrating live data streams, and building out the core functionalities of the workbench application.

## Primary Objective: Implement Core Workbench Features

With the build stabilized, the new objective is to implement the core features of the trading workbench and address the technical debt identified by static analysis.

1.  **Implement Chart Rendering**: Develop interactive candlestick and metric charts in the Signals Tab using ImPlot.
2.  **Integrate Live Data**: Connect the OANDA data feed to the GUI and the pattern analysis engine.
3.  **Build Out UI Functionality**: Implement the Engine Diagnostics and Backend Operations tabs.
4.  **Address Technical Debt**: Systematically fix the high-priority issues identified in the static analysis report to improve code quality and reliability.

## Architecture

The platform is built on a decoupled architecture that ensures the core engine, data connectors, and quantum algorithms are independent of the GUI.

-   **Data Pipeline**: Market Data (Live/Historical) → `OandaConnector` → `DataParser` → `PatternMetricEngine` (CUDA-accelerated) → Real-time Analysis (Coherence, Stability, Entropy) → Signal Generation.
-   **GUI**: A 3-tab workbench built with ImGui/ImPlot for **Signals**, **Engine Diagnostics**, and **Backend Operations**.

## Key Components Status

-   **Core Engine & Backend**: **Compiling Successfully.** The decoupling from the GUI is complete. The engine now forms a stable base for the application.
-   **DataParser**: **Compiling Successfully.** All type mismatches and API inconsistencies from the refactoring have been resolved.
-   **OandaConnector**: **Compiling Successfully.** The connector is fully operational and independent of the GUI layer.
-   **Workbench & UI**: **Compiling Successfully.** The application shell is stable. The focus is now on implementing the UI components within each tab.

## Next Milestones

1.  **Chart Rendering (In Progress)**: Implement candlestick and metric charts in the Signals Tab.
2.  **Live Data Integration**: Connect the OANDA data feed to the charts and engine.
3.  **Signal Validation**: Implement the backtesting UI and validate signal generation logic against historical data.
4.  **Static Analysis Cleanup**: Address critical and high-priority issues identified in `report.md`.

## Build & Run

The project now compiles successfully. Use the provided scripts to build and run the workbench application.

```bash
# Build the project
./build.sh

# Run the workbench application
./run_workbench.sh
```

## Key Files

-   [`DATA.md`](DATA.md): Describes the now-operational data pipeline architecture.
-   [`GUI.md`](GUI.md): Outlines the workbench GUI architecture and implementation plan.
-   [`TODO.md`](TODO.md): Provides a detailed development roadmap, focusing on new features and technical debt.