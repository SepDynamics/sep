# Bitspace Metrics Pipeline

This document outlines the core logic for processing financial market data as a complex signal within a bitspace representation. The primary goal is to calculate reliable, forward-looking metrics (coherence, stability, entropy) by integrating future trajectories until the signal "damps" to a stable value.

## Core Pipeline Logic

The process follows these sequential steps:

1.  **Data Ingestion**: A market data package (e.g., an OANDA candlestick) is received.
2.  **Bitstream Conversion**: The raw data is converted into a bitstream representation (e.g., using `MarketDataConverter::pricesToByteStream`).
3.  **Independent Kernel Processing**: The bitstream is sent to an independent CUDA kernel for parallelized QBSA (Quantum Bit State Analysis) and QFH (Quantum Field Harmonics) processing.
4.  **Future Trajectory Integration**: Within the kernel, the algorithm integrates future data points (a forward window) to calculate how the initial signal evolves.
5.  **Damping and Stabilization**: The metrics are repeatedly calculated, integrating more future data until they converge on a stable "damped" value. This process measures the signal's flux and resolves it to a final state.
6.  **Path History Storage**: The sequence of metric values during the damping process (the trajectory path) is stored for confidence scoring.
7.  **Confidence Scoring**: The resulting trajectory is compared against a database of known historical paths. A high similarity score increases confidence in the signal's predictive power.

## References

-   **Trajectory Validation**: The logic for validating trajectories against historical data is based on the concepts outlined in [`poc_6_predictive_backtest.md`](proofs/poc_6_predictive_backtest.md).
-   **Bitspace Math**: The formal mathematical models for damping and value calculation are defined in [`bitspace_math.md`](bitspace_math.md).
