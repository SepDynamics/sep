# SEP Workbench: Dynamic GUI Behavior

## I. Overview

This document specifies the dynamic behavior of the SEP Workbench GUI in the new real-time architecture. The GUI must be able to render a continuous stream of market data and SEP signals in a clear, performant, and thread-safe manner.

## II. Components

### 1. Candlestick Chart

- **Description:** The main chart displaying the market data.
- **Behavior:**
  - The chart will display the `CandleData` from the `RollingWindow`.
  - As new `CandleData` arrives, the chart will scroll to the right, keeping the latest candle in view.
  - The x-axis will represent time, and the y-axis will represent price.

### 2. SEP Signal Charts

- **Description:** Three separate line charts to display the Coherence, Entropy, and Stability signals.
- **Behavior:**
  - Each chart will display the corresponding value from the `SEPSignal`s in the `SignalHistoryStore`.
  - The charts will be plotted against time, aligned with the Candlestick Chart.
  - As new `SEPSignal`s are generated, the line charts will extend to the right.

## III. Thread-Safe Rendering

To prevent race conditions and ensure smooth rendering, the following thread-safety mechanism will be implemented:

1.  **Mutex:** The `SignalHistoryStore` will be protected by a `std::mutex`.
2.  **Data Copying:** The GUI rendering thread (in `signals_tab_controller.cpp`) will lock the mutex, create a temporary copy of the required data from the `SignalHistoryStore`, and then immediately unlock the mutex.
3.  **Rendering:** The GUI will render the charts using the copied data. This ensures that the rendering process does not block the main processing loop from writing new signals to the `SignalHistoryStore`.

## IV. Implementation Details

- **Framework:** The existing GUI framework will be used.
- **File:** The primary file for GUI updates will be `src/apps/workbench/tabs/signals_tab_controller.cpp`.
- **Data Source:** The GUI will read data from the `SignalHistoryStore` as described above.
