~ : Edit `CMakeLists.txt` at repository root.
~ : After `set(CMAKE_CXX_STANDARD_REQUIRED ON)`, insert `set(CMAKE_CXX_EXTENSIONS OFF)`.
~ : Verify no subdirectory overrides `CMAKE_CXX_STANDARD`.
~ : Configure with CMake to ensure all targets use C++17 without GNU extensions.
~ : Update `include/core/common.h` inside the `PinState` struct:   * Add members `std::uint32_t pin_id{0};`, `float value{0.f};`, and `float coherence{1.f};`.
   * Extend `operator==` to compare these new fields.
~ : Adjust any construction of `PinState` in tests (`tests/core/types_test.cpp` and `tests/core/engine_test.cpp`) to set the new members.
~ : Replace direct assignments to `qbsa_result.corrections`, `qsh_result.collapse_indices`, and `qsh_result.collapse_counts` with `assign()` calls:   * `qbsa_result.corrections.assign(impl_->d_corrections_.begin(), impl_->d_corrections_.end());`
   * For `collapse_indices`, clear and `emplace_back` or reconstruct as described in TASK.md.
   * `qsh_result.collapse_counts.assign(impl_->d_collapse_counts_.begin(), impl_->d_collapse_counts_.end());`
~ : Edit `src/compat/cuda_api.cu`:   * Delete the `#if !defined(__CUDACC__)` / `#endif` pair surrounding the global-state functions (`sep_cuda_init`, `sep_cuda_cleanup`, `sep_cuda_process_batch`, `sep_cuda_process_symmetry`).
~ : Ensure the remaining code is compiled both for host and device builds.
~ : Recompile CUDA components to verify no missing-symbol errors.~ : After updating `PinState`, refine `Engine::generate_probes` in `src/core/engine.cpp`:   * Calculate `probe_idx` using `pin_state.pin_id` and `tick`.
   * Compute `expected` from `pin_state.value` and `pin_state.coherence`.
~ : Ensure device buffers are sized based on `inputs.size()`.
~ : Run `core` unit tests to validate probe generation with the new fields.~ : In `include/api/server.h`, add private members\
   `std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor_;`\
   `std::unique_ptr<sep::blender::ccl::CyclesRenderer> cycles_renderer_;`.
~ : Initialize them in `src/api/server.cpp` constructor.
~ : Modify `setupBlenderRoutes()` to use these members instead of constructing new objects per request.
~ : Replace it with a message when Cycles support is absent:   ```cpp
   #else
       spdlog::critical("Cycles rendering requested, but SEP Engine was built without Cycles support.");
       curl_global_cleanup();
       sep::logging::shutdownLogging();
       return 1;
   #endif
   ```
~ : Ensure no other code references the removed block.