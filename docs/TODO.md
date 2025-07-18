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

### Completed Tasks ✓

1. **Fixed run_experiment.sh Script**
   - Updated paths to use relative references
   - Made script executable
   - Added proper error handling and logging

2. **Created Sample Forex Data**
   - Generated `assets/test_data/sample_forex_data.csv` with realistic EUR/USD tick data
   - Includes proper timestamps, bid/ask prices, and volume
   - Ready for testing the backtesting pipeline

### In Progress

1. **Build System CUDA Issues (Critical)**
   - **Problem:** CMake build fails with CUDA path configuration errors
   - **Root Causes Identified:**
     - `FindCUDA.cmake` searches for nvcc in wrong location (should be `/usr/local/cuda/bin/nvcc`)
     - Toolchain files set incorrect include paths (`${CUDA_PATH}/src` instead of `${CUDA_PATH}/include`)
     - Results in invalid path construction: `/usr/local/cuda-12.9/bin/include`
   - **Required Fixes:**
     - Fix `FindCUDA.cmake` line 18 to search in bin subdirectory
     - Update `cuda-toolchain.cmake` line 17 to use `/include`
     - Update `cuda-clang-toolchain.cmake` line 13 to use `/include`
     - Update `cuda-gcc-toolchain.cmake` to use correct paths
   - **Status:** Ready for implementation

2. **Profile CUDA Performance**
   - **Task:** Analyze the current SEP Engine implementation to identify performance bottlenecks
   - **Blocked by:** CUDA build issues must be resolved first
   - **Tools:** Will use `nvprof` or `Nsight Systems` once build succeeds

3. **Define Predictive Gauge**
   - **Task:** Create a markdown document (`docs/predictive_gauge_definition.md`) that formally defines the predictive gauge
   - **Details:** Specify the formula combining coherence, stability, and entropy with smoothing techniques

---

## Phase 2: Financial Analysis & Backtesting

### Completed Components ✓

1. **Backtesting Script (`financial_backtest.py`)**
   - Implemented with three analysis strategies:
     - Analysis A: Leading Breakout Strategy
     - Analysis B: Iterative Learning (Walk-Forward)
     - Analysis C: Multi-Resolution Mapping
   - Includes performance metrics calculation
   - Ready for testing once metrics data is available

2. **Experiment Runner (`run_experiment.sh`)**
   - Created and tested basic functionality
   - Automates the end-to-end workflow
   - Includes proper error handling and logging

### Pending

1. **Complete Pipeline Testing**
   - Run full experiment with sample forex data
   - Verify metrics generation and backtesting integration
   - Document results

2. **Data Format Conversion**
   - Currently using `convert_metrics.py` as a workaround
   - Need to update `pattern_metric_example` to output JSON directly

---

## Phase 3: Optimization & Documentation

1. **Optimize CUDA Kernels**
   - **Blocked by:** Build system issues
   - **Goal:** Achieve >10x performance speedup for large-scale data analysis

2. **Benchmark & Document Performance**
   - Run experiments before and after CUDA optimizations
   - Quantify performance improvements

3. **Create Final Proposal Report (`docs/proofs/poc_6_predictive_backtest.md`)**
   - Include predictive gauge definition
   - Document backtesting results with visualizations
   - Add performance benchmarks

---

## Technical Debt & Issues

### Critical Issues
1. **CUDA Build System** - Multiple CMake configuration errors preventing compilation
2. **Data Format Mismatch** - `pattern_metric_example` outputs unstructured text instead of JSON

### Minor Issues
1. **Visualization** - `financial_backtest.py` needs matplotlib integration for charts
2. **Error Handling** - Need more robust error handling in the pipeline scripts

---

## Next Steps (Priority Order)

1. **Fix CUDA Build Issues** - Update all CMake files with correct paths
2. **Build SEP Engine** - Compile with CUDA support enabled
3. **Test Pipeline** - Run complete experiment with sample data
4. **Profile Performance** - Use CUDA profiling tools
5. **Enhance Visualization** - Add charts to backtesting results
6. **Document Results** - Create comprehensive experiment documentation

---

## Open Issues/Blockers
- **CUDA Build System:** CMake configuration needs immediate fixes (solutions identified)
- **Data Pipeline:** Need to standardize on JSON format throughout
- **Performance Baseline:** Cannot profile until build succeeds
- **Financial Modeling:** Current strategies are simplified; need real-world validation