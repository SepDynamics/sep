# Quantum Processing Header Flow

This chart outlines how data moves through the key quantum-processing headers in the SEP engine. Patterns originate either from in-memory data structures or via the engine's HTTP API. They then pass through several stages of analysis before results are stored back in memory or returned through the API.

```
[Memory / API Input]
        |
        v
+----------------------+
| quantum/processor.h  |
+----------------------+
        |
        v
+-----------+
| qbsa.h    |
+-----------+
        |
        v
+-----------+
| qfh.h     |
+-----------+
        |
        v
+---------------------------+
| quantum_processor_qfh.h   |
+---------------------------+
        |
        v
+-------------------------+
| quantum_processor.h     |
+-------------------------+
        |
     Results
        v
[Memory / API Output]
```

**Header Roles**

- `quantum/processor.h` – Base pattern processor that orchestrates analysis.
- `quantum/qbsa.h` – Implements Quantum Binary State Analysis to find bit-level corrections.
- `quantum/qfh.h` – Performs the Quantum Fourier Hierarchy transform for detecting coherence ruptures.
- `quantum/qbsa_qfh.h` & `quantum/quantum_processor_qfh.h` – Combine QBSA with QFH enhancements for advanced mutation and stability checks.
- `quantum/quantum_processor.h` – Provides the public interface for quantum pattern processing and finalizes results.

Data fed through these headers is ultimately written back into the memory tiers or delivered to external systems via the API, closing the loop.
