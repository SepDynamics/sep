# TODO.md: Predictive Financial Modeling with SEP Engine

**Last Updated:** July 18, 2025
**Goal:** Develop and validate a predictive financial gauge using the SEP Engine on historical forex data. This involves optimizing the engine's performance with CUDA, defining a robust predictive metric, backtesting trading strategies, and documenting the results for a business proposal.

**Key Principles:**
- **Performance:** The engine must be optimized to handle large-scale time-series data efficiently. This requires profiling and parallelizing the core algorithms on the GPU.
- **Predictive Power:** The "predictive gauge" must be a leading indicator of market movements, combining multiple of the engine's metrics (coherence, stability, entropy).
- **Validation:** The strategies must be rigorously backtested against historical data to measure performance metrics like Alpha, Sharpe Ratio, and Total Return.
- **Automation:** The entire workflow, from data processing to analysis and reporting, must be automated for repeatability and scalability.

---

## Phase 1: System Optimization & Foundation (Current)

1.  **Profile CUDA Performance (In Progress)**
    *   **Task:** Analyze the current SEP Engine implementation to identify performance bottlenecks, specifically in the pattern comparison and evolution algorithms.
    *   **Tools:** Use `nvprof` or `Nsight Systems` to profile the `pattern_metric_example` executable when processing the large forex dataset.
    *   **Goal:** Pinpoint the exact functions and loops that are candidates for CUDA acceleration.

2.  **Define Predictive Gauge (Next)**
    *   **Task:** Create a markdown document (`docs/predictive_gauge_definition.md`) that formally defines the predictive gauge.
    *   **Details:** Specify the formula combining coherence, stability, and entropy. Define the smoothing techniques (e.g., 20-day rolling average, Z-score normalization) to be applied.

---

## Phase 2: Financial Analysis & Backtesting

1.  **Implement Backtesting Script (`financial_backtest.py`)**
    *   **Task:** Develop a Python script to perform three distinct backtesting analyses using the generated metrics from the SEP Engine.
    *   **Analysis A (Leading Breakout):** Implement a strategy that buys/sells based on breakouts in the predictive gauge.
    *   **Analysis B (Iterative Learning):** Implement a walk-forward analysis that trains the engine on a rolling window of past data to predict the next period.
    *   **Analysis C (Multi-Resolution Mapping):** Correlate the daily gauge signals with the high-resolution minute-by-minute data to test intra-day effectiveness.

2.  **Create Experiment Runner (`run_experiment.sh`)**
    *   **Task:** Write a shell script to automate the end-to-end experiment.
    *   **Workflow:**
        1.  Run the `pattern_metric_example` on the raw forex data (`/workspace/train_data_2021_to_2025`).
        2.  Output the metrics to a `metrics.json` file.
        3.  Execute the `financial_backtest.py` script to consume the metrics and generate an analysis report.

---

## Phase 3: Optimization & Documentation

1.  **Optimize CUDA Kernels**
    *   **Task:** Based on the profiling from Phase 1, refactor the core processing loops into CUDA kernels.
    *   **Goal:** Achieve significant performance speedup (target: >10x) for large-scale data analysis by parallelizing pattern comparisons.

2.  **Benchmark & Document Performance**
    *   **Task:** Run the automated experiment before and after the CUDA optimizations.
    *   **Goal:** Quantify the performance improvements and document them in the final report.

3.  **Create Final Proposal Report (`docs/proofs/poc_6_predictive_backtest.md`)**
    *   **Task:** Write a new, detailed proof-of-concept document summarizing the entire project.
    *   **Content:** Include the definition of the predictive gauge, the backtesting results (with graphs and tables), and the performance benchmarks. This document should be suitable for inclusion in a business proposal.

---

## Open Issues/Blockers
- **CUDA Expertise:** Requires careful implementation of CUDA kernels to ensure correctness and avoid race conditions.
- **Financial Modeling:** The backtesting strategies are simplified models; their results are indicative but not a guarantee of future performance.
- **Data Integrity:** Assumes the provided forex data is clean and correctly formatted.