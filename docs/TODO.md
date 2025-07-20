# TODO: Predictive Financial Modeling - Backtest Correction

**Last Updated:** 2025-07-20
**Goal:** Re-run the predictive backtest from `poc_6_predictive_backtest.md` using the correct, full dataset to properly validate the SEP Engine's alpha generation capabilities and benchmark CUDA performance.

---

## Phase 1: System Optimization & Foundation (Complete)

### ✓ Build System Stabilized
- **Achievement:** The build is stable and reproducible within a hermetic Docker environment.

### ✓ Sample Data Prepared
- **Achievement:** The full 280MB `OANDA_full_history.json` dataset is available.

---

## Phase 2: Corrected Financial Analysis & Backtesting (Current)

### In Progress

1.  **Refine `pattern_metric_example` for Per-Pattern JSON Output**
    *   **Task:** Modify `pattern_metric_example.cpp` to output a JSON array where each object contains the metrics (`coherence`, `stability`, `entropy`) for an individual pattern. This is critical for the downstream Python backtesting script.
    *   **Blocker:** The current implementation only outputs aggregate metrics.

2.  **Update `build_and_test.sh` to Automate the Full Pipeline**
    *   **Task:** Modify the script to execute the entire workflow in the correct sequence:
        1.  Build the C++ project (`ninja`).
        2.  Run unit tests (`ctest`).
        3.  Run the `pattern_metric_example` executable on the full `assets/test_data/OANDA_full_history.json` data, saving the per-pattern JSON metrics to `output/metrics_output.json`.
        4.  Run the Python backtesting script (`scripts/run_alpha_experiment.py`) using the generated `metrics_output.json` as input.
        5.  Run and log CUDA performance benchmarks.

### Pending

1.  **Execute and Profile the Full Pipeline**
    *   **Task:** Run the updated `build_and_test.sh` script.
    *   **Verification:**
        *   Confirm `output/metrics_output.json` contains per-pattern data.
        *   Confirm `output/alpha_results.json` is generated with valid results.
        *   Analyze benchmark logs to compare CPU vs. GPU performance and validate CUDA high-throughput processing.

2.  **Document Final Results (PoC #6 Update)**
    *   **Task:** Create a new or update the existing `docs/proofs/poc_6_predictive_backtest.md` with the results from the corrected backtest.
    *   **Content:** Include the new alpha, Sharpe ratio, performance metrics, and CUDA benchmark results.