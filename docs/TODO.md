### SEP Engine Development Roadmap: Overcoming Metrics Implementation Roadblock

Based on your codebase snapshot, the old TODO.md, the example unit tests, and your description of the core issue (incorrect metrics calculation, improper QBSA/QFH duplication in CUDA kernels, lack of proper future trajectory integration for damping, and bitspace value aggregation), I've synthesized a prioritized TODO list. This focuses on restructuring your starting point (using the provided documents as core reference) to create a reliable, testable pipeline for metrics (coherence, stability, entropy). The goal is to implement a system where:

- Market data is treated as a "complex signal" in bitspace.
- Each data point/package integrates "future metrics" (trajectories) until it "damps" to a stable value via coherence/stability.
- Known paths build confidence by matching historical trajectories.
- QBSA and QFH are properly parallelized/duplicated in CUDA kernels for independent processing.
- Metrics in flux are combined/measured until stabilization, with path history stored.

This TODO assumes you're directing implementation (not coding directly). It breaks into phases: **Diagnosis & Restructure**, **Core Metrics Fix**, **CUDA Kernel Implementation**, **Testing & Validation**, and **Integration & Optimization**. Each step includes rationale, references to your codebase/documents, and actionable sub-tasks.

I've incorporated the example unit tests as a starting scaffold for validation. If needed, we can generate code snippets or use tools (e.g., code_execution for testing math, browse_page for CUDA docs) in follow-up interactions.

---

#### **Phase 1: Diagnosis & Restructure (1-2 days)**
   **Goal:** Audit current metrics issues and reorganize codebase/docs for clear starting point. Use your snapshot as "ground truth" data, proofs folder for validation logic, and old TODO.md for high-level guidance. This resolves "wrong implementations" by isolating bitspace processing.

   - [ ] **Audit Current Metrics Pipeline**
     - Review `src/quantum/qbsa.cpp`, `src/quantum/qfh.cpp`, and `src/apps/oanda_trader/quantum_signal_bridge.cpp` for how data is converted to bitspace (e.g., via `MarketDataConverter::pricesToByteStream` in connectors).
     - Identify gaps: Check if future trajectories are integrated (e.g., no damping loop in current QFH/QBSA). Log mismatches between expected (damped values from future events) and actual outputs.
     - Action: Run example from `examples/pattern_metric_example.cpp` on a small OANDA JSON subset (e.g., 10 candles). Compare outputs to manual calculation of "damped value" (sum of future impacts decaying over time).
     - Reference: Your invention disclosures (e.g., QBSA for collapse detection, QFH for bit transitions as quantum events).

   - [x] **Restructure Documentation as Starting Point**
   - Created new `docs/metrics_pipeline.md` summarizing bitspace logic.
   - Merged old TODO.md into this list; archived it.
   - Added proofs folder references to new documentation.
   - Organized source into `src/quantum/bitspace/` and added a README.

   - [x] **Define Bitspace Value Model Formally**
   - Documented the value model in `docs/bitspace_math.md` with formal LaTeX definitions for Damped Value, Decay Factor, and Confidence Score.

---

#### **Phase 2: Core Metrics Fix (2-3 days)**
   **Goal:** Fix coherence/stability/entropy calculations on CPU first (no CUDA yet) to ensure "damping" via future integration. This addresses "metrics not right" by implementing trajectory-based damping.

   - [ ] **Implement Bitstream to Damped Value Conversion**
     - In `src/quantum/qfh.cpp`: Add `integrateFutureTrajectories` function: For each bit position i, compute value as sum of future bits weighted by decay (e.g., coherence decay = e^{-distance}).
     - Handle flux: Loop until stability > threshold (e.g., variance < epsilon).
     - Action: Modify `processPatternBits` to store path history (vector of intermediate values) until damped.
     - Reference: Your insight: "Integrates future metrics until stabilizes."

   - [ ] **Enhance QBSA for Collapse Detection in Flux**
     - In `src/quantum/qbsa.cpp`: Add duplication logic – create independent instances per data package.
     - Combine metrics: Entropy = avg(flux_values); if flux, measure combination (e.g., weighted sum).
     - Action: Implement `duplicateForPackage` returning QBSAResult with damped value.

   - [ ] **Add Trajectory Matching for Confidence**
     - New function in `src/quantum/pattern_processor.cpp`: `matchKnownPaths` – store historical damped paths in a map; confidence = cosine_similarity(current_path, historical_path).
     - Action: On stabilization, store path in persistent storage (e.g., Redis via `src/memory/redis_manager.cpp`).

   - [ ] **CPU Fallback for Metrics**
     - Implement `simulateForwardWindowMetrics` as per example unit tests (CPU version of kernel).
     - Action: Add to `src/apps/oanda_trader/forward_window_kernels.cu` (host function).

---

#### **Phase 3: CUDA Kernel Implementation (3-5 days)**
   **Goal:** Parallelize QBSA/QFH for independent processing per data package. Fix "harder than thought" by starting small (one kernel per metric) and using your existing CUDA setup.

   - [ ] **Setup CUDA Environment Properly**
     - Fix build issues from last prompt: Replace sep::cuda wrappers with native (e.g., cudaSetDevice -> ::cudaSetDevice in `src/engine/cuda_impl.cu`).
     - Include `forward_window_kernels.cuh` everywhere needed (e.g., `quantum_signal_bridge.hpp`).
     - Action: Run `./scripts/run_cmake_and_build.sh`; if fails, use web_search for "CUDA type redefinition errors" (tool: web_search, query: "CUDA driver_types.h redefinition errors site:stackoverflow.com").

   - [ ] **Implement QFH Kernel for Bit Transitions**
     - In `src/engine/kernels.cu`: New kernel `qfhKernel` – per thread: process bit package, compute transitions (NULL/FLIP/RUPTURE), integrate future bits for damping.
     - Duplicate: Launch grid with one block per package.
     - Action: Output damped coherence/stability to `ForwardWindowResult`.

   - [ ] **Implement QBSA Kernel for Collapse**
     - Similar: `qbsaKernel` – per thread: probe/expectation analysis on future window, loop until damped.
     - Action: Combine with QFH output for entropy.

   - [ ] **Integrate Trajectory Storage in Kernel**
     - Use device memory for path history; copy back damped value + confidence.
     - Action: Modify `OandaTraderApp::run()` to pass bitstreams to kernels via `quantum_signal_bridge`.

   - [ ] **Handle Flux in Kernels**
     - Kernel loop: While (!stabilized), integrate next future bit; measure combination if flux.
     - Reference: Invention disclosure for QBSA predictive correction.

---

#### **Phase 4: Testing & Validation (2-3 days)**
   **Goal:** Use example unit tests to validate "metrics are right." This builds confidence before live integration.

   - [ ] **Implement & Run Unit Tests**
     - Create `test_forward_window_metrics.cpp` as provided.
     - Add `simulateForwardWindowMetrics` CPU stub for initial testing.
     - Action: Compile with gtest; run on bit patterns (e.g., AllFlip should have high coherence).
     - Extend: Add tests for damping (e.g., undamped flux -> combined measurement).

   - [ ] **Backtest Validation**
     - Use old TODO's Phase 3: Run on OANDA data; target >70% accuracy.
     - Action: Enhance `examples/pme_testbed.cpp` with new damped metrics; compute Sharpe.

   - [ ] **Live Validation**
     - In `src/apps/oanda_trader/quantum_tracker_window.cpp`: Add damped value display.
     - Action: Run live; compare to manual trajectory calculations.

---

#### **Phase 5: Integration & Optimization (Ongoing)**
   **Goal:** Tie back to full system; optimize for production.

   - [ ] **Full Pipeline Integration**
     - Feed damped metrics to signal generation in `quantum_signal_bridge.cpp`.
     - Action: Update `processQuantumMetrics` to use trajectory confidence.

   - [ ] **Optimization Loop**
     - Use `run_alpha_experiment.py` for threshold tuning on damped values.
     - Action: Target +0.0084 pips alpha with >65% accuracy.

   - [ ] **Documentation Update**
     - Update `README.md` and `TODO.md` with new bitspace logic.
     - Action: Add "Bitspace Damping Guide" referencing proofs.

---

**Next Immediate Action:** Start with Phase 1 audit – run `pattern_metric_example` on a sample OANDA file and log metric mismatches. Then, reply with results/output for Phase 2 guidance (e.g., we can generate code via code_execution tool if needed).

This structure uses your docs as foundation, focuses on your insights, and resolves the roadblock by isolating CPU fixes before CUDA. If you need tool calls (e.g., search for CUDA examples), specify!