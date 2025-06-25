# Quantum Processing Flow

This diagram illustrates how data moves through the quantum-processing implementation in `src/quantum`. Pattern information originates in the tiered memory system and is processed through several layers before being written back.

```
+---------------------+
| Memory/Core         |
| (MemoryTierManager, |
|  DagGraph, Hooks)   |
+----------+----------+
           |
           v
+----------+----------+
| Pattern Processor   |
+----------+----------+
           |
           v
+---------------------+
| Pattern Quantum     |
| Processor (PQP)     |
+----------+----------+
           |
           v
+---------------------+
| Quantum Processor   |
|  (QBSA/QFH logic)   |
+----------+----------+
           |
           v
+---------------------+
| GPU/Core Algorithms |
|  (Evolution, Kernels)|
+----------+----------+
           |
           v
+----------+----------+
| Updated States      |
| -> Memory/Core      |
+---------------------+
```

1. **Memory/Core**: `MemoryTierManager` stores `PatternData` with associated `QuantumState`. Core utilities (hooks, DAG) provide system services.
2. **Pattern Processor**: Coordinates evolution and mutation of patterns; interacts with `MemoryTierManager` to fetch or store data.
3. **Pattern Quantum Processor**: Bridges high-level pattern logic with the lower-level `QuantumProcessor` API.
4. **Quantum Processor**: Implements coherence/stability calculations using QBSA and QFH algorithms. It updates the in-memory state of each pattern.
5. **GPU/Core Algorithms**: When GPU support is enabled, kernels in `compat` accelerate calculations.
6. **Updated States**: Results are written back to the memory tiers and the DAG for future processing cycles.

The flow forms a loop: patterns are retrieved from memory, processed through these layers, and the updated results are stored back, ready for the next iteration.
