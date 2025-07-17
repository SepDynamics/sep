# TODO.md: Roadmap to Standalone SEP Engine Demo for Quant Trading

This roadmap is tailored to your current codebase snapshot and the Proposal.md goal: a demo of the SEP Engine as a standalone processor for quantitative trading data. Focus: Analyze market data (e.g., options/stocks) for pattern coherence/evolution, generate DAG representations (using existing dag_graph.h), and output metrics (e.g., stability, generation via quantum_state). 

**Assessment Insights**:
- **Current State**: Core components (quantum/processor.h, memory_tier_manager.hpp, quantum_coherence_manager.h, dag_graph.cpp) exist for pattern processing/coherence/DAGs. Quantum kernels (QBSA/QFH in qbsa.cpp/qfh.cpp) are GPU-ready but plagued by CUDA ambiguities/incomplete types (e.g., cudaStream_t in gpu_context.h). API/bridge (bridge.cpp, client.cpp) can handle inputs/outputs. Non-relevant parts (workbench/demos like genesis_pattern, audio/pipewire, blender/cycles) cause build defects (e.g., missing scene.h, blocking fgets) and bloat—disabling them reduces ~60% of defects/dependencies (no ImGui/GLM/GLAD needed for standalone).
- **Gaps**: No direct quant data handling (e.g., CSV parsers for market feeds); DAG is basic (needs quant-specific nodes like volatility patterns); metrics output is console/JSON-ready but lacks trading specifics (e.g., alpha generation from coherence). Standalone mode: Engine runs via api_main.cpp/sep_engine.cpp, but needs CLI entry for demo.
- **Strategy**: Disable visuals/demos to stabilize; build data pipeline around existing quantum/memory cores; test with sample quant data (e.g., options chains). Expand steps as needed (e.g., if CUDA fixes reveal kernel bugs, add sub-tasks). Proceed sequentially: Clean/stabilize first, then pipeline, then demo. Use code_execution tool for quick tests (e.g., verify DAG additions).

Proceed one major step at a time; after each, rebuild (`ninja`), run analyzer, and test core functionality (e.g., process mock patterns).

## Step 1: Cleanup and Disable Non-Essential Components
- **Insight**: Demos (e.g., genesis_pattern) rely on removed deps (GLM for positions, ImGui for UI, Cycles for render)—cause cycles/incomplete types (e.g., gpu_context.h errors in demos). Audio/Blender add unrelated defects (blocking calls, missing headers). Disabling slims build, fixes ~50 defects (e.g., ImGui dead stores), focuses on core/API for quant pipeline.
- Remove workbench/demos dir (including genesis_pattern.*—not needed for quant; it's a visual sim).
- Delete src/audio, src/blender dirs.
- In CMakeLists.txt: Remove `add_subdirectory(workbench/audio/blender)`; excise links/includes for GLM/ImGui/GLAD/Cycles/Pipewire.
- Strip refs: In engine.h/core files, remove demo_manager.hpp, CyclesRenderer; stub `on_ui_render`/render calls if lingering.
- Replace GLM uses: In quantum/memory (e.g., vec4 positions), use std::array<float,4> or raw floats.
- Rebuild: Fix any orphan includes; expect clean(er) log without demo errors.

## Step 2: Resolve CUDA/Build Issues for Core Stability
- **Insight**: Persistent ambiguities (e.g., cudaStreamDestroy in qbsa.cpp) from sep::cuda vs. global; incomplete cudaStream_t in gpu_context.h (demos trigger via includes). Namespace fixes + forwards resolve without stubs.
- Qualify CUDA calls in cuda_runtime.h/unified.h with `::` (e.g., `::cudaStreamDestroy`).
- In gpu_context.h/quantum_manifold_optimizer.h, change `sep::cuda::cudaStream_t` to `::cudaStream_t`; include cuda_fwd.h first.
- Remove redundant stubs in cuda_functions.h; strengthen guards (`#pragma once` + conditionals).
- Fix namespace in sep_engine.cpp (e.g., `sep::api::Status::Success`).
- In memory_tier.cpp, declare `cudaError_t err = ::cudaSuccess;` (global qual); fix undef `err`.
- Rebuild quantum/core; test: Run simple pattern add/process in main.cpp, check no crashes.

## Step 3: Stabilize Quantum/Memory Cores for Standalone Use
- **Insight**: Processor (processor.cpp) and coherence_manager ready for patterns; DAG (dag_graph.cpp) can represent quant data (e.g., nodes as price patterns, edges as correlations). Add standalone entry in api_main.cpp.
- Test/Fix: Use code_execution to verify basics (e.g., add mock Pattern, evolve, compute coherence).
- Enhance DAG: Add quant nodes (e.g., volatility/coherence as attributes) in dag_graph.h.
- Standalone Mode: In api_main.cpp, add CLI args for input files; process patterns, output JSON metrics.
- Metrics Output: In coherence_manager.cpp, add funcs for avg coherence/stability/generation; log/JSON export.
- Rebuild; run analyzer—aim for 0 CRITICAL/HIGH defects.

## Step 4: Build Quant Data Pipeline
- **Insight**: No current market data input; use std::filesystem/Crow for file/socket reads. Process as Patterns: Map prices/vol to quantum_state (e.g., energy=price delta, coherence=signal strength).
- Add Input Parser: In core/new `data_parser.cpp`: Read CSV/JSON (e.g., options data: strike/expiry/vol); convert to Patterns (id=ticker, position=vec from time/price).
- Integrate Processor: In engine.cpp, add `processQuantData` : Load data, add to processor, evolve via QBSA/QFH.
- DAG Reps: In memory_tier_manager.cpp, build DAG from processed patterns (nodes=assets, edges=correlations via coherence).
- Metrics: Compute/output (e.g., tail risk from low coherence, alpha from evolution).
- Test: Sample data (browse_page for free options CSV, e.g., from Yahoo Finance); process, verify DAG/metrics.

## Step 5: Implement Standalone Demo and Validation
- **Insight**: Demo: CLI tool (api_main.cpp) takes data file, runs pipeline, outputs JSON (DAG graph, coherence metrics) for trading insights (e.g., "High coherence in AAPL options—low risk").
- Add CLI: Use argc/argv for input file/params; process, print metrics.
- Validation: Unit tests in src/tests (e.g., mock data, assert coherence >0.5 for stable patterns).
- Polish: Error handling (e.g., invalid data); CUDA checks for GPU accel.
- Final Build: Clean ninja; run demo on sample quant data.

Expand as needed: E.g., if Step 2 reveals kernel bugs, sub-task: Debug QBSA in code_execution. Once stable, iterate on quant features (e.g., Black-Scholes comparison metrics).