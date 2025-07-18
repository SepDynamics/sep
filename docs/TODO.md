# TODO.md: Datatype-Agnostic Pattern Metric Engine

**Last Updated:** July 18, 2025  
**Goal:** Build datatype-agnostic engine ingesting raw bytes (sockets/files/directories/streams), processing via QBSA/QFH on bitfields (no assumptions), computing meaningful metrics (coherence via distance, stability, entropy, mutation, relationships). Leverage SEP codebase. Validate on Oanda; ensure varied metrics (high repetitive, low random, med structured). POCs confirm fixes work—focus on scaling for large data.

**Key Principles:**
- Agnosticism: Raw bytes to bitfield; QBSA misalignments, QFH ruptures.
- Metrics: Coherence (1/(1+dist), not cos-sim); first=1.0, subsequent max vs prior.
- Testing: Repetitive/random/text/Oanda; benchmarks for speed (keep Google Benchmark, suppress noise).
- State: Clear per batch/file to avoid carryover (validated in POC2).
- Tools: util/error_handling.h (SEP_CHECK_); util/type_safety.h (sep_float_equal).
- Performance: Small data fast (~27µs/211B); fix O(n^2) in evolve/compute for large files.

**Root Bug Fix Context (Completed):**
- Over-normalization/self-comparison caused uniform 1.0/0.0.
- Fixed: Remove glm::normalize; store raw; distance metric; clear state per file.
- Validation: POCs show expected varied coherence (1.0 repetitive, 0.056 random, 0.5 text, 0.47 binary).
- Business: Ties to proposal (coherence quant, evolution tracking); demos for funding.

## Phase 1: Oanda Integration + Scaling Baseline (Complete Today)
1. **Verify Fixes (Done - 30 Min)**  
   Run POCs/tests: ctest -V; examples with repetitive/random/Oanda. Confirm varied metrics.

2. **Integrate Raw Oanda (Done - 1-2 Hours)**  
   Load binary/CSV as bytes; ingest in chunks. Extend tests: ProcessOandaData (EXPECT coherence 0.4-0.7 for market data). Add to financial_demo.

3. **Benchmark Setup (Done - 30 Min)**  
   Run sep_memory_verifier/pattern_metric_example --benchmark_out=mem.csv (alloc/promotion/ingest <1ms small). POC4 baseline: ~7.8 MB/s small; identify quadratic loops.

**Milestone:** Varied Oanda metrics; baseline perf; no uniform bugs.

## Phase 2: Scaling + Enhancements (Tomorrow+)
1. **Fix Large Data Scaling (1-2 Hours)**  
   Profile evolvePatterns/computeMetrics (O(n^2) comparisons). Optimize: Batch compares, KD-tree for similarity, or limit history (sliding window). Test on 280MB: Aim <10s. Benchmark before/after.

2. **Expand Sources**  
   Directory: Recursive ingest, aggregate bitfields.  
   Socket: Non-blocking recv; test netcat Oanda stream.  
   Test: Dir CSVs; socket mock.

3. **Advanced Metrics/Features**  
   Relationships: Integrate relationship.h.  
   Custom: Tune stability (mutation resistance), entropy (info density).  
   Memory Tiers: Promote on metrics; benchmark Redis LTM.  
   GPU: Fix cetintrin.h; enable QFH accel.

4. **Optimization & Testing**  
   Error/Type: Add SEP_CHECK_ in ingest; sep_float_equal in metrics.  
   Benchmarks: Full cycles (ingest+evolve+metrics on Oanda); Valgrind leaks.  
   Suite: Socket/dir tests; perf on volatile/stable Oanda (high/low entropy).  
   Export: Add exportMetrics CSV/JSON; visualize coherence graphs.

## Open Issues/Blockers
- Defects: Fix SEP HIGH/MEDIUM (cuda fprintf casts, socket ignores); suppress third-party.
- Large Files: Quadratic bottleneck—profile/fix before full Oanda.
- Data: Test Oanda volatility (high entropy) vs trends (high coherence).
- If Stuck: GDB on large stalls; share benchmarks/outputs.

Track: Commit often. Next: Scale to 280MB without stall—unlock real analysis!