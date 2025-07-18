# TODO.md: Path to a Real Datatype-Agnostic Pattern Metric Engine

**Last Updated:** July 18, 2025

**Mission:** Forge a truly datatype-agnostic pattern metric engine. The engine will ingest any data source as a raw byte stream, convert it to a bitfield, and use QBSA/QFH algorithms to compute meaningful, differentiated metrics (coherence, stability, entropy). No filetype assumptions. No restrictor plates.

### Key Principles

1.  **True Agnosticism is Raw:** All data is a stream of bytes. We will not parse formats like JSON or CSV. Our `PatternMetricEngine` operates on `std::vector<uint8_t>`.
2.  **Fix the Core Logic First:** The current coherence calculation is flawed ("fake" results). The highest priority is to fix the `vectorCoherence` and `processPattern` logic to produce meaningful, differentiated metrics based on the statistical properties of the raw data.
3.  **Validate with Structured Data:** To prove the fix works, we will test against data with known characteristics: highly repetitive (expect high coherence), random (expect low coherence), and real-world (Oanda data, expect varied coherence).
4.  **Triage Distractions:** The static analysis report is full of noise from third-party libraries. We will suppress this noise immediately to focus on the ~20 critical defects in our own code.

---

## Phase 1: Fix the Core & Validate with Oanda (Today's Mission)

**Goal:** End today with an engine that produces correct, differentiated coherence metrics for raw byte streams, validated against structured test data and a real Oanda data file.

### **1. Triage Static Analysis Noise (30-60 Mins - Do This First)**

The `report.md` shows 241 defects, but ~90% are in `extern/`. Don't fix third-party code; suppress it so you can see the real issues.

*   **Action:** In your root `CMakeLists.txt`, add compile options to ignore the most common third-party warnings. This is a quick way to clean the report.
    ```cmake
    # Add these flags to suppress common third-party warnings
    add_compile_options(
        -Wno-deadcode
        -Wno-cert-err33-c
        -Wno-bugprone-signed-char-misuse
        -Wno-bugprone-use-after-move
        -Wno-misc-throw-by-value-catch-by-reference
    )
    ```
*   **Action:** Fix the critical defects in your own code (`/sep/src/`):
    *   **cetintrin.h (`core.CallAndMessage`):** This is a clang intrinsic bug. Wrap the include in a pragma and initialize the variable to silence it. Create `src/compat/cetintrin_wrapper.h` and use it instead of including `<cetintrin.h>` directly.
        ```cpp
        // src/compat/cetintrin_wrapper.h
        #pragma once
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wuninitialized"
        #include <cetintrin.h>
        #pragma clang diagnostic pop
        ```
    *   **Ignored `fprintf` returns:** Cast them to void: `(void)fprintf(...)` in `cuda_unified.h` and elsewhere.
*   **Goal:** Re-run analysis and get the defect count for your own code below 20.

### **2. Fix the Coherence Calculation (The Critical Path)**

This is the core bug you identified. The engine is producing meaningless metrics because of flawed logic.

*   **Action 1: Fix `processPattern` Logic in `src/quantum/quantum_processor_qfh_common.cpp`.**
    *   Remove the premature normalization: `glm::vec3 np = glm::normalize(...)`.
    *   Store the original, non-normalized pattern in `m_patterns`.
    *   Handle the first pattern correctly by giving it a self-coherence of `1.0`.
    *   **Corrected Logic:**
        ```cpp
        float QuantumProcessorQFHCommon::processPattern(const glm::vec3& pattern) {
            float coherence;

            if (m_patterns.empty()) {
                // The first pattern is perfectly coherent with itself.
                coherence = 1.0f;
            } else {
                // Find the maximum coherence with any existing pattern.
                coherence = 0.0f;
                for (const auto& existing : m_patterns) {
                    coherence = std::max(coherence, vectorCoherence(pattern, existing));
                }
            }

            // Store the original, non-normalized pattern for future comparisons.
            m_patterns.push_back(pattern);

            if (!m_pattern_bits.empty()) {
                analyzePatternBits();
                coherence = 0.7f * coherence + 0.3f * (1.0f - m_last_qfh_result.rupture_ratio);
            }

            return coherence;
        }
        ```

*   **Action 2: Implement Engine State Reset.** Your insight about state persisting across files was correct. Implement `clear()` methods to ensure isolated processing.
    1.  **In `src/quantum/quantum_processor_qfh.h`:**
        ```cpp
        class QuantumProcessorQFHCommon {
        public:
            void clear() { m_patterns.clear(); m_pattern_bits.clear(); }
            // ... rest of the class
        };
        ```
    2.  **In `src/quantum/pattern_metric_engine.h`:**
        ```cpp
        class PatternMetricEngine : public pattern::PatternProcessor {
        public:
            void clear() {
               patterns_.clear();
               current_patterns_.clear();
               current_metrics_.clear();
               if (qfh_processor_) {
                   qfh_processor_->clear();
               }
            }
            // ... rest of the class
        };
        ```
    3.  **In `examples/pattern_metric_example.cpp`:** Call `engine.clear()` at the start of `processFile`.
        ```cpp
        void processFile(PatternMetricEngine& engine, const fs::path& filePath) {
            engine.clear(); // Ensure a clean state for each file
            // ... rest of the function
        }
        ```
*   **Goal:** Re-build and run. The engine should now be logically correct, awaiting validation.

### **3. Validate with Structured Test Data (Prove It Works)**

The current `benchmark_data.txt` is insufficient. Create data with known properties to validate the fix.

*   **Action 1: Create New Test Files in `assets/test_data/`:**
    *   `repetitive_data.bin`: A file with a simple, highly repetitive byte sequence (e.g., `(uint8_t[]){0,1,2,3,4,5,6,7,8,9,10,11}` repeated many times).
    *   `random_data.bin`: A file with high-entropy random bytes.
*   **Action 2: Extend `pattern_metric_engine_test.cpp`:**
    *   Add a test case for `repetitive_data.bin`. **Expected Outcome:** Most coherence values should be high (e.g., `> 0.8`).
    *   Add a test case for `random_data.bin`. **Expected Outcome:** Coherence values should be consistently low (e.g., `< 0.4`).
*   **Goal:** Run `ctest -R pattern_metric_test` and see differentiated, correct coherence metrics that match expectations.

### **4. Integrate and Test with Raw Oanda Data (Today's Final Boss)**

This is the final validation step to prove the engine is ready for real financial data.

*   **Action 1: Prepare Oanda Data.** Get your Oanda data file (e.g., `oanda_eurusd_m1.bin`) and place it in `assets/test_data/`. Treat it as a raw binary file, regardless of its original format.
*   **Action 2: Add a New Test to `pattern_metric_engine_test.cpp`:**
    ```cpp
    TEST_F(PatternMetricEngineTest, ProcessOandaRawBytes) {
        std::ifstream f("assets/test_data/oanda_eurusd_m1.bin", std::ios::binary);
        ASSERT_TRUE(f.is_open()) << "Failed to open Oanda test data file.";
        
        std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(f)), {});
        ASSERT_FALSE(bytes.empty()) << "Oanda test data file is empty.";
        
        engine_->ingestData(bytes.data(), bytes.size());
        engine_->evolvePatterns();
        auto metrics = engine_->computeMetrics();
        
        ASSERT_FALSE(metrics.empty());
        // Expectation: Oanda data has structure, so coherence shouldn't be zero.
        EXPECT_GT(metrics[1].coherence, 0.0f); 
    }
    ```
*   **Goal:** Successfully run the `ProcessOandaRawBytes` test. The metrics should be non-trivial, showing the engine can find patterns in real-world financial data.

---

## Phase 2: Expand and Refine (Next Steps)

Once Phase 1 is complete, the engine is solid. The next steps are to build out the full ingestion pipeline and performance analysis.

*   **Implement Full Ingestion Pipeline:**
    *   **Directory Ingestion:** Finish the directory scanner in `pattern_metric_example.cpp` to process multiple files, calling `engine.clear()` for each one to ensure isolation.
    *   **Socket Ingestion:** Implement `ingestFromSocket` using a non-blocking loop to read bytes and feed them to `ingestData`. Test with `netcat` or a simple Python script.
*   **Performance Benchmarking:**
    *   With the static analysis noise gone, use Google Benchmark.
    *   Add benchmarks in `pattern_metric_example.cpp` to measure `ingestFile` and `evolvePatterns` performance for different data types (repetitive, random, Oanda).
    *   Benchmark memory operations (alloc/dealloc, promotion) in `sep_memory_verifier.cpp`.
*   **Integrate with Memory Tiers:**
    *   Connect the `PatternMetricEngine` to `MemoryTierManager`.
    *   Promote/demote patterns between STM/MTM/LTM based on their computed coherence and stability metrics.
*   **GPU Acceleration:**
    *   Move the core QFH/QBSA loops into CUDA kernels in `compat/quantum_kernels.cu`.
    *   Use the `compat` layer to switch between CPU and GPU implementations.