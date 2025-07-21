# SEP Workbench Engine - Standalone Architecture

## Critical Implementation Requirements

### Primary Objective
Create a **standalone window executable** that runs the SEP Engine directly within the workbench, providing real-time pattern analysis without requiring external services.

### Core Metrics Required
The workbench must display these three fundamental SEP metrics in real-time:

1. **Coherence**: Internal self-similarity and consistency of patterns
2. **Stability**: Resistance to change over time  
3. **Entropy**: Complexity and unpredictability

### Architecture Overview

```
SEP Workbench Executable
├── Offline Engine (Primary)
│   ├── PatternMetricEngine
│   ├── QBSA Algorithm (Coherence Prober)
│   └── QFH Algorithm (Phase Aligner)
├── Metrics Dashboard (Main Interface)
│   ├── Real-time Graphs
│   ├── Pattern Statistics
│   └── Data Source Controls
└── UI Framework (ImGui)
```

### Key Implementation Details

#### Engine Integration
- **NO SERVICE CONNECTION**: The workbench IS the engine
- Use existing `offline_engine_` as the primary engine
- Connect `PatternMetricEngine` directly to `MetricsDashboard`
- Skip all service connection logic

#### Critical Fixes Applied
- **NEVER return nullptr**: All engine connection methods must return valid pointers
- Service connector fixed to always provide working engine reference
- Memory manager fallback tiers prevent allocation failures

#### Data Flow
1. User loads data file through dashboard
2. `MetricsMonitor` ingests data into `PatternMetricEngine`
3. Engine processes data with QBSA/QFH algorithms
4. Real-time metrics (Coherence/Stability/Entropy) displayed
5. Pattern statistics updated continuously

### Deliverable Proof Requirements

#### Window Executable Features
- [x] Standalone executable (`sep_workbench`)
- [x] Metrics dashboard as primary interface
- [x] Real-time pattern analysis display
- [x] Data file loading capabilities
- [x] Export functionality for results

#### Testing Requirements
- Load any data file (text, binary, financial data)
- Display live Coherence/Stability/Entropy metrics
- Show pattern detection statistics
- Export analysis results to JSON
- Clear/reset for new analysis

### Build Commands
```bash
./build.sh
./build/sep_workbench
```

### Success Criteria
When launched, the workbench should:
1. Open directly to the metrics dashboard
2. Display "SEP Pattern Metrics Engine v2.0.0"
3. Show working pattern analysis interface
4. Process data files and display real-time metrics
5. Function entirely offline without service dependencies

## Critical Failure Documentation

### Issue: Engine Connection Failures
**Symptom**: "Service engine is null, falling back to offline mode"
**Root Cause**: nullptr returns in service connector preventing engine connection
**Fix Applied**: All connector methods now return valid engine pointers, never nullptr

### Issue: Missing Dependencies  
**Symptom**: Build failures with missing headers
**Fix Applied**: Updated CMakeLists.txt with required dependencies (nlohmann_json, fmt, TBB)

### Resolution Status: ✅ IMPLEMENTED
The workbench now operates as a standalone engine with integrated metrics dashboard.
