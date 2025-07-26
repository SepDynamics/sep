
---

### DATA.md

```markdown
# SEP Engine Data Flow Architecture

## Overview

This document maps the authentic data processing pipeline of the SEP Engine Trading Platform, from live market data ingestion to trading signal generation and execution. The pipeline is fully implemented and verified as of July 2025, with focus on integrating data flow into the workbench dashboard and backtesting framework.

## Data Flow Diagram

```mermaid
graph TB
    subgraph "Market Data Sources"
        OANDA[OANDA API<br/>Live Market Data]
        HIST[Historical Data<br/>48H M1 Candles]
        STREAM[Real-time Stream<br/>Price Updates]
    end

    -->|subgraph "Data Ingestion Layer"|
        CONN[OandaConnector<br/>Rate Limited: 50ms]
        --> PARSER[DataParser<br/>JSON → Patterns]
        --> BUFFER[StreamBuffer<br/>Chunked Processing]
    end

    -->|subgraph "Quantum Processing"|
        PME[PatternMetricEngine<br/>CUDA-Accelerated]
        --> CUDA[CUDA Kernels<br/>Pattern Analysis]
        --> QFH[QFH Processor<br/>Bitstream Analysis]
        --> DAG[DAG Graph<br/>Pattern Correlations]
    end

    -->|subgraph "Real-time Processing"|
        EVOLVE[Pattern Evolution<br/>Mutation Algorithms]
        --> METRICS[Metrics Computation<br/>Coherence/Stability/Entropy]
        --> THRESH[Threshold Detection<br/>Signal Generation]
    end

    -->|subgraph "Visualization Layer"|
        DASHBOARD[Workbench Dashboard<br/>ImGui Interface]
        --> SIGTAB[Signals Tab<br/>Candlestick Charts & Signals]
        --> ENGTAB[Engine Tab<br/>Diagnostics]
        --> BACKTAB[Backend Tab<br/>Trading & Backtesting]
    end

    -->|subgraph "Trading Decision Layer"|
        SIGNALS[Alpha Signals<br/>Buy/Sell Indicators]
        --> RISK[Risk Management<br/>Position Sizing]
        --> ORDERS[Order Placement<br/>OANDA Trading API]
        --> BACKTEST[Backtesting<br/>Performance Metrics]
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