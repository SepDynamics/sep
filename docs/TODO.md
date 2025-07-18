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

## Phase 1: Core Engine Implementation (Completed)
**Status:** Complete.

**Accomplishments:**
- Successfully designed and implemented the `PatternMetricEngine` as a datatype-agnostic component.
- The engine ingests data via raw byte pointers (`const uint8_t*`) and C++ input streams (`std::istream`).
- It correctly processes various data types, including binary, text, and numerical data, by treating them as byte sequences.
- Integrated the Quantum Fourier Hierarchy (QFH) processor to compute key metrics: coherence, stability, and entropy.
- All unit tests pass, verifying the engine's functionality and its agnostic design.

## Phase 2: Expansion and Integration
**Goal:** Expand the engine's capabilities to handle more complex data sources, enhance its analytical power, and integrate it more deeply with the SEP ecosystem.

1.  **Advanced Ingestion Capabilities:**
    -   **File/Directory Ingestion:** Implement methods to ingest entire files or recursively scan directories, treating each file as a distinct data stream.
    -   **Socket/Stream Ingestion:** Add support for real-time data ingestion from network sockets and other streaming sources.

2.  **Advanced Metrics and Pattern Detection:**
    -   Implement more sophisticated pattern detection algorithms beyond simple byte-level analysis.
    -   Integrate the full QBSA (Quantum Binary State Analysis) feature set for more detailed metrics.
    -   Explore algorithms for detecting emergent and evolving patterns over time.

3.  **Integration with Memory Tiers:**
    -   Connect the engine to the memory tier system (`short-term`, `mid-term`, `long-term`).
    -   Develop logic for promoting significant patterns to higher memory tiers based on their metrics (e.g., high coherence or stability).

4.  **Performance and Optimization:**
    -   Optimize the pattern extraction and metric computation algorithms for high-throughput scenarios.
    -   Leverage GPU acceleration for quantum processing tasks where applicable.
    -   Investigate strategies for managing memory usage with very large data sets.

