# TODO.md: Refined Roadmap to Standalone SEP Engine Demo for Quant Trading

This refined roadmap addresses the latest analyzer defects (e.g., macro expansions causing syntax errors like 'expected unqualified-id' from cudaSuccess, circular includes in cuda_runtime.h/cuda_types.h, ambiguous CUDA calls in qbsa.cpp/engine.cpp) and build failures (e.g., missing audio/capture.h, noexcept mismatches in math functions fixed via toolchain flags). Key strategy: Use CUDA toolchain exclusively (no 'CUDA' in project languages) to avoid math conflicts; remove all non-core subs (audio/blender/workbench/UI); qualify CUDA symbols globally (::cudaSuccess); break header cycles with cuda_fwd.h; implement minimal quant CSV parser in data_parser.cpp; use api_main.cpp as CLI demo entry for batch processing with JSON output. Expand steps with sub-tasks for fixes (e.g., test CUDA snippets via code_execution). Proceed sequentially: CMake cleanup first, then CUDA stabilization, then pipeline, verifying with ninja/analyzer after each.

## Step 1: Update CMake Configuration and Remove Non-Essential Components
- **Insight**: Build fails on missing audio/capture.h (from engine.cpp) and UI deps; toolchain handles CUDA (flags like -include cuda_unified_fix.h stubs sinpi/cospi noexcept); strip all optional finds/subs (no OpenVDB/Imath/GLM/spdlog—use std alternatives); disable workbench/tests for minimal core/API focus—reduces defects by ~80% (no ImGui mem ops, no Blender scene.h).
- Update main CMakeLists.txt: Load toolchain early; set project LANGUAGES CXX only; add_compile_definitions(SEP_HAS_CUDA=1 SEP_USE_CUDA=1 SEP_MINIMAL=1); find only essentials (CUDAToolkit Threads fmt TBB); include_dirs src/compat/core/quantum/memory/api; add_subdirectory only those.
- Update src/CMakeLists.txt: Add libs for sep_core (error_handler/dag_graph/logging), sep_compat (cuda_impl/raii/stream), sep_memory (memory_tier_manager/quantum_coherence_manager), sep_quantum (qbsa/qfh/evolution/processor/pattern_processor_interface/types_serialization), sep_api (bridge/client/server/sep_engine/api_main); link sep_api to sep_quantum/sep_memory/sep_core/sep_compat/CUDA/TBB/fmt/Threads; no workbench/audio/blender/third_party.
- Delete dirs/files: src/workbench src/audio src/blender third_party/glm third_party/imgui; strip refs in core/engine.h (no demo_manager/CyclesRenderer); replace GLM vec3 with std::array<float,3> in Pattern/positions.
- Rebuild/Verify: ninja clean; ninja; check log for no missing headers/ambiguities; run analyzer—target 0 CRITICAL.

## Step 2: Resolve CUDA Macro and Syntax Issues
- **Insight**: 'expected unqualified-id' from cudaSuccess macro (redefined as 0, conflicts in enums/constexpr); fix by undefining macros in cuda_fwd.h or using enum class; noexcept math issues (sinpi/cospi) resolved by cuda_unified_fix.h defines (__CUDA_NO_HALF_OPERATORS__ etc.)—ensure -include flag.
- Sub-task: In compat/cuda_fwd.h, change #define cudaSuccess 0 to enum class CudaError { Success = 0, MemoryAllocation = 2 }; use CudaError::Success; update all uses (e.g., cuda_impl.cpp ::CudaError::Success).
- Sub-task: Qualify calls in cuda_runtime.h/cuda_impl.cpp with :: (e.g., ::cudaStreamDestroy); remove inline redefs in cuda_wrappers.h causing ambiguities.
- Sub-task: Fix Error constructor in cuda_impl.cpp: Align to 4 args (add location: __FILE__ ":" + std::to_string(__LINE__)).
- Test: Use code_execution for snippet: #include "compat/cuda_fwd.h"; cudaError_t err = CudaError::Success; std::cout << (err == 0);
- Rebuild quantum/core; verify no syntax errors in qbsa.cpp/memory_tier.cpp.

## Step 3: Break Circular Dependencies and Include Cycles
- **Insight**: Cycles like cuda_runtime.h <-> cuda_types.h <-> cuda_unified.h cause undef types (cudaStream_t incomplete in gpu_context.h); LOW defects from misc-header-include-cycle—use cuda_fwd.h for all forwards, ensure #pragma once, one-way includes (fwd -> types -> runtime).
- Sub-task: Move all typedefs/decls (cudaError_t, cudaStream_t, cudaMemcpyKind enum) to cuda_fwd.h; include cuda_fwd.h first in all CUDA headers; remove <cuda_runtime.h> from cuda_types.h.
- Sub-task: In cufft.h, qualify using ::cufftComplex; etc.
- Sub-task: Remove circular refs in engine.h (no audio/capture.h—stub if needed).
- Audit: Grep for #include "compat/cuda_*" and reorder.
- Rebuild; check logs for no cycle warnings; analyzer 0 circular defects.

## Step 4: Fix Analyzer Defects and Remaining Compilation Errors
- **Insight**: CRITICAL in driver_types.h from macro conflicts—undef cudaSuccess before including system CUDA; MEDIUM cert-err33-c ignored returns (add (void)fprintf); HIGH uninit in cetintrin.h (init vars); LOW reserved macros in cuda_unified_fix.h (rename __CUDA_* to SEP_CUDA_*).
- Sub-task: HIGH/CRITICAL first: Init t in cetintrin.h __builtin_ia32_rdsspd(t); cast chars in ImGui (but remove ImGui); correct Status::Success namespace in sep_engine.cpp to sep::api.
- Sub-task: MEDIUM/LOW: Replace memset/memcpy on non-trivial (e.g., ImVector—but remove ImGui); add noexcept to swaps; (void) ignored returns in socket_adaptors.h shutdown.
- Sub-task: In memory_tier.cpp, fix syntax from macro: Use ::cudaSuccess instead of cudaSuccess.
- Sub-task: In stream.cpp/raii.cpp, define SEP_cudaStreamNonBlocking/SEP_cudaStreamDefault as 1/0.
- Sub-task: In engine.h, qualify SEPBlenderBridge as pattern::BlenderBridge (but remove Blender).
- Re-run analyzer; aim for 0 HIGH/CRITICAL; rebuild all modules.

## Step 5: Stabilize Core Runtime and Add Quant Data Parser
- **Insight**: Post-fixes, test headless runtime; no UI/render—use console/JSON logs; add data_parser.cpp for CSV (ticker,price,vol -> Pattern: id=ticker, energy=price delta, coherence=1/vol).
- Implement src/core/data_parser.cpp/h: std::vector<sep::Pattern> parseQuantCSV(const std::string& path); use std::ifstream, std::getline, std::stringstream for split.
- In api_main.cpp, add CLI: if argc>1, parse file argv[1], process patterns, output JSON metrics (coherence/alpha/risk).
- Test: code_execution for parser snippet with mock CSV; run api_main with sample.csv (web_search_with_snippets "free options csv sample").
- Rebuild; run basic pattern process; confirm no crashes.

## Step 6: Implement DAG for Quant Correlations and Metrics Output
- **Insight**: dag_graph.cpp ready—extend addNode with volatility attr; build edges if coherence>0.7 (strong hedge).
- In engine.cpp, add processQuantData: Parse CSV -> add Patterns to processor -> evolve -> build DAG (nodes=assets/edges=correlations) -> compute metrics (alpha=avg coherence * generation, risk=1-stability).
- JSON output: {"dag":{"nodes":[{id,vol,coherence}],"edges":[{from,to,strength}]}, "metrics":{"alpha":0.15,"risk":"low"}}.
- Test: Mock patterns, assert DAG size/metrics; code_execution for compute funcs.

## Step 7: Create Standalone CLI Demo and Final Validation
- **Insight**: Demo: sep_engine --quant data.csv --output metrics.json processes batch, outputs trading insights.
- In api_main.cpp main: Parse args (--quant file --gpu), call processQuantData, dump JSON.
- Validation: Add src/tests/quant_test.cpp (mock CSV, assert coherence>0.5); run analyzer/tests.
- Polish: Error handling (invalid CSV=JSON{"error":"parse failed"}); --gpu flag for CUDA.
- Final: ninja; run demo on sample data (browse_page "yahoo finance options csv sample"); commit if stable.