# Core Module Overview

The headers and source files reside in `src/core`.

## Header Details

This document illustrates how data flows through the headers inside
`src/core` and how they connect to higher level modules such as
`api`, `quantum`, and `memory`. Each header is represented by a Mermaid
node. Arrows show the primary direction of data or control flow.

```mermaid
graph TD
    subgraph Core
        manager[manager.h]
        env_keys[env_keys.h]
        types[types.h]
        common[common.h]
        engine[engine.h]
        dag[dag_graph.h]
        error_handler[error_handler.h]
        metrics[metrics_collector.h]
        prometheus[prometheus_exporter.h]
        allocation[allocation_metrics.h]
        tracing[tracing.h]
        hooks[system_hooks.h]
    end

    env_keys --> manager
    types --> manager
    common --> engine
    types --> engine
    manager --> engine
    engine --> dag
    engine --> metrics
    engine --> error_handler
    engine --> hooks
    metrics --> prometheus
    metrics --> allocation
    tracing --> metrics
    dag --> memory["memory module"]
    engine --> quantum["quantum module"]
    engine --> api["api module"]
    metrics --> api
    prometheus --> api
    error_handler --> api
    memory --> api
    quantum --> api
```

### Notes

- **Configuration flow**: `env_keys.h`, `types.h`, and `manager.h` work
together to load configuration values which are then consumed by
`engine.h` and other modules.
- **Metrics flow**: `metrics_collector.h` gathers performance data. It
feeds `prometheus_exporter.h` and `allocation_metrics.h` so metrics can
be published through the `api`.
- **DAG updates**: `dag_graph.h` tracks pattern lineage and is mainly
manipulated by the engine and `memory` module.
- **Error handling**: `error_handler.h` centralizes reporting from core
and higher-level modules.
- **System hooks**: `system_hooks.h` provides callbacks that modules can
implement to observe engine events.

## Implementation Details

This document illustrates how data moves through the main subsystems inside `src/core`.
Each section provides a high level flowchart describing the major interactions.

## Engine Initialization

```mermaid
flowchart TD
    config(CudaConfig)
    Engine -->|init| CudaCore
    CudaCore -->|createStream| Stream
    Engine -->|allocate DeviceMemory| DeviceMemory
    Engine --> AudioCapture
    Engine --> BlenderBridge
    Stream --> Engine
    DeviceMemory --> Engine
```

- The `Engine` loads configuration, initializes the CUDA runtime via `CudaCore`, and creates the default stream.
- Device memory buffers are allocated for bitfields, probe indices, expectations, and other working data.
- Optional subsystems such as audio capture and the Blender bridge are created if available.
- Once complete, the engine is ready to process batches of quantum state data.

## Metrics Collection

```mermaid
flowchart TD
    Operations -->|recordOperation| MetricsCollector
    MetricsCollector -->|updateSystemMetrics| CollectorThread
    CollectorThread -->|collectMetrics| GPU
    CollectorThread --> CPU
    CollectorThread --> Memory
    MetricsCollector --> DetailedMetrics
```

- User operations call `recordOperation`, `incrementCounter`, or `setGauge` which update in-memory metrics.
- A background collector thread periodically gathers GPU, CPU, and memory usage, storing the latest values in `DetailedMetrics`.
- Kernel start/stop events feed execution timing information back into the metrics structures.

## Error Handling

```mermaid
flowchart TD
    Module -->|reportError| ErrorHandler
    ErrorHandler --> ErrorQueue
    ErrorQueue -->|processRetries| Module
    ErrorHandler -->|getErrors| API
```

- Any module can report an error with optional retry logic to the singleton `ErrorHandler`.
- Errors are stored in an internal queue. On each report, the handler attempts retries up to three times.
- External callers can query or clear the error list via the API layer.

## DAG Graph System

```mermaid
flowchart TD
    Engine -->|process_batch| StateNode
    StateNode -->|push_back| StateHistory
    StateHistory -->|coherence history| API
    Engine --> DagGraph
    DagGraph -->|add/update| Nodes
```

- During batch processing, the engine computes collapse indices and coherence information, creating a new `StateNode`.
- The node is appended to `StateHistory`, allowing inspection of coherence over time.
- A separate `DagGraph` tracks relationships between patterns, storing parent links and coherence values.
- Clients can access both the state history and graph data to visualize system evolution.

