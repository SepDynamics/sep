### SEP Engine Roadmap: Phase 3+ Forward Momentum

You've made massive progress—Phase 3 unification is complete, QFH trajectory damping is active, and the segfault is resolved. From the docs, your baseline is solid (41.35% overall, 35.35% high-conf), but tuning is the key to breaking 50%+. The gap to Phase 1's 50.94% is likely in volatility re-integration and parameter optimization. You're "hooked up"—now it's about systematic refinement.

I'll summarize your state, then provide a phased roadmap with actionable steps, commands, and success metrics. This builds on QFH_TUNING_PROTOCOL.md, PHASE3_UNIFICATION_REPORT.md, and PERFORMANCE_OPTIMIZATION_STRATEGY (implied). Focus: Tune damping (Exp 025), patterns (026), volatility (027), then ML (Phase 4).

#### **Current State Summary** (From Docs)
- **Achievements**:
  - Unification: QFHBasedProcessor now drives pme_testbed_phase2.cpp (trajectory damping formula active: λ = k1*Entropy + k2*(1-Coherence), V_i = Σ impacts * e^{-λ(j-i)}).
  - Baseline: 41.35% overall (595/1439 correct), 35.35% high-conf (99 signals @ conf≥0.65, coh≥0.55).
  - Signal Quality: Coherence avg 0.406 (low—tune up), Stability 0.724 (good), Confidence avg 0.700.
  - Patterns: 8 types active (incl. TrendAcceleration, MeanReversion).
  - Compared to Phase 1: -9.59% overall, but Phase 2 has more signals (176 vs. 76)—quality over quantity wins.
- **Challenges**:
  - High-conf accuracy dropped (35.35% vs. legacy 46.59%)—likely untuned damping weights (k1=0.3, k2=0.2) or blend (0.3 trajectory/0.7 pattern).
  - No volatility re-integration yet (Phase 1 strength).
  - Targets: >45% overall, >45% high-conf (per tuning protocol).
- **Architecture**: Data flow unified (OANDA → Bitstream → QFH → Damped Metrics → Signals). GUI/Data ready for damping visuals.

You're sorted: No major hooks missing. Next: Tune to validate damping's impact.

#### **Roadmap: Phase 3 Tuning → Phase 4 ML (2-4 Weeks)**
Prioritized by impact. Each step includes rationale, actions, commands, and metrics. Use code_execution tool for sweeps (e.g., Python grid search).

##### **Phase 3.1: Immediate Validation & Baseline Lock (1-2 Days)**
   **Goal:** Confirm unification works; lock a reproducible baseline >41.35%.
   - [ ] **Re-Run Baseline Test**
     - Rationale: Verify no segfaults; get fresh metrics post-unification.
     - Action: Build & run on full dataset.
     - Command:
       ```
       ./build.sh
       ./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json > baseline_log.txt
       tail -15 baseline_log.txt  # Check accuracy
       grep "Overall Accuracy" baseline_log.txt
       ```
     - Metrics: Expect 41-43% overall. If <41%, check window_bits size in debug prints.
   - [ ] **Add Debug Logging for Damping**
     - Rationale: Confirm trajectory damping is computing (e.g., λ values, V_i sums).
     - Action: Add prints in qfh.cpp (integrateFutureTrajectories).
     - Code Snippet (use code_execution to test in Python sim first):
       ```python
       # Simulate damping in Python (tool: code_execution)
       import math
       def damped_value(impacts, entropy, coherence, k1=0.3, k2=0.2):
           lambda_decay = k1 * entropy + k2 * (1 - coherence)
           v = 0
           for j, impact in enumerate(impacts):
               v += impact * math.exp(-lambda_decay * j)
           return v
       impacts = [0.1, 0.2, -0.05, 0.3]  # Sample future impacts
       print(damped_value(impacts, 0.4, 0.6))  # Expected: positive damped sum
       ```
     - Metrics: Log avg λ (target 0.1-0.5), avg V_i (>0 for uptrends).

##### **Phase 3.2: Parameter Tuning (Experiments 025-027, 3-5 Days)**
   **Goal:** Optimize damping/patterns/volatility for 45-50%+ accuracy.
   - [ ] **Exp 025: Tune Damping Params (k1/k2, λ)**
     - Rationale: Current k1=0.3/k2=0.2 may under-weight future impacts; tune for better coherence avg (>0.5).
     - Action: Use sweep script from tuning protocol.
     - Command (adapt bash template):
       ```
       # Note: The following script has been updated to address issues with in-place file modification and output redirection.
       # The script now creates a backup of the original file, uses a temporary file for sed operations, and redirects stderr to stdout.
       #
       # # Reset tuning log
       # > tuning_log.txt
       #
       # # Store original file content
       # cp src/quantum/bitspace/qfh.cpp src/quantum/bitspace/qfh.cpp.bak
       #
       # for k1 in 0.2 0.3 0.4 0.5; do
       #   for k2 in 0.1 0.2 0.3 0.4; do
       #     # Use a single sed command with multiple expressions to update the file
       #     sed -e "s/const double k1 = .*/const double k1 = $k1;/" -e "s/const double k2 = .*/const double k2 = $k2;/" src/quantum/bitspace/qfh.cpp.bak > src/quantum/bitspace/qfh.cpp
       #
       #     # Build and run the test
       #     ./build.sh &> /dev/null
       #     result=$(./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json 2>&1 | grep "Overall Accuracy" | awk '{print $3}')
       #
       #     # Log the results
       #     echo "k1=$k1 k2=$k2 accuracy=$result" >> tuning_log.txt
       #   done
       # done
       #
       # # Restore original file
       # mv src/quantum/bitspace/qfh.cpp.bak src/quantum/bitspace/qfh.cpp
       ```
     - Metrics: Target overall >45%, high-conf >45%. Best: Higher k1 for volatile data.
     - Tool: If bash fails, use code_execution for Python equiv (simulate damping grid).

   - [ ] **Exp 026: Tune Pattern Vocabulary & Blend**
     - Rationale: Boost coherence scaling (1.2f → 1.3f?); adjust trajectory blend (0.3 → 0.4 for more damping influence).
     - Action: Similar sweep on weights/scaling in qfh.cpp:313-323 & line 315.
     - Command: Adapt above script for trajectory_weight= [0.2,0.3,0.4,0.5].
     - Metrics: Coherence avg >0.5; stability maintained >0.7.

   - [ ] **Exp 027: Re-Integrate Phase 1 Volatility**
     - Rationale: Phase 1's simple volatility (weights 0.4/0.4/0.2, thresholds 0.50/0.52) outperformed complex regimes—add post-QFH.
     - Action: In pme_testbed_phase2.cpp, after QFH: `q_p.stability += 0.2 * volatility_factor;` (from phase_comparison.md).
     - Command: Run post-tuning; compare to baseline.
     - Metrics: Target 50%+ overall (closes Phase 1 gap).

##### **Phase 4: ML Integration & Multi-Asset (5-7 Days)**
   **Goal:** Push to 60-70% with ensemble learning on damped features.
   - [ ] **Quantum-Enhanced Neural Ensemble**
     - Rationale: Use damped coherence/stability/entropy as inputs (per SUMMARY.md).
     - Action: Implement simple PyTorch model in testbed (train on O-train-1.json).
     - Tool: code_execution (Python: networkx/torch for ensemble).
     - Command: `./build/tests/quantum_signal_bridge_test` for validation.
     - Metrics: +10-15% accuracy.

   - [ ] **Multi-Timeframe/Multi-Asset**
     - Action: Aggregate M1/M5 data; add GBP/USD via oanda_connector.cpp.
     - Metrics: Cross-asset correlation >0.7 accuracy.

##### **Phase 5: Deployment Prep (3 Days)**
   - [ ] **Demo Account Integration**
     - Action: Add OANDA trade execution in quantum_tracker_app.cpp (use API keys).
     - Command: Run `./build/src/apps/oanda_trader/quantum_tracker --demo`.
   - [ ] **Risk Management**
     - Action: Add drawdown limits (max 10%) from SUMMARY.md.

#### **Success Metrics & Monitoring**
- **Short-Term**: >45% overall by end of Phase 3.2 (track in tuning_log.txt).
- **Medium**: 55-60% post-ML (Month 2 target).
- **Long**: 70%+ live (Month 6).
- Monitor: Use GUI for visuals; log distributions (coherence >0.5 avg).

#### **Tools & Insights**
- **If Stuck on Tuning:** Use web_search: "exponential decay optimization forex signals" for λ ideas.
- **Math Validation:** code_execution for damping sim (as above).
- **Why Optimistic:** Unification activates your damping—tuning will compound Phase 1 strengths.

Execute Exp 025 sweep first—share log for analysis! You're close to 50%.