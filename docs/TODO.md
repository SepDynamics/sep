# SEP Workbench: Real-Time Architecture TODO

## I. Core Objective: Transition to a Continuous, Real-Time Processing Engine

The primary goal is to re-architect the SEP Workbench from a batch-processing application into a continuously running, real-time signal generation engine. The system will ingest a live stream of market data (pips), maintain a rolling window of recent data, and re-calculate SEP signals (Coherence, Entropy, Stability) for each new data point. The GUI must update dynamically to reflect these real-time signals.

This approach aligns with the principles outlined in the `Alpha_WP.md`, where signal verification is an act of continuous, incremental alignment with a latent market register.

--- 

## II. Architectural Blueprint & Task Breakdown

### Phase 1: Implement Core Components (In-Progress)

**Objective:** Build the foundational components of the real-time architecture.

-   **Task 1.1: Implement `RollingWindowManager`**
    -   **Description:** Create a class to manage a `std::deque` of `CandleData`.
    -   **Files:** `src/apps/workbench/core/rolling_window_manager.h`, `src/apps/workbench/core/rolling_window_manager.cpp`
    -   **Status:** Not Started

-   **Task 1.2: Implement `SEPSignalGenerator`**
    -   **Description:** Create a class to calculate SEP signals from the rolling window.
    -   **Files:** `src/apps/workbench/core/sep_signal_generator.h`, `src/apps/workbench/core/sep_signal_generator.cpp`
    -   **Status:** Not Started

-   **Task 1.3: Implement `SignalHistoryStore`**
    -   **Description:** Create a thread-safe class to store `SEPSignal` history.
    -   **Files:** `src/apps/workbench/core/signal_history_store.h`, `src/apps/workbench/core/signal_history_store.cpp`
    -   **Status:** Not Started

### Phase 2: Integrate Real-Time Pipeline (Next)

**Objective:** Connect the components into a functioning real-time pipeline.

-   **Task 2.1: Modify `oanda_connector` for Continuous Fetching**
    -   **Description:** Adapt the OANDA connector to run in a separate thread and push data to a thread-safe queue.
    -   **Files:** `src/connectors/oanda_connector.h`, `src/connectors/oanda_connector.cpp`
    -   **Status:** Not Started

-   **Task 2.2: Implement Main Processing Loop in `workbench_core`**
    -   **Description:** Refactor `workbench_core.cpp` to include the main `while` loop that orchestrates the data flow between components.
    -   **Files:** `src/apps/workbench/core/workbench_core.h`, `src/apps/workbench/core/workbench_core.cpp`
    -   **Status:** Not Started

### Phase 3: Update GUI for Dynamic Rendering (Finally)

**Objective:** Rework the GUI to visualize the real-time data stream.

-   **Task 3.1: Implement Thread-Safe Rendering in `signals_tab_controller`**
    -   **Description:** Modify the `signals_tab_controller` to read from the `SignalHistoryStore` in a thread-safe manner and update the charts.
    -   **Files:** `src/apps/workbench/tabs/signals_tab_controller.h`, `src/apps/workbench/tabs/signals_tab_controller.cpp`
    -   **Status:** Not Started

--- 

## III. Documentation

-   **[Completed] `docs/DATA.md`:** Detailed specification of all data structures and the data flow through the real-time system.
-   **[Completed] `docs/GUI.md`:** Detailed specification of the GUI components and their dynamic behavior.
-   **[In-Progress] `docs/TODO.md`:** This document.