# SEP Engine for Predictive Financial Modeling

This repository contains the **SEP Engine**, a high-performance C++ framework for quantum-inspired pattern analysis, specifically tailored for developing and validating predictive financial models. The goal of this project is to create a "predictive gauge" for financial markets by analyzing historical forex data.

## Project Goal

The primary objective is to develop and validate a predictive financial gauge using the SEP Engine. This involves:

*   **Performance Optimization**: Optimizing the engine's performance with CUDA to handle large-scale time-series data.
*   **Predictive Gauge**: Defining a robust predictive metric that combines coherence, stability, and entropy to act as a leading indicator for market movements.
*   **Backtesting**: Rigorously backtesting trading strategies based on the predictive gauge against historical data.
*   **Automation**: Automating the entire workflow from data processing to analysis and reporting.

For a detailed breakdown of the project's phases and tasks, please see [`docs/TODO.md`](docs/TODO.md).

## Getting Started

### Prerequisites

*   CMake 3.20+
*   A C++17 compliant compiler (GCC 10+, Clang 12+)
*   NVIDIA CUDA Toolkit (for GPU acceleration)
*   Python 3.x with pandas and other common data science libraries

### Build and Run

1.  **Configure and Build the Engine**:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

2.  **Run the Experiment**:
    The `run_experiment.sh` script automates the process of running the pattern metric engine on the training data and executing the backtesting script.
    ```bash
    ./run_experiment.sh
    ```

## Project Structure

*   `src/`: Core C++ source code for the SEP Engine.
    *   `quantum/`: Quantum-inspired algorithms for pattern analysis.
    *   `compat/`: CUDA compatibility layer for GPU acceleration.
    *   `core/`: Core data structures and utilities.
*   `docs/`: Project documentation, including the project roadmap and predictive gauge definition.
*   `examples/`: Example executables, including `pattern_metric_example`.
*   `scripts/`: Helper scripts for tasks like backtesting and data conversion.

## Key Components

*   **SEP Engine**: The core C++ engine for pattern analysis.
*   **Advanced Trading HUD**: Professional real-time trading interface featuring:
    - Live candlestick charts with technical indicators (EMAs, RSI, MACD, Bollinger Bands)
    - Automatic trend line detection for support/resistance levels
    - Interactive mouse hover showing OHLC data + SEP metrics at cursor position
    - Multi-timeframe analysis (24H/6H/3H) with dynamic data loading
    - Account balance monitoring with real-time P&L tracking
    - SEP signal overlay with proprietary pattern analysis visualization
    - Fixed window positioning for optimal trading workflow
*   **Predictive Gauge**: A composite metric for financial market prediction.
*   **Backtesting Script**: A Python script (`financial_backtest.py`) for validating trading strategies.
*   **Experiment Runner**: A shell script (`run_experiment.sh`) to automate the end-to-end workflow.
