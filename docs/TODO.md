# TODO.md: Predictive Financial Modeling with SEP Engine

**Last Updated:** July 19, 2025
**Goal:** Develop and validate a predictive financial gauge using the SEP Engine on historical forex data. This involves optimizing the engine's performance with CUDA, defining a robust predictive metric, backtesting trading strategies, and documenting the results for a business proposal. The immediate focus is to achieve a demonstrable alpha prediction capability.

**Key Principles:**
- **Performance:** The engine must be optimized to handle large-scale time-series data efficiently. This requires profiling and parallelizing the core algorithms on the GPU.
- **Predictive Power:** The "predictive gauge" must be a leading indicator of market movements, combining multiple of the engine's metrics (coherence, stability, entropy).
- **Validation:** The strategies must be rigorously backtested against historical data to measure performance metrics like Alpha, Sharpe Ratio, and Total Return.
- **Automation:** The entire workflow, from data processing to analysis and reporting, must be automated for repeatability and scalability.

---

## Phase 1: System Optimization & Foundation (Complete)

### ✓ Build System Stabilized
- **Achievement:** The build is now fully stable and reproducible. The complex, multi-faceted build failures have been resolved through a combination of code-level refactoring and the adoption of a hermetic Docker-based build environment.
- **Key Fixes Implemented:**
    - **Corrected Header Dependencies:** Resolved compilation errors by adding missing `#include` statements for shared types across modules (e.g., `engine/types.h`).
    - **Fixed CUDA API Usage:** Removed direct `cuda_runtime.h` includes from standard C++ source files, enforcing proper abstraction.
    - **Resolved GLM Conflicts:** Addressed CUDA compiler errors related to GLM header conflicts.
    - **Containerized Build Environment:** The entire build process now runs within a Docker container, eliminating system-level toolchain and library inconsistencies.
- **Reference:** See `docs/cuda_build_resolution.md` for a full report on the final solution.

### ✓ Sample Data Prepared
- **Achievement:** Initial `OANDA.json` data has been acquired, and the `prepare_experiment_data.py` script is ready to generate training and testing sets for walk-forward analysis.

---

## Phase 2: Financial Analysis & Backtesting (Current)

### In Progress

1.  **Refine `pattern_metric_example` JSON Output**
    *   **Task:** Modify `pattern_metric_example.cpp` to output metrics in JSON format, eliminating the need for `parse_metrics_from_stream` in `run_alpha_experiment.py`.
    *   **Status:** This is the next immediate priority.

2.  **Test Pipeline**
    *   **Task:** Run the complete experiment with `run_alpha_experiment.py` using OANDA train/test data to demonstrate alpha prediction and evaluate iterative training effectiveness.
    *   **Blocked by:** `pattern_metric_example` JSON output.

3.  **Profile Performance**
    *   **Task:** Use CUDA profiling tools (`nvprof` or `Nsight Systems`) to identify and address performance bottlenecks.
    *   **Blocked by:** Completion of the pipeline test.

### Pending

1.  **Enhance Visualization**
    *   **Task:** Add charts to backtesting results in `financial_backtest.py`.

2.  **Document Results**
    *   **Task:** Create comprehensive experiment documentation in `docs/proofs/poc_6_predictive_backtest.md`.

---

## Next Steps (Priority Order)

1.  **Refine `pattern_metric_example` JSON Output**: Modify `pattern_metric_example.cpp` to output metrics in JSON format, eliminating the need for `parse_metrics_from_stream` in `run_alpha_experiment.py`.
2.  **Test Pipeline** - Run complete experiment with `run_alpha_experiment.py` using OANDA train/test data to demonstrate alpha prediction and evaluate iterative training effectiveness.
3.  **Profile Performance** - Use CUDA profiling tools (`nvprof` or `Nsight Systems`).
4.  **Enhance Visualization** - Add charts to backtesting results in `financial_backtest.py`.
5.  **Document Results** - Create comprehensive experiment documentation in `docs/proofs/poc_6_predictive_backtest.md`.