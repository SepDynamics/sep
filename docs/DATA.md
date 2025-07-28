# SEP Workbench: Real-Time Data Structures and Flow

## I. Overview

This document specifies the data structures and the flow of data through the real-time processing architecture of the SEP Workbench. The new architecture is designed to handle a continuous stream of market data, process it in real-time, and generate SEP signals for dynamic visualization.

## II. Data Structures

### 1. `CandleData`

- **Description:** Represents a single OHLCV (Open, High, Low, Close, Volume) data point.
- **Location:** `src/common/candle_data.h`
- **Fields:**
  - `uint64_t timestamp`: The timestamp of the candle.
  - `float open`: The opening price.
  - `float high`: The highest price.
  - `float low`: The lowest price.
  - `float close`: The closing price.
  - `int volume`: The trading volume.

### 2. `SEPSignal`

- **Description:** Represents a single calculated SEP signal, which is a triplet of Coherence, Entropy, and Stability.
- **Location:** `src/apps/workbench/core/workbench_core.hpp` (to be created)
- **Fields:**
  - `uint64_t timestamp`: The timestamp of the signal, corresponding to the latest data point used in the calculation.
  - `float coherence`: The coherence value.
  - `float entropy`: The entropy value.
  - `float stability`: The stability value.

### 3. `RollingWindow`

- **Description:** A rolling window of `CandleData` points.
- **Implementation:** `std::deque<CandleData>`
- **Location:** `RollingWindowManager` class.

### 4. `SignalHistory`

- **Description:** A history of generated `SEPSignal` points.
- **Implementation:** `std::vector<SEPSignal>`
- **Location:** `SignalHistoryStore` class.
- **Thread Safety:** Access to this data structure must be protected by a `std::mutex`.

## III. Data Flow

The data flows through the system in the following sequence:

1.  **Data Ingestion:** The `DataIngestor` (in `oanda_connector.cpp`) fetches live market data (pips) from the OANDA API in a dedicated thread.
2.  **Queueing:** The `DataIngestor` pushes the new `CandleData` into a thread-safe queue.
3.  **Dequeuing:** The main processing loop in `workbench_core.cpp` pops the `CandleData` from the queue.
4.  **Rolling Window Update:** The `RollingWindowManager` adds the new `CandleData` to its `std::deque` and removes the oldest data point if the window is full.
5.  **Signal Generation:** The `SEPSignalGenerator` is invoked with the current `RollingWindow`. It calculates the Coherence, Entropy, and Stability, and creates a new `SEPSignal`.
6.  **Signal Storage:** The new `SEPSignal` is pushed into the `SignalHistoryStore`'s `std::vector` under a mutex lock.
7.  **GUI Rendering:** The `GUIRenderer` (in `signals_tab_controller.cpp`) locks the mutex on the `SignalHistoryStore`, copies the latest signals, and then unlocks the mutex. It then uses the copied data to update the charts.
