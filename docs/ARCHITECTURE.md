Of course. After analyzing all the provided materials—the source code, the architectural plans, and the newly drafted Mermaid diagrams—I have created a new, comprehensive architecture document.

This document replaces the previous `ARCHITECTURE.md` and serves as the primary technical reference for the SEP Engine. It provides a high-level system overview, details the purpose and dependencies of each module, and uses your diagrams to illustrate the data and control flows.

---

# SEP Engine: System Architecture

## 1. Introduction

The SEP Engine is a high-performance C++ framework for quantum-inspired pattern analysis and evolution. It is designed to be a modular, scalable, and maintainable platform for simulating and exploring the principles of the **Recursive Framework for Emergent Reality**. The architecture prioritizes a clear separation of concerns, allowing for independent development and testing of its core components while enabling complex, emergent behaviors through their interaction.

This document provides a comprehensive overview of the engine's architecture, from the high-level system composition down to the detailed interaction flows between modules.

### Guiding Principles

The architecture is guided by these principles:

1.  **Clear Component Boundaries**: Each module has a distinct responsibility and a well-defined public interface located in `/include/sep`.
2.  **Unidirectional Dependencies**: High-level modules (like `api`) can depend on low-level modules (like `core`), but not the other way around. This prevents circular dependencies and promotes a clean build process.
3.  **Standard Project Layout**: The project follows a standard layout (`src`, `include`, `tests`, `third_party`, `assets`) for better tooling integration and developer onboarding.
4.  **Consolidation of Core Logic**: Cross-cutting concerns like logging, metrics, error handling, configuration, and the Directed Acyclic Graph (DAG) are unified into a single, foundational `core` library.
5.  **Isolate External Dependencies**: Third-party libraries (e.g., Crow, nlohmann, hiredis) are kept separate from the engine's source code and are linked appropriately during the build process.

## 2. High-Level System Diagram

The SEP Engine is compiled into a single executable, `sep_engine`, which links a set of self-contained static libraries. This design ensures modularity and allows for optional components (like Blender or audio integration) to be included or excluded at build time. The dependency graph is strictly unidirectional, flowing from high-level interfaces down to the foundational core.

```mermaid
graph TD
    subgraph Executable
        exe[sep_engine]
    end

    subgraph Source
        main[main.cpp]
    end

    subgraph "Static Libraries (.a)"
        api[libsep_api.a]
        blender[libsep_blender.a]
        audio[libsep_audio.a]
        context[libsep_context.a]
        quantum[libsep_quantum.a]
        memory[libsep_memory.a]
        compat[libsep_compat.a]
        core[libsep_core.a]
    end

    exe --> main
    main --> api
    main --> core

    api --> context
    blender --> context
    audio --> context

    context --> quantum
    context --> memory

    quantum --> compat
    quantum --> core
    memory --> core
    compat --> core
```

## 3. Component Breakdown

Each module is built as a self-contained static library, providing a clear and reusable unit of functionality.

### `core` - The Foundation
*   **Purpose**: Provides the foundational utilities, data structures, and managers required by all other engine modules. It has no dependencies on other SEP modules.
*   **Key Files**: `engine.cpp`, `manager.cpp` (ConfigManager), `metrics_collector.cpp`, `error_handler.cpp`, `dag_graph.cpp`.
*   **Dependencies**: None.
*   **Rationale**: The previous `config`, `metrics`, and `dag` modules were merged into `core` to create a single, robust foundational library (`libsep_core.a`), which simplifies the dependency graph significantly.

### `compat` - CUDA Backend & Shims
*   **Purpose**: Provides the complete CUDA backend for GPU acceleration, along with compatibility shims required for building in non-GPU environments.
*   **Key Files**: `core.cu` (CudaCore singleton), `quantum_kernels.cu`, `pattern_kernels.cu`, `raii.cpp` (RAII wrappers).
*   **Dependencies**: `core`.
*   **Rationale**: This module abstracts all GPU-specific implementations, allowing the rest of the engine to remain portable.

### `quantum` - The Algorithms
*   **Purpose**: Contains the "secret sauce"—the quantum-inspired algorithms for analyzing and evolving patterns, including QBSA and QFH.
*   **Key Files**: `qbsa.cpp`, `qfh.cpp`, `evolution.cpp`, `processor.cpp`.
*   **Dependencies**: `core`, `compat`.

### `memory` - Tiered Storage System
*   **Purpose**: Manages the three-tiered memory hierarchy (STM, MTM, LTM) and handles optional pattern persistence via Redis.
*   **Key Files**: `memory_tier_manager.cpp`, `memory_tier.cpp`, `redis_manager.cpp`.
*   **Dependencies**: `core`.

### `context` - High-Level Logic
*   **Purpose**: Acts as the "business logic" layer. It uses the `quantum` algorithms and `memory` tiers to process high-level `Context` objects, extract embeddings, and manage relationships.
*   **Key Files**: `processor.cpp`, `relationship.cpp`.
*   **Dependencies**: `core`, `quantum`, `memory`.
*   **Rationale**: This logic is a clear, high-level abstraction that separates the core algorithms from their application.

### `api` - The Public Interface
*   **Purpose**: Exposes the engine's functionality to the outside world via an HTTP server (Crow) and a stable C-style bridge.
*   **Key Files**: `server.cpp`, `sep_engine.cpp` (facade), `bridge_c.cpp`, `rate_limit_middleware.cpp`.
*   **Dependencies**: `core`, `context`.

### `blender` & `audio` - Specialized Integrations
*   **Purpose**: These are optional, platform-specific integrations that can be enabled or disabled at build time.
*   **Key Files**:
    *   **blender**: `api.cpp`, `blender_integration.cpp`, `mesh_handler.cpp`.
    *   **audio**: `pipewire_capture.cpp`, `pipeline.cpp`.
*   **Dependencies**: `core`, `context`.
*   **Rationale**: Keeping these integrations as separate modules prevents their specific dependencies (e.g., Blender headers, PipeWire) from polluting the core engine build.

## 4. Detailed Interaction and Data Flows

The following diagrams illustrate how data and control flow between and within the modules.

### HTTP API Request Flow
This sequence shows how an external HTTP request is processed, from the web server down to the core engine components and back.

```mermaid
sequenceDiagram
    participant Client
    participant Crow as CrowApp
    participant RateLimit as RateLimitMiddleware
    participant Auth as AuthMiddleware
    participant Server as SEPApiServer
    participant Engine as SepEngine
    participant Quantum as QuantumProcessor
    participant Memory as MemoryTierManager

    Client->>Crow: HTTP request
    Crow->>RateLimit: before_handle()
    RateLimit-->>Crow: allow or reject
    Crow->>Auth: before_handle()
    Auth-->>Crow: allow or reject
    Crow->>Server: route handler
    Server->>Engine: call (e.g. processPatterns)
    Engine->>Quantum: compute coherence
    Engine->>Memory: persist & retrieve
    Engine-->>Server: JSON result
    Server-->>Client: HTTP response
```

### Core Module Dependency Map
The `core` module provides foundational services consumed by nearly every other part of the engine.

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

### Quantum Processing Flow
This diagram illustrates the pipeline for processing pattern data through the quantum-inspired algorithms.

```mermaid
graph TD
    A("Memory/Core (Input)") --> B("Pattern Processor");
    B --> C("Pattern Quantum Processor (PQP)");
    C --> D("Quantum Processor (QBSA/QFH)");
    D --> E("GPU/Core Algorithms (Kernels)");
    E --> F("Updated States --> Memory/Core");

    subgraph "src/quantum"
        B; C; D; E;
    end
```

### Memory Management Flow
The tiered memory system manages the lifecycle of patterns based on their coherence and stability.

```mermaid
graph TD
  subgraph Headers
    MT["memory_tier.hpp"]
    MTMGR["memory_tier_manager.hpp"]
    UM["unified_memory.h"]
    LOG["logger.hpp"]
    TYPES["types.h"]
  end

  MTMGR --> MT
  UM --> MTMGR
  MTMGR --> TYPES
  MT --> TYPES
  UM --> LOG
```

### CUDA Compatibility Layer
The `compat` module abstracts GPU interactions, allowing other modules to remain portable.

```mermaid
graph TD
    A[src/core/engine.cpp] -- uses --> B[CudaCore]
    A -- allocates --> C[DeviceMemory]
    A -- creates --> D[Stream]
    F[include/memory/unified_memory.h] -- calls --> G(allocateUnifiedMemory)
    G --> H[Tiered Memory Manager]
    H -->|returns| I[Unified pointer]
    J[src/core/metrics_collector.cpp] -- records --> K[cudaEvent_t]
```

### Specialized Integration Flows

#### Blender Integration
`src/blender` provides the C API and visualization pipeline for rendering SEP patterns.
```mermaid
graph TD
    subgraph BlenderHeaders
        api_h[api.h]
        bridge_h[bridge.h / pattern_bridge.h]
        base_types_h[base_types.h]
        observer_h[pattern_observer.h]
    end

    subgraph Core
        core_types[core/types.h]
        memory_tiers[memory/memory_tier.hpp]
    end

    subgraph API
        c_api[src/blender/api.cpp]
    end

    c_api --> api_h
    api_h --> bridge_h
    bridge_h --> core_types
    bridge_h --> memory_tiers
    bridge_h --> observer_h
    observer_h --> core_types
    base_types_h --> core_types
```

#### Audio Integration
`src/audio` captures audio via PipeWire and converts it into pattern vectors for the engine.
```mermaid
graph TD
    A([PipeWire Device]) --> B(PipeWireCapture)
    B --> C[processAudioFrame]
    C --> D[applyHannWindow]
    D --> E[performFFT]
    E --> F[calculateSpectralFeatures]
    F --> G[convertToPattern]
    G --> H([Pattern Queue])
    H --> I([Memory Tiers / Engine])
```

## 5. Conclusion

This architecture establishes a clean, modular, and high-performance foundation for the SEP Engine. By separating core logic from interfaces and platform-specific implementations, the system is well-positioned for future expansion, testing, and application. The unidirectional dependency flow ensures maintainability and simplifies the build process, while the tiered memory and quantum processing modules provide the power and flexibility needed to explore the principles of the Recursive Framework for Emergent Reality.