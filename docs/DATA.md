# SEP Engine Data Flow Architecture

## Overview

This document outlines the SEP Engine's data processing pipeline. As of July 2024, the entire pipeline is **non-functional due to critical compilation failures**. The primary architectural flaw identified is a dependency conflict where core engine components (`DataParser`, `OandaConnector`) are forced to include GUI headers (`imgui.h`), making a successful build impossible.

**The immediate and only priority is to resolve these build-blocking issues by refactoring header dependencies.**

## Data Flow Diagram (Intended Architecture)

```mermaid
graph TB
    subgraph "Market Data Sources"
        OANDA[OANDA API<br/>Live Market Data]
        HIST[Historical Data<br/>48H M1 Candles]
        STREAM[Real-time Stream<br/>Price Updates]
    end

    subgraph "Data Ingestion Layer"
        CONN[OandaConnector<br/>Rate Limited: 50ms]
        PARSER[DataParser<br/>JSON → Patterns]
        BUFFER[StreamBuffer<br/>Chunked Processing]
    end

    subgraph "Quantum Processing"
        PME[PatternMetricEngine<br/>CUDA-Accelerated]
        CUDA[CUDA Kernels<br/>Pattern Analysis]
        QFH[QFH Processor<br/>Bitstream Analysis]
        DAG[DAG Graph<br/>Pattern Correlations]
    end

    subgraph "Real-time Processing"
        EVOLVE[Pattern Evolution<br/>Mutation Algorithms]
        METRICS[Metrics Computation<br/>Coherence/Stability/Entropy]
        THRESH[Threshold Detection<br/>Signal Generation]
    end

    subgraph "Visualization Layer"
        DASHBOARD[Workbench Dashboard<br/>ImGui Interface]
        SIGTAB[Signals Tab<br/>Candlestick & Metric Charts]
        ENGTAB[Engine Tab<br/>Diagnostic Plots]
        BACKTAB[Backend Tab<br/>Trading & Backtest UI]
    end

    subgraph "Trading Decision Layer"
        SIGNALS[Alpha Signals<br/>Buy/Sell Indicators]
        RISK[Risk Management<br/>Position Sizing]
        ORDERS[Order Placement<br/>OANDA Trading API]
        BACKTEST[Backtesting<br/>Performance Metrics]
    end

    %% Data Flow Connections
    OANDA --> CONN
    HIST --> CONN
    STREAM --> BUFFER
    CONN --> PARSER
    BUFFER --> PME
    PME --> CUDA
    PME --> QFH
    PME --> DAG
    PME --> EVOLVE
    EVOLVE --> METRICS
    METRICS --> THRESH
    METRICS --> SIGTAB
    HIST --> SIGTAB
    THRESH --> SIGTAB
    METRICS --> ENGTAB
    THRESH --> SIGNALS
    SIGNALS --> RISK
    SIGNALS --> BACKTEST
    RISK --> ORDERS
    ORDERS --> OANDA
```

## Critical Component Compilation Status

### 1. **Data Parser (`data_parser.cpp`)**
-   **Function**: Converts raw data into engine-compatible formats.
-   **Status**: **BUILD FAILED**.
-   **Error Analysis**:
    ```
    # From errors.txt:
    error: field has incomplete type 'sep::workbench::CorrelationMetrics'
    error: member access into incomplete type 'const sep::workbench::CorrelationMetrics'
    error: no viable overloaded '=' for std::chrono::time_point
    ```
-   **Conclusion**: A forward declaration of `CorrelationMetrics` is used, but the full definition is never included. The header containing the struct must be included. Additionally, string-based timestamps are being incorrectly assigned to `std::chrono::time_point` members without proper parsing.

### 2. **Multi-Timeframe Analyzer (`multi_timeframe_analyzer.cpp`)**
-   **Function**: Resamples and analyzes market data across different timeframes.
-   **Status**: **BUILD FAILED**.
-   **Error Analysis**:
    ```
    # From errors.txt:
    error: out-of-line definition of 'ingestMarketData' does not match any declaration...
    error: no viable conversion from 'vector<sep::workbench::CandleData>' to 'const vector<sep::common::CandleData>'
    error: member access into incomplete type 'const sep::workbench::CandleData'
    ```
-   **Conclusion**: A major refactoring has created a mismatch between the class declaration and its implementation. There is a critical namespace conflict between `sep::workbench::CandleData` and `sep::common::CandleData`, and the `CandleData` struct is being used as an incomplete type, indicating a missing header include.

### 3. **OANDA Connector (`oanda_connector.cpp`)**
-   **Function**: Fetches market data and handles trade execution with the OANDA API.
-   **Status**: **BUILD FAILED**.
-   **Error Analysis**: The build fails because it indirectly includes a GUI header:
    ```
    # From build_log.txt:
    fatal error: 'imgui.h' file not found
    # Included from: .../src/apps/workbench/core/ui_layout_manager.h
    ```
-   **Conclusion**: A backend data connector should **never** include a GUI library. This indicates a severe architectural flaw where headers are improperly chained. The dependency on `ui_layout_manager.h` must be removed.

### 4. **Backtester (`backtester.cpp`, `data_loader_test.cpp`)**
-   **Function**: Simulates trading strategies on historical data.
-   **Status**: **BUILD FAILED**.
-   **Error Analysis**:
    ```
    # From errors.txt:
    error: unknown type name 'CandleData'; did you mean 'common::CandleData'?
    error: no member named 'loadData' in '...DataLoader'; did you mean 'load_data'?
    ```
-   **Conclusion**: The backtester components have not been updated to reflect recent refactoring of data types (namespaces) and method names.

## Data Authenticity Policy (Post-Build-Fix)

The platform is designed for authentic data processing. Once the build is fixed, all components will use genuine data:
-   ✅ **OandaConnector**: Real REST API and streaming data.
-   ✅ **DataParser**: Genuine OHLC to pattern conversion.
-   ✅ **PatternMetricEngine**: CUDA-accelerated algorithms operating on real data.

**Overall Conclusion**: The data flow architecture is sound in theory but is completely blocked in practice. **Resolving the critical compilation errors by refactoring headers and fixing API usage is the mandatory first step to make this pipeline operational.**