# SEP Engine - Quantum Pattern Processing Framework

*Personal project management README*

## Current Status: COMPILING BUILD

The quantum framework consolidation is complete. All previous modular components (context, pattern, etc.) have been unified into a cohesive system built around quantum-inspired algorithms.

## Architecture Overview

### Core Modules (Static Libraries)

**`libsep_core.a`** - Foundation Layer
- `engine.cpp` - Main Engine orchestrator
- `manager.cpp` - ConfigManager singleton
- `metrics_collector.cpp`, `prometheus_exporter.cpp`, `tracing.cpp` - Unified metrics
- `error_handler.cpp` - Error reporting singleton
- `dag_graph.cpp` - Pattern lineage tracking
- `allocation_metrics.cpp` - Memory allocation tracking

**`libsep_quantum.a`** - The Algorithm Core
- `qbsa.cpp`, `qbsa_qfh.cpp` - Quantum Binary State Analysis
- `qfh.cpp` - Quantum Fourier Hierarchy
- `quantum_processor.cpp`, `quantum_processor_qfh.cpp` - Main processors
- `evolution.cpp` - Pattern evolution algorithms
- `processor.cpp` - Pattern processing logic
- `pattern_processor.cpp`, `pattern_processor_interface.cpp` - Pattern interface
- `types_serialization.cpp` - Type serialization

**`libsep_memory.a`** - Tiered Storage System
- `memory_tier_manager.cpp` - Manages STM/MTM/LTM hierarchy
- `memory_tier.cpp` - Individual tier implementations
- `redis_manager.cpp` - Redis persistence backend
- `manager.cpp` - Memory management coordinator

**`libsep_context.a`** - Business Logic Layer
- `processor.cpp` - Context processing
- `relationship.cpp` - Pattern relationship management

**`libsep_api.a`** - External Interface
- `server.cpp` - HTTP server (Crow-based)
- `sep_engine.cpp` - Main facade
- `bridge_c.cpp` - C-style bridge
- `crow_adapter.cpp`, `crow_error.cpp` - Crow integration
- `auth_middleware.cpp`, `rate_limit_middleware.cpp` - HTTP middleware
- `ollama_client.cpp` - Ollama integration
- `js_integration.cpp` - JavaScript bridge

**`libsep_compat.a`** - CUDA Backend
- `core/core.cu` - CudaCore singleton
- `core/cuda_wrapper.cu` - CUDA Runtime wrappers
- `quantum_kernels.cu`, `pattern_kernels.cu` - GPU kernels
- `raii.cpp` - CUDA resource management
- `core_stub.cpp` - CPU fallback stubs

**`libsep_blender.a`** - Blender Integration (Optional)
- `api.cpp` - Blender API interface
- `blender_integration.cpp` - Core integration
- `mesh_handler.cpp` - Mesh processing
- `gpu_context.cpp` - GPU context management
- `compression.cpp`, `compression_utils.cpp` - Data compression
- `pattern_visualization_pipeline.cpp` - Visualization pipeline

**`libsep_audio.a`** - Audio Processing (Optional)
- `pipewire_capture.cpp` - PipeWire audio capture
- `pipeline.cpp` - Audio processing pipeline
- `config.cpp` - Audio configuration

## Key Quantum Framework Components

### Pattern Types
- **Pattern**: Core pattern structure with quantum state
- **QuantumState**: coherence, stability, entropy, generation, memory_tier
- **PatternRelationship**: ENTANGLEMENT, CAUSAL, SIMILARITY types

### Memory Tiers
- **STM** (Short Term Memory): New/unstable patterns
- **MTM** (Medium Term Memory): Intermediate patterns
- **LTM** (Long Term Memory): Stable/coherent patterns

### Quantum Processors
- **QBSA** (Quantum Binary State Analysis): Core pattern analysis
- **QFH** (Quantum Fourier Hierarchy): Frequency domain processing
- **QuantumProcessorQFH**: Main processing interface

## Build System

### Static Library Dependencies
```
sep_engine (main executable)
├── libsep_api.a
│   ├── libsep_context.a
│   │   ├── libsep_quantum.a
│   │   │   └── libsep_core.a
│   │   └── libsep_memory.a
│   │       └── libsep_core.a
│   └── libsep_core.a
├── libsep_compat.a (CUDA support)
├── libsep_blender.a (optional)
└── libsep_audio.a (optional)
```

### Build Configuration
- **Compiler**: Clang++ with C++17
- **CUDA**: 12.9 (optional, controlled by SEP_CUDA_AVAILABLE)
- **Dependencies**: Crow, GLM, TBB, Redis, spdlog, fmt
- **Optional**: Blender (SEP_HAS_BLENDER), Redis (SEP_HAS_HIREDIS)

## Key Directories

```
sep-engine/
├── src/                    # Source modules
│   ├── core/              # Foundation
│   ├── quantum/           # Algorithms
│   ├── memory/            # Storage tiers
│   ├── context/           # Business logic
│   ├── api/               # HTTP interface
│   ├── compat/            # CUDA backend
│   ├── blender/           # Blender integration
│   └── audio/             # Audio processing
├── include/sep/           # Public headers
├── tests/                 # Test suites
├── docs/                  # Architecture docs
└── third_party/          # External dependencies
```

## Current State

### Working Components
- ✅ Quantum framework compiles and links
- ✅ QBSA and QFH algorithms integrated
- ✅ Three-tier memory system operational
- ✅ HTTP API server functional
- ✅ CUDA backend with CPU fallback
- ✅ Pattern processing pipeline complete

### Development Notes
- All previous modular components successfully consolidated
- Pattern/context modules merged into unified quantum framework
- CUDA compilation working with proper fallback stubs
- Redis integration for LTM persistence
- Comprehensive test suite covering all modules

### Build Status
- **Last successful build**: Links all libraries into `sep_engine` executable
- **CUDA support**: Optional, falls back to CPU implementations
- **Dependencies**: All external libraries properly linked

## Usage Patterns

### Engine Initialization
The main `Engine` class orchestrates all subsystems and manages the quantum processor lifecycle.

### Pattern Processing
Patterns flow through the quantum algorithms (QBSA → QFH) and are automatically tiered into STM/MTM/LTM based on coherence and stability metrics.

### API Access
HTTP server exposes pattern processing endpoints with authentication and rate limiting middleware.

## Memory Management
- Automatic pattern promotion/demotion between memory tiers
- Relationship tracking with coherence-based pruning
- Optional Redis persistence for LTM patterns
- CUDA memory management with RAII wrappers

---

*This is a private project management document. The system is designed around quantum-inspired pattern analysis with a focus on stability, coherence, and evolutionary relationships.*
