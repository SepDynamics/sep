Here is the no-bullshit task list to get the SEP Workbench fully operational. The primary blocker is a series of linker errors caused by a misconfigured build system. We will fix that first, then systematically implement the demo functionality as outlined in your checklist.

No new approaches. No new files unless absolutely necessary. We are fixing this build.

---

## **Phase 1: Fix the Linker Errors & Build System**

This phase is critical. The build is failing at the final link stage for the `sep_workbench` executable. The `build_log.txt` shows hundreds of `undefined reference` errors. We will resolve these by correcting the CMake configuration.

### **Task 1.1: Consolidate and Correct Library Linking in `src/CMakeLists.txt`**

The current link command for `sep_workbench` is a mess of duplicated libraries and incorrect dependencies. This is the root cause of the build failure.

*   **File to Edit:** `src/CMakeLists.txt`
*   **Action:** Replace the entire `add_executable(sep_workbench ...)` block with a clean, correct version. The goal is to ensure that all static libraries (`.a`) are linked *before* the shared libraries (`.so`) they depend on, and that all transitive dependencies from Cycles/Blender are properly included.

```cmake
# --- START REPLACEMENT BLOCK for src/CMakeLists.txt ---

if(SEP_BUILD_WORKBENCH)
    add_executable(sep_workbench workbench_main.cpp)
    set_target_properties(sep_workbench PROPERTIES CXX_STANDARD 17)

    target_include_directories(sep_workbench PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/third_party/imgui
        ${CMAKE_SOURCE_DIR}/third_party/imgui/backends
        ${GLEW_INCLUDE_DIRS}
    )

    # Link all libraries in the correct order.
    # Static libraries first, then the shared libraries they need.
    target_link_libraries(sep_workbench PRIVATE
        # --- SEP Static Libraries ---
        sep_workbench_lib
        sep_api
        sep_quantum
        sep_memory
        sep_core
        sep_audio
        sep_embeddings
        imgui

        # --- Blender/Cycles and its Dependencies ---
        # Link the shared library we built
        sep_blender

        # Transitive dependencies required by sep_blender
        cycles_device
        cycles_kernel
        cycles_scene
        cycles_util
        ${OSL_LIBRARIES}
        ${TBB_LIBRARIES}
        ${OpenImageIO_LIBRARIES}
        ${Alembic_LIBRARIES}
        ${OpenPGL_LIBRARIES}

        # --- System & Other Dependencies ---
        sep_compat # Must be linked as a shared lib if it provides CUDA symbols
        ${CURL_LIBRARIES}
        -lhiredis
        http_parser::http_parser
        ${GLEW_LIBRARIES}
        glfw
        OpenGL::GL
        ${CMAKE_DL_LIBS}
        Threads::Threads
        ${PIPEWIRE_LIBRARIES}
        ${FFTW_LIBRARIES}
    )
endif()

# --- END REPLACEMENT BLOCK ---
```

### **Task 1.2: Fix `sep_compat` and `sep_blender` Library Definitions**

The `build_log.txt` shows that `libsep_blender.so` is missing symbols it expects from other libraries, and `sep_compat` has its own issues. This is a dependency definition problem.

1.  **File to Edit:** `src/compat/CMakeLists.txt`
    *   **Problem:** It's being built as a `SHARED` library but doesn't link its dependencies correctly.
    *   **Action:** Change `add_library(sep_compat SHARED ...)` to `add_library(sep_compat STATIC ...)`. The compatibility layer should be a static library that gets compiled into the final executable. This resolves the `undefined reference to sep::cuda::*` errors.

2.  **File to Edit:** `src/blender/CMakeLists.txt`
    *   **Problem:** It's being built as a `SHARED` library (`libsep_blender.so`) but fails to correctly link its own dependencies, causing the main executable link to fail.
    *   **Action:** Ensure the `target_link_libraries` for `sep_blender` is correct and includes all Cycles components. **The fix in Task 1.1 should resolve this by linking everything correctly at the final executable stage, but double-check this file if issues persist.**

### **Task 1.3: Address TBB Version Conflict**

*   **Problem:** The log shows `warning: libtbb.so.2 ... may conflict with libtbb.so.12`. This is because the system's OpenImageIO is built against an older TBB than the one you're likely providing for Cycles.
*   **Action:** This is an environment issue. The cleanest fix is to rebuild the dependencies (like OpenImageIO) against the same version of TBB that Cycles uses. A quicker, dirtier fix is to use an `LD_PRELOAD` trick at runtime, but this is not recommended. **For now, focus on getting it to link. If it crashes at runtime, this is the cause.**

---

## **Phase 2: Implement the Workbench UI and Demo Management**

With the build fixed, we can make the application functional. This aligns with your `SEP_WORKBENCH_CHECKLIST.md`.

### **Task 2.1: Finalize the Demo Manager**

*   **Files to Edit:** `src/workbench/demos/demo_manager.cpp`, `src/workbench_main.cpp`.
*   **Action:**
    1.  In `workbench_main.cpp`, ensure `registerDemos()` is called. It already is.
    2.  Ensure `manager.switchToDemo("genesis")` is called to load the first demo. It already is.
    3.  Implement the key callback (`key_callback` in `workbench_main.cpp`) to call `manager.switchToDemo()` based on the key pressed (1-9). Map keys to the demo names registered in `registerDemos`.

### **Task 2.2: Implement the Main UI**

*   **Files to Edit:** `src/workbench_main.cpp` and `src/workbench/demos/demo_manager.cpp`.
*   **Action:**
    1.  In the main loop of `workbench_main.cpp`, create an ImGui window that lists the available demos and allows switching. This window should call `manager.switchToDemo()`.
    2.  The current active demo's `on_ui_render()` method must be called from the main loop. This is where demo-specific controls will live.

---

## **Phase 3: Activate and Verify Individual Demos**

Work through each demo, connecting its logic to the renderer and UI.

### **Task 3.1: "Genesis Pattern" Demo**

*   **Files to Edit:** `src/workbench/demos/genesis_pattern.cpp`, `src/workbench/demos/genesis_pattern.hpp`.
*   **Action:**
    1.  In `on_load`, initialize the pattern processor and create the seed pattern.
    2.  In `on_update`, call the evolution logic.
    3.  In `on_render`, get the pattern data and pass it to `renderer_->renderPatternState()`. The renderer adapter should handle the visualization.
    4.  In `on_ui_render`, implement the ImGui controls (sliders, toggles) defined in `SEP_WORKBENCH_CHECKLIST.md`.

### **Task 3.2: "Audio-Visual Synthesizer" Demo**

*   **Files to Edit:** `src/workbench/demos/audio_visualizer.cpp`, `src/workbench/demos/audio_visualizer.hpp`.
*   **Action:**
    1.  In `on_load`, initialize the `sep::audio::AudioCapture` and `AudioPipeline`.
    2.  In the audio callback, convert the FFT data into pattern vectors for the engine.
    3.  In `on_render`, map the spectral data to visual properties (color, etc.) and render the patterns.
    4.  In `on_ui_render`, add controls for audio input and sensitivity.

### **Task 3.3: "Memory Garden" Demo**

*   **Files to Edit:** `src/workbench/demos/memory_garden.cpp`, `src/workbench/demos/memory_garden.hpp`.
*   **Action:**
    1.  In `on_update`, query the `MemoryTierManager` for patterns in STM, MTM, and LTM.
    2.  In `on_render`, implement the 3D visualization. Map each pattern's position based on its tier (e.g., STM in an outer sphere, LTM in a central core).
    3.  Render lines between related patterns by querying the DAG.
    4.  In `on_ui_render`, add UI elements to inspect pattern details and view memory stats.

---

## **Phase 4: Static Analysis Cleanup**

The build is working, demos are running. Now, clean up the code rot.

### **Task 4.1: Address High-Priority Defects**

*   **File to Review:** `report.md`
*   **Action:**
    1.  Focus on **HIGH** and **MEDIUM** severity issues within your own source files (e.g., in `src/workbench/renderer.cpp`, `src/core/engine.cpp`).
    2.  Ignore defects in `third_party/` and system headers unless they are causing demonstrable problems. Many are false positives or stylistic issues.
    3.  Fix the `bugprone-integer-division` issues in `renderer.cpp` by ensuring floating-point division (e.g., `width / 2.0f`).
    4.  Fix any legitimate `[cert-err33-c]` warnings by checking the return values of functions like `snprintf`.

Execute this plan sequentially. Phase 1 is the gatekeeper for all other progress. Once the linker is satisfied, the rest is implementation.
