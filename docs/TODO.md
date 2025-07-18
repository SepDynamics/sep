# TODO.md: Truly Datatype-Agnostic Pattern Metric Engine

**Last Updated:** July 17, 2025  
**Goal:** Build a datatype-agnostic pattern metric engine on top of the existing SEP codebase. The engine ingests raw data from any source (sockets, files, directories, streams, memory buffers, etc.), treats it as a byte stream or bitfield, detects patterns without assuming any format, computes metrics via QBSA and QFH (coherence, stability, entropy, mutation rate, relationships), and outputs results. No filetype assumptions—everything is raw bytes/bits until processed by QBSA/QFH.

**Key Principles:**
- **True Agnosticism:** Ingest as raw bytes/bits. No format detection/pre-parsing unless explicitly added later. QBSA/QFH operate directly on bitfields (as in `processPatternBits`), so focus on converting inputs to bit vectors.
- **Leverage Existing Code:** Core on `quantum/processor.h`, `pattern_processor.cpp`, `quantum_processor_qfh.h` (QBSA/QFH impl), and memory tiers. Use `QuantumProcessorQFHCommon::processPatternBits` for analysis.
- **Metrics Focus:** QBSA for binary state misalignments/flips; QFH for hierarchy/ruptures. Output raw metrics without format ties.
- **Testing Priority:** First test with raw Oanda data as bytes (load file/socket as binary). Aim for working QBSA/QFH on it **today**.
- **Phased Approach:** Small steps. Complete Phase 1 today.
- **Tools & Dependencies:** Use `<fstream>`, `<vector<char>>` for raw bytes. For sockets, standard `<sys/socket.h>`. No JSON/Curl unless needed for optional output.
- **Error Handling:** Follow `util/error_handling.h` and `core/error_handler.h`.
- **Type Safety:** Use `util/type_safety.h` for operations.

## Phase 1: Core Engine Skeleton & Raw Oanda Byte Test (Complete Today)
Focus: Minimal engine ingests raw bytes, converts to bitfield, runs QBSA/QFH, outputs metrics. Test with Oanda data as raw file bytes.

1. **Define Engine Interface**
   - Create `include/sep/pattern_metric_engine.h` with class `PatternMetricEngine`.
   - Methods:
     - `void ingestRawBytes(const std::vector<uint8_t>& bytes);` (core ingest—everything converts to this)
     - `void ingestFromFile(const std::string& path);` (read file as bytes, call ingestRawBytes)
     - `void ingestFromSocket(int socket_fd);` (read stream as bytes, call ingestRawBytes)
     - `std::vector<PatternMetrics> computeMetrics();` (run QBSA/QFH on bits)
   - `struct PatternMetrics { QFHResult qfh; /* Add QBSA result struct */ float coherence, stability, entropy; };`
   - Integrate directly with `QuantumProcessorQFHCommon` for `processPatternBits` (convert bytes to `std::vector<uint32_t>` bitfield).

2. **Implement Raw Byte Ingestion**
   - In `src/pattern_metric_engine.cpp`, add byte loader.
   - For file: `std::ifstream f(path, std::ios::binary); std::vector<uint8_t> bytes(std::istreambuf_iterator<char>(f), {});`
   - For socket: Read in loop until EOF/error, append to vector.
   - Convert bytes to bitfield: Pack into `std::vector<uint32_t>` (e.g., 4 bytes per uint32).
   - Store as internal bitfield for QBSA/QFH.

3. **Run QBSA/QFH on Bits**
   - In computeMetrics: Call `QuantumProcessorQFHCommon::processPatternBits(bitfield);`
   - Get `QFHResult` and derive metrics (coherence from flip_ratio, etc.).
   - No format assumptions—raw bits are the input.

4. **Output Metrics**
   - Print to console or file as plain text/key-value (no JSON unless requested).
   - E.g., "Coherence: 0.85\nStability: 0.92\n..."

5. **Write Raw Oanda Byte Test**
   - In `/sep/src/tests/raw_data_test.cpp` (add to CMake).
   - Load Oanda file as bytes.
   - Create engine, ingestFromFile, computeMetrics.
   - ASSERT metrics make sense (e.g., coherence > 0).
   - Run with `ctest -R raw_data_test`.

6. **Build & Debug**
   - Fix errors (e.g., ambiguous calls—qualify namespaces).
   - Test small byte vectors first.


## Phase 2: Expand Sources & Refinements 
1. **Add Directory/Socket Ingestion**
   - Scan dir, ingest each file as bytes, aggregate bitfields.
   - Socket: Non-blocking read loop.

2. **Advanced Metrics**
   - Full QBSA/QFH integration.
   - Custom: Bit entropy, pattern repeats.

3. **Optimization**
   - Memory tiers for large bitfields.
   - GPU accel via compat.

