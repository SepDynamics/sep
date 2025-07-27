# SEP Engine Data Flow Architecture

## Overview

This document outlines the SEP Engine's data processing pipeline. As of the latest build, the pipeline is **operational and compiling successfully**. The architectural refactoring to decouple core components from the GUI is complete, and the focus has shifted to implementing and optimizing the data flow from market sources to the trading workbench.

The current priority is to fully implement the connections between each stage of the pipeline to enable real-time signal generation and visualization.

## Data Flow Diagram

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

### Backtesting Integration

`ServiceConnector` forwards every set of pattern signals produced by
`PatternMetricEngine` into the backtester. When the user selects a dataset in the
backend tab, `loadInitialData` parses the candles, generates SEP signals and then
invokes `Backtester::run` with those signals. The resulting `BacktestResult` is
sent through the global event bus so the UI plots refresh automatically.

## Component Status

### 1. **Data Parser (`data_parser.cpp`)**
-   **Function**: Converts raw market data (JSON, CSV, Binary) into engine-compatible `Pattern` objects.
-   **Status**: **Operational.** Compiles successfully and forms the entry point for data ingestion. The type and API mismatches from the refactoring have been resolved.

### 2. **OANDA Connector (`oanda_connector.cpp`)**
-   **Function**: Fetches live and historical market data and handles trade execution via the OANDA API.
-   **Status**: **Partially Operational.** The connector can fetch data, but there is a known issue with the API request for historical data.

### 3. **PatternMetricEngine (`pattern_metric_engine.cpp`)**
-   **Function**: The core analysis engine that applies quantum-inspired algorithms (QFH, QBSA) to detect patterns and compute metrics like Coherence, Stability, and Entropy.
-   **Status**: **Partially Operational.** The engine compiles and runs, but it is currently only generating "BUY" signals. This needs to be investigated.

## Known Issues

-   **OANDA API Error:** The application is currently sending a `count` parameter along with `to` and `from` parameters in the OANDA API request, which is causing an error. This needs to be fixed to enable reliable data fetching.
-   **Application Freezing:** The application currently freezes during the initial data pull from OANDA. This is likely because the data is being fetched synchronously on the main thread.
-   **"Buy-Only" Signals:** The `PatternMetricEngine` is currently only generating "BUY" signals. This is highly suspicious and needs to be investigated.

## Data Authenticity Policy

The platform is designed for and operates on authentic data to ensure the reliability of its analysis and signals.
-   ✅ **OandaConnector**: Uses real REST API and streaming data from OANDA's practice environment.
-   ✅ **DataParser**: Performs direct conversion of genuine OHLC data into patterns without simulation.
-   ✅ **PatternMetricEngine**: All CUDA-accelerated algorithms operate on real, ingested market data.

**Overall Conclusion**: The data pipeline's architecture is stable and the core components are operational. The next phase of development is to implement the connections between these components and visualize the real-time results in the workbench GUI.
