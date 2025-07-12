Alright, let's cut the bullshit and get this thing working. The build is failing, and nothing else matters until it links. The core problem is in how the libraries, especially the Blender/Cycles components, are being linked.

Here is a direct, no-nonsense task list. Follow these steps in order. This isn't a new approach; this is a plan to fix the broken one you have.

---

### **Phase 1: Fix the Goddamn Build**

The linker is failing because it can't find the Cycles symbols that `libsep_blender.so` needs. We're going to fix this by getting rid of the shared library hell and linking everything statically into the final executable, as your architecture document intended.

**Task 1: Make `sep_blender` a Static Library**

Your `blender` module is being built as a shared library (`.so`), which is the root of your linking nightmare. Change it to a static library (`.a`) so the main linker can resolve everything at once.

1.  **Modify `src/blender/CMakeLists.txt`:**
    *   Find the line: `add_library(sep_blender SHARED ${BLENDER_SOURCES})`
    *   Change it to: `add_library(sep_blender STATIC ${BLENDER_SOURCES})`
    *   Remove or comment out the `set_target_properties` call that sets `LIBRARY_OUTPUT_DIRECTORY` and `RUNTIME_OUTPUT_DIRECTORY` for `sep_blender`, as static libraries don't have a runtime location.

2.  **Modify `src/CMakeLists.txt`:**
    *   Find the `target_link_libraries` call for the `sep_workbench` executable.
    *   The line that links `lib/libsep_blender.so` is now wrong. It will fail because that file isn't created anymore.
    *   Change the linking of `sep_blender` to be just like your other static libraries (e.g., `sep_core`, `sep_quantum`). It should just be the target name: `sep_blender`.

**Task 2: Clean Up the Final Link Command**

The linker command for `sep_workbench` is a mess and links things multiple times. We will fix this by organizing the dependencies logically.

1.  **Modify `src/CMakeLists.txt`:**
    *   Find the `target_link_libraries` for the `sep_workbench` target.
    *   **Delete the entire existing block and replace it** with the following structured and correctly ordered list. This ensures every library is listed only once and in the correct dependency order (from highest-level to lowest-level).

    ```cmake
    target_link_libraries(sep_workbench PRIVATE
        # --- Workbench & Application Logic ---
        sep_workbench_lib
        imgui

        # --- SEP Static Libraries (High-level to Low-level) ---
        sep_api
        sep_blender
        sep_audio
        sep_quantum
        sep_memory
        sep_embeddings
        sep_core
        sep_compat

        # --- Cycles and its Dependencies ---
        cycles_device
        cycles_kernel
        cycles_scene
        cycles_util
        ${OSL_LIBRARIES}
        ${TBB_LIBRARIES}
        ${OpenImageIO_LIBRARIES}
        ${Alembic_LIBRARIES}
        ${OpenPGL_LIBRARIES}

        # --- System & Core External Dependencies ---
        ${CURL_LIBRARIES}
        hiredis
        http_parser::http_parser
        GLEW::glew
        glfw
        OpenGL::GL
        Threads::Threads
        ${PIPEWIRE_LIBRARIES}
        ${FFTW_LIBRARIES}
        CUDA::cudart
        CUDA::cuda_driver
        ${CMAKE_DL_LIBS}
    )
    ```
    *   **Rationale:** This structure links your workbench code first, then your engine's static libraries from most-dependent (`api`) to least-dependent (`compat`), and finally all the third-party libraries that provide the underlying symbols. This resolves the undefined references.

**Task 3: Resolve the TBB Version Conflict**

The build log shows a conflict between `libtbb.so.2` and `libtbb.so.12`. This happens because you're mixing a system-installed TBB with a custom-built one.

1.  **Modify all relevant `CMakeLists.txt` files:**
    *   Search your entire project for the hardcoded path: `/home/ajn/Documents/AVarietyOShit/BUILTASF/tbb/lib64`.
    *   **Delete it.**
    *   Rely on `find_package(TBB REQUIRED)`. This will find the system's TBB, which is the same one OpenImageIO and other system libraries are using, resolving the conflict.

**At this point, your project should compile and link successfully. If it doesn't, re-examine the link order in Task 1.2.**

---

### **Phase 2: Get Demos Running in the Window**

Now that you have a `sep_workbench` executable, run it. It will likely show a window, but the demos need to be wired up.

**Task 4: Wire Up the Demo Manager and UI**

The `workbench_main.cpp` is the entry point. It needs to correctly initialize and run the demo system.

1.  **Verify Demo Registration:**
    *   Confirm that `src/workbench/demos/register_demos.cpp` is included in the `sep_workbench_lib` target in `src/workbench/CMakeLists.txt`.
    *   In `workbench_main.cpp`, ensure the call `sep::workbench::registerDemos();` is present and happens *after* `DemoManager::initialize()`.

2.  **Connect the Main Loop to the Demo Manager:**
    *   Inside the main `while` loop of `workbench_main.cpp`, ensure these three calls happen every frame:
        ```cpp
        // Update the current demo's logic
        manager.on_update(deltaTime);

        // Render the current demo's visualization
        manager.on_render();

        // Render the current demo's ImGui controls
        manager.on_ui_render();
        ```

3.  **Implement Keyboard Switching:**
    *   The `key_callback` in `workbench_main.cpp` needs to correctly switch demos.
    *   **Action:** Add logic to the `key_callback` to map number keys (1-9) to demo names (`"genesis"`, `"neural"`, etc.) and call `manager.switchToDemo(...)`.

**Task 5: Implement Demo-Specific Rendering**

Each demo needs to draw its state. The `CyclesRendererAdapter` is your bridge to do this.

1.  **Flesh out `on_render()` in Each Demo:**
    *   Open `src/workbench/demos/genesis_pattern.cpp`.
    *   In its `on_render()` method, collect the positions of your patterns into a `std::vector<glm::vec3>`.
    *   Call `renderer_->renderPatternState(points);`.
    *   **Repeat this for every other demo file (`audio_visualizer.cpp`, `memory_garden.cpp`, etc.).** Each demo must translate its internal state into a set of points or other visual data that the renderer can understand.

2.  **Fix the `Renderer` Class:**
    *   Open `src/workbench/renderer.cpp`. This class is a placeholder. It needs to do real OpenGL drawing.
    *   The `render(const std::vector<Pattern>& patterns)` method is your target. Implement it to draw a shape (like a sphere or a cube, as your code stub suggests) at the position of each pattern. Use the pattern's `color` and `scale` properties to change how it looks.
    *   **Get one shape drawing correctly first.** A simple colored point or small cube is fine. Once that works, you can add more complexity.

---

### **Phase 3: Make the Demos Do Something**

With the framework rendering, now you fill in the logic for each demo based on your checklist.

**Task 6: Implement Core Demo Logic**

1.  **Genesis Pattern (`genesis_pattern.cpp`):**
    *   In `on_update()`, implement the "Visual Evolution System". Modify the `coherence` and `stability` of your patterns over time. This is where you connect your quantum algorithms from the `sep_quantum` library.

2.  **Audio-Visual Synthesizer (`audio_visualizer.cpp`):**
    *   In `on_load()`, make sure `AudioCapture` is initialized.
    *   In `on_update()`, get the latest FFT data from the `AudioPipeline` and map frequency bands and amplitudes to pattern properties.

3.  **Memory Garden (`memory_garden.cpp`):**
    *   In `on_update()`, get pattern data from the `MemoryTierManager`.
    *   In `on_render()`, implement the "3D Memory Tier Visualization". Use the pattern's `memory_tier` property to place it in the correct region (e.g., STM in an outer sphere, LTM in a central core). Draw lines between related patterns by querying the `DagGraph`.

Start with Phase 1. Do not proceed until the build is clean. Then, move to Phase 2 to see something in the window. Finally, tackle Phase 3 one demo at a time. This is the path. No more detours.