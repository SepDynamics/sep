# SEP Engine: System Architecture


```mermaid
graph TB
    %% Main Executable
    subgraph "Executable Layer"
        exe[sep_engine<br/>Final Binary]
    end

    %% Static Libraries with detailed component mapping
    subgraph "Static Library Layer"
        subgraph "API & Interface"
            api[libsep_api.a<br/>HTTP/C Bridge/Rate Limiting<br/>└─ server.cpp<br/>└─ bridge_c.cpp<br/>└─ crow_adapter.cpp<br/>└─ sep_engine.cpp]
        end

        subgraph "Domain Components"
            blender[sep_blender<br/>Cycles Integration<br/>└─ cycles_renderer.cpp<br/>└─ mesh_handler.cpp<br/>└─ gpu_context.cpp]
            audio[libsep_audio.a<br/>PipeWire Integration<br/>└─ pipewire_capture.cpp<br/>└─ pipeline.cpp]
        end

        subgraph "Quantum Processing"
            quantum[libsep_quantum.a<br/>QBSA/QFH Algorithms<br/>└─ processor.cpp<br/>└─ evolution.cpp<br/>└─ pattern_processor.cpp<br/>└─ qfh.cpp]
        end

        subgraph "Memory Management"
            memory[libsep_memory.a<br/>Tiered Storage<br/>└─ memory_tier_manager.cpp<br/>└─ quantum_coherence_manager.cpp<br/>└─ redis_manager.cpp]
        end

        subgraph "CUDA Backend"
            compat[libsep_compat.a<br/>GPU Abstraction<br/>└─ core.cu<br/>└─ stream.cpp<br/>└─ cuda_api.cu<br/>└─ quantum_kernels.cu<br/>└─ pattern_kernels.cu]
        end

        subgraph "Core Foundation"
            core[libsep_core.a<br/>Base Infrastructure<br/>└─ engine.cpp<br/>└─ manager.cpp<br/>└─ metrics_collector.cpp<br/>└─ logging.cpp<br/>└─ dag_graph.cpp]
        end
    end

    %% External Dependencies
    subgraph "Cycles Dependencies"
        cycles_kernel[libcycles_kernel.a]
        cycles_scene[libcycles_scene.a]
        cycles_device[libcycles_device.a]
        cycles_osl[libcycles_osl.a]
        osl_exec[liboslexec.so]
        osl_comp[liboslcomp.so]
        osl_query[liboslquery.so]
    end

    subgraph "System Libraries"
        cuda_runtime[CUDA Runtime<br/>cudart/cuda_driver]
        pthread[pthread]
        redis[hiredis]
        pipewire[pipewire-0.3]
    end

    %% Dependency Arrows with Link Order
    exe -.->|1| compat
    exe -.->|2| cycles_kernel
    exe -.->|3| cycles_scene
    exe -.->|4| cycles_device
    exe -.->|5| cycles_osl
    exe -.->|6| osl_exec
    exe -.->|7| osl_comp
    exe -.->|8| osl_query
    exe ==>|9| api
    exe ==>|10| memory
    exe ==>|11| quantum
    exe ==>|12| core
    exe ==>|13| audio
    exe ==>|14| blender

    %% Internal Dependencies
    api --> quantum
    api --> memory
    api --> core

    blender --> quantum
    blender --> memory
    blender --> cycles_kernel
    blender --> cycles_scene
    blender --> cycles_device
    blender --> cycles_osl
    blender --> core

    audio --> quantum
    audio --> memory
    audio --> core

    quantum --> compat
    quantum --> core
    quantum -.->|manifold symbols| memory

    memory --> core
    memory --> redis

    compat --> cuda_runtime


    %% Symbol Conflicts
    quantum x--x|multiple defs<br/>manifold::memory<br/>manifold::quantum<br/>manifold::cuda<br/>manifold::api| memory


    %% Styling
    classDef conflict fill:#ffaa66,stroke:#ff6600,stroke-width:3px
    classDef critical fill:#66ff66,stroke:#00ff00,stroke-width:2px
    
    class quantum,memory conflict
    class compat,core critical
```

## 1. Introduction

The SEP Engine is a high-performance C++ framework for quantum-inspired pattern analysis and evolution. It is designed to be a modular, scalable, and maintainable platform for simulating and exploring the principles of the **Recursive Framework for Emergent Reality**. The architecture prioritizes a clear separation of concerns, allowing for independent development and testing of its core components while enabling complex, emergent behaviors through their interaction.

This document provides a comprehensive overview of the engine's architecture, from the high-level system composition down to the detailed interaction flows between modules.

### Guiding Principles

The architecture is guided by these principles:

1.  **Clear Component Boundaries**: Each module has a distinct responsibility and a well-defined public interface located under `/src/sep`.
2.  **Unidirectional Dependencies**: High-level modules (like `api`) can depend on low-level modules (like `core`), but not the other way around. This prevents circular dependencies and promotes a clean build process.
3.  **Standard Project Layout**: The project follows a standard layout (`src`, `tests`, `third_party`, `assets`) for better tooling integration and developer onboarding.
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
        blender[sep_blender]
        audio[libsep_audio.a]
        quantum[libsep_quantum.a]
        memory[libsep_memory.a]
        compat[libsep_compat.a]
        core[libsep_core.a]
    end

    exe --> main
    main --> api
    main --> core

    api --> quantum
    api --> memory
    blender --> quantum
    blender --> memory
    audio --> quantum
    audio --> memory

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

### `embeddings` - Text Embedding Utilities
*   **Purpose**: Supplies lightweight text embeddings for pattern generation and testing.
*   **Key Files**: `simple_embedding_model.cpp`, `simple_embedding_model.h`.
*   **Dependencies**: None.
*   **Diagrams**: [embeddings.md](diagrams/embeddings.md).

### `api` - The Public Interface
*   **Purpose**: Exposes the engine's functionality to the outside world via an HTTP server (Crow) and a stable C-style bridge.
*   **Key Files**: `server.cpp`, `sep_engine.cpp` (facade), `bridge_c.cpp`, `rate_limit_middleware.cpp`.
*   **Dependencies**: `core`, `quantum`, `memory`.

### `blender` & `audio` - Specialized Integrations
*   **Purpose**: These are optional, platform-specific integrations that can be enabled or disabled at build time.
*   **Key Files**:
    *   **blender**: `api.cpp`, `blender_integration.cpp`, `mesh_handler.cpp`, `cycles_renderer.cpp`.
    *   **audio**: `pipewire_capture.cpp`, `pipeline.cpp`.
*   **Dependencies**: `core`, `quantum`, `memory`.
*   **Rationale**: Keeping these integrations as separate modules prevents their specific dependencies (e.g., Blender headers, PipeWire) from polluting the core engine build.
*   **Cycles Integration**: The `cycles_renderer.cpp` provides pattern-driven rendering through Blender's Cycles renderer when `SEP_HAS_CYCLES` is enabled.
*   **Cycles Source**: Headers are accessed via the `src/cycles_src` symlink. See [cycles_src.md](diagrams/cycles_src.md) for details.

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
    F[src/memory/unified_memory.h] -- calls --> G(allocateUnifiedMemory)
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