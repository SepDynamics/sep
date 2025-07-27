# SEP Workbench TODO

## Phase 1: Data Pipeline Stabilization

**Objective:** Fix critical bugs and stabilize the data pipeline to enable real-time data processing and signal generation.

-   **Task 1.1: Fix OANDA API Request**
    -   **Description:** The application is currently sending a `count` parameter along with `to` and `from` parameters in the OANDA API request, which is causing an error. This needs to be fixed to enable reliable data fetching.
    -   **File:** `src/apps/workbench/core/workbench_core.cpp`
    -   **Status:** Not Started

-   **Task 1.2: Implement Asynchronous Data Fetching**
    -   **Description:** The application currently freezes during the initial data pull from OANDA. This is likely because the data is being fetched synchronously on the main thread. This needs to be moved to a separate thread to keep the UI responsive.
    -   **Files:** `src/apps/workbench/core/workbench_core.cpp`, `src/connectors/oanda_connector.cpp`
    -   **Status:** Not Started

-   **Task 1.3: Investigate "Buy-Only" Signals**
    -   **Description:** The `PatternMetricEngine` is currently only generating "BUY" signals. This is highly suspicious and needs to be investigated. The signal generation logic in `workbench_core.cpp` and the `PatternMetricEngine` itself need to be analyzed.
    -   **Files:** `src/apps/workbench/core/workbench_core.cpp`, `src/engine/pattern_metric_engine.cpp`
    -   **Status:** Not Started

## Phase 2: Data Pipeline Testing and Refinement

**Objective:** Create a robust testing framework for the data pipeline and refine the signal generation logic.

-   **Task 2.1: Create a Testbed for the `PatternMetricEngine`**
    -   **Description:** Create a new executable that can be used to test the `PatternMetricEngine` in isolation. This will allow for controlled experiments with different datasets and configurations.
    -   **Files:** `examples/CMakeLists.txt`, new files in `examples/`
    -   **Status:** Not Started

-   **Task 2.2: Implement Incremental Data Processing**
    -   **Description:** The application should be able to process data in incremental chunks, rather than all at once. This is crucial for real-time data processing.
    -   **Files:** `src/apps/workbench/core/workbench_core.cpp`, `src/connectors/oanda_connector.cpp`
    -   **Status:** Not Started

-   **Task 2.3: Refine Signal Generation Logic**
    -   **Description:** Based on the findings from the `PatternMetricEngine` testbed, refine the signal generation logic to produce more accurate and reliable signals.
    -   **Files:** `src/apps/workbench/core/workbench_core.cpp`, `src/engine/pattern_metric_engine.cpp`
    -   **Status:** Not Started

## Phase 3: Advanced Feature Implementation

**Objective:** Implement the advanced features described in the patent documents.

-   **Task 3.1: Implement QBSA**
    -   **Description:** Implement the Quantum Bit State Analysis (QBSA) algorithm for pattern correction and collapse detection.
    -   **Files:** `src/quantum/qbsa.h`, `src/quantum/qbsa.cpp`
    -   **Status:** Not Started

-   **Task 3.2: Implement Quantum Manifold Optimizer**
    -   **Description:** Implement the Quantum Manifold Optimizer for pattern enhancement.
    -   **Files:** `src/quantum/quantum_manifold_optimizer.h`, `src/quantum/quantum_manifold_optimizer.cpp`
    -   **Status:** Not Started

-   **Task 3.3: Implement Pattern Evolution System**
    -   **Description:** Implement the Pattern Evolution System for adaptive pattern optimization.
    -   **Files:** `src/quantum/pattern_evolution.h`, `src/quantum/pattern_evolution.cpp`
    -   **Status:** Not Started
