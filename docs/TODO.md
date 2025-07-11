Right, let's cut the bullshit and fix this. The build is failing at the link stage, which means the components are compiling but not connecting correctly. The goal is to make `sep_workbench` link and run the demos you've already defined.

Here is the plan. We're not inventing new shit. We're fixing the broken build and connecting the existing parts.

### Phase 1: Fix the Goddamn Build

The linker is screaming about undefined references. This is happening for three main reasons: missing libraries, incorrect CUDA symbol linking, and transitive dependencies from the Blender/Cycles library not being linked. We'll fix them one by one.

#### Task 1.1: Link the Audio Library

The `audio_visualizer.cpp` demo can't find `sep::audio::*` functions because `sep_workbench` isn't linked against `libsep_audio.a`.

**Action:**
1.  Open `src/CMakeLists.txt`.
2.  Locate the `target_link_libraries(sep_workbench PRIVATE ...)` block.
3.  Add `sep_audio` and its dependencies (`PIPEWIRE_LIBRARIES`, `FFTW_LIBRARIES`) to the list.

```cmake
# In src/CMakeLists.txt

# ... inside the if(SEP_BUILD_WORKBENCH) block ...
target_link_libraries(sep_workbench PRIVATE
    sep_workbench_lib
    sep_api
    sep_quantum
    sep_memory
    sep_core
    sep_compat
    sep_audio  # <-- ADD THIS
    imgui
    ${GLEW_LIBRARIES}
    glfw
    OpenGL::GL
    Threads::Threads
    ${CMAKE_DL_LIBS}
    ${PIPEWIRE_LIBRARIES} # <-- ADD THIS
    ${FFTW_LIBRARIES}     # <-- ADD THIS
)
```

#### Task 1.2: Fix CUDA Symbol Linking

The `sep_compat` static library isn't correctly packaging the CUDA object files (`.cu.o`) with the C++ object files (`.cpp.o`). The C++ code in `raii.cpp` calls CUDA functions defined in `.cu` files, but the linker can't find them in the final `libsep_compat.a` archive.

We'll fix this by defining the CUDA sources as an `OBJECT` library and explicitly adding them to the final static library. This ensures correct dependency ordering.

**Action:**
1.  Open `src/compat/CMakeLists.txt`.
2.  Replace the existing `add_library(sep_compat STATIC ...)` with the following block. This creates an object library for CUDA sources and adds them to the main `sep_compat` target.

```cmake
# In src/compat/CMakeLists.txt

# Define CUDA sources
set(CUDA_SOURCES
    pattern_kernels.cu
    cuda_api.cu
    event.cu
    utils.cu
    kernels.cu
    core.cu
    quantum_kernels.cu
)

# Define C++ sources
set(CPP_SOURCES
    stream.cpp
    raii.cpp
    hip_compat.cpp
)

# Create an OBJECT library for the CUDA sources
add_library(sep_compat_cuda OBJECT ${CUDA_SOURCES})
set_target_properties(sep_compat_cuda PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    POSITION_INDEPENDENT_CODE ON
    CUDA_STANDARD 17
    CUDA_ARCHITECTURES "75;70;61;60"
)
target_compile_options(sep_compat_cuda PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:${CUDA_NVCC_FLAGS}>)

# Create the final static library from the C++ sources and the CUDA object library
add_library(sep_compat STATIC
    ${CPP_SOURCES}
    $<TARGET_OBJECTS:sep_compat_cuda>
)

# Ensure CUDA symbols are exported correctly
set_target_properties(sep_compat PROPERTIES
    CUDA_VISIBILITY_PRESET "default"
    C_VISIBILITY_PRESET "default"
    CXX_VISIBILITY_PRESET "default"
    VISIBILITY_INLINES_HIDDEN OFF
)

# ... (the rest of the file remains the same: target_link_libraries, target_include_directories, etc.)
```

#### Task 1.3: Link Transitive Dependencies from Cycles/Blender

The `libsep_blender.so` shared library has its own dependencies (Cycles, OSL, TBB, etc.). The linker command for `sep_workbench` must also include these.

**Action:**
1.  Open `src/CMakeLists.txt`.
2.  Add all the missing libraries from the `libsep_blender.so` link step to the `target_link_libraries` for `sep_workbench`.

```cmake
# In src/CMakeLists.txt

# ... inside the if(SEP_BUILD_WORKBENCH) block ...
target_link_libraries(sep_workbench PRIVATE
    sep_workbench_lib
    sep_api
    sep_quantum
    sep_memory
    sep_core
    sep_compat
    sep_audio
    libsep_blender.so # <-- Make sure this is linked
    imgui
    ${GLEW_LIBRARIES}
    glfw
    OpenGL::GL
    Threads::Threads
    ${CMAKE_DL_LIBS}
    ${PIPEWIRE_LIBRARIES}
    ${FFTW_LIBRARIES}

    # ADD TRANSITIVE DEPENDENCIES FOR BLENDER/CYCLES
    cycles_device
    cycles_kernel
    cycles_scene
    cycles_util
    ${OSL_LIBRARIES}
    ${TBB_LIBRARIES}
    ${OpenImageIO_LIBRARIES}
    ${Alembic_LIBRARIES}
    ${OpenPGL_LIBRARIES}
    ${CURL_LIBRARIES} # From API
    -lhiredis # From memory
)
```

After these changes, run the build again from a clean directory. This should resolve the linker errors.

### Phase 2: Get the Demos Running

With the executable linking, we can connect the pieces as intended by your `SEP_WORKBENCH_CHECKLIST.md`. The goal is to get the demo selection UI working and have the selected demo render in the window.

#### Task 2.1: Verify Main Application Logic

The `src/workbench_main.cpp` file contains the main loop. It needs to correctly initialize the `DemoManager`, register demos, and then call the manager's update/render functions in the main loop.

**Action:**
1.  Create a file `src/workbench/demos/register_demos.cpp` (if it doesn't exist, based on the snapshot it does) and ensure it has the `registerDemos()` function which calls `DemoManager::getInstance().registerDemo(...)` for each demo. The provided `register_demos.cpp` looks correct.
2.  In `src/workbench_main.cpp`, make sure you call `sep::workbench::registerDemos()` after the demo manager is initialized.
3.  Ensure the main loop in `workbench_main.cpp` calls `demoManager.on_update(deltaTime)` and `demoManager.on_render()`. The provided file seems to do this correctly.

#### Task 2.2: Implement the UI for Demo Selection

The `workbench_main.cpp` already has a basic ImGui window. We need to make sure the buttons correctly call `demoManager.switchToDemo("demo_name")`.

**Action:**
1.  Review the ImGui section in `src/workbench_main.cpp`.
2.  Verify each `ImGui::Button` call is mapped to the correct demo name string used in `registerDemos()`. For example: `if (ImGui::Button("Genesis Pattern")) manager.switchToDemo("genesis");`
3.  The provided `workbench_main.cpp` seems to have this implemented correctly. No changes are likely needed here unless demo names are inconsistent.

#### Task 2.3: Connect the Renderer to Demos

The `Demo` base class holds a `sep::CyclesRenderer* renderer_`. The `CyclesRendererAdapter` is the bridge between the simple `Renderer` you have and the interface the demos expect.

**Action:**
1.  The `main` function in `workbench_main.cpp` already creates the `Renderer`, the `CyclesRendererAdapter`, and passes the adapter to the `DemoManager`. This setup is correct.
2.  Each demo's `on_render()` method must use `renderer_->renderPatternState(...)` to draw. Review each demo file (e.g., `genesis_pattern.cpp`) to confirm it calls the renderer. The provided files look correct.

#### Task 2.4: Final Polish and Run

At this point, the application should build and run. The final step is to ensure the demos are visually distinct and the controls work as expected.

**Action:**
1.  Build and run `./src/sep_workbench`.
2.  Use the ImGui window to switch between demos.
3.  Confirm that the visuals change for each demo.
4.  Test the keyboard controls defined in each demo's `on_key_press` method (e.g., spacebar, '+/-' in `GenesisPatternDemo`).

This task list is a direct, targeted attack on the build failure and a methodical check of the existing architecture to bring it online. No new files, no new approaches. Just fixing what's there.