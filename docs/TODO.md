### Guiding Principles

*   **Triage First:** We'll start by fixing the foundational issues that cause the most build errors, like the `Demo` base class and the `DemoManager`.
*   **Systematic Fixes:** We will methodically fix each demo's C++ files, using the build log as a guide.
*   **Correct Linking:** Once the code compiles, we will ensure the `sep_workbench` executable links against all necessary SEP static libraries and external dependencies.

---

### Phase 1: Foundational Code and Dependency Fixes

This phase addresses the core issues preventing any of the demos from compiling.

#### Task 1: Fix Core Demo Infrastructure

The build log shows that the `Demo` class interface is being used incorrectly by its children and that the `DemoManager` has redefinition errors. Let's fix these first.

*   **Action:**
    1.  In `examples/workbench/demos/demo_manager.hpp`, change the signatures of the `Demo::on_key_press` and `Demo::on_mouse` virtual functions to match the apparent usage in the derived classes. The build log shows a conflict between `on_key_press(int)` and `on_key_press(unsigned char)`. We'll standardize on `int` for keys and add mouse parameters.
    2.  Move the implementations of `DemoManager::initialize` and `DemoManager::registerDemo` from `demo_manager.hpp` to `demo_manager.cpp` to fix the redefinition errors. Leave only the declarations in the header.
    3.  In `examples/workbench/main.cpp`, fix the calls to `manager.update(dt)` and `manager.render()` to `manager.on_update(dt)` and `manager.on_render()` to match the `DemoManager` class definition.

#### Task 2: Fix Dear ImGui Integration

The first error in the build log is a missing `imgui_impl_opengl3_loader.h`. This is a common ImGui setup issue. We will create this file to provide the necessary OpenGL function loading.

*   **Action:**
    1.  Create a new file: `third_party/imgui/backends/imgui_impl_opengl3_loader.h`.
    2.  Populate it with the standard OpenGL loader code required by ImGui's backend. It should include headers for your OpenGL loader (GLEW, in this case) and define the necessary function pointers.

#### Task 3: Fix Widespread Type and API Mismatches

The build log indicates numerous errors related to undeclared types (`Config`), missing class members (`setRotation`, `renderConnection`), and type conflicts (`MemoryTierEnum`). We will perform a pass to fix these by including the correct headers and using proper namespaces.

*   **Action:**
    1.  For every file under `examples/workbench/demos/` that fails to compile due to missing types, add the required headers. For example:
        *   `Config` errors: Add `#include "config.hpp"`.
        *   `QuantumCoherenceManager` errors: Add `#include "memory/quantum_coherence_manager.h"`.
        *   `MemoryTierManager` errors: Add `#include "memory/memory_tier_manager.hpp"`.
        *   `dag::DagGraph` errors: Add `#include "core/dag_graph.h"`.
    2.  Resolve the `MemoryTierEnum` conflict by using the fully qualified name `sep::memory::MemoryTierEnum` where needed.
    3.  In `examples/workbench/demos/genesis_pattern.cpp`, remove the unused parameter warnings in the stubbed `cycles_renderer.hpp` by adding `(void)parameter_name;` inside the function bodies.

---

### Phase 2: Systematically Fix Each Demo

Now, we'll fix the compile errors for each demo, one by one.

#### Task 4: Fix `GenesisPatternDemo`

This demo has a variety of issues, from incomplete types to invalid API calls.

*   **Action:**
    1.  In `genesis_pattern.hpp`, change the `on_key_press(unsigned char key)` signature to `on_key_press(int key)` to match the base `Demo` class.
    2.  In `genesis_pattern.cpp`, fix calls to the `CyclesRenderer`. The build log shows `setRotation`, `setZoom`, etc., are missing. Add these methods to the `CyclesRenderer` class definition in `src/sep_engine_wrapper.h` (or a more appropriate header) and provide stub implementations.
    3.  Fix the call to `renderer_->renderPatternState(pattern_state);`. The argument type is `void*` but the renderer expects `const std::vector<glm::vec3>&`. Cast the `pattern_state` to the correct type.

#### Task 5: Fix `AudioVisualizerDemo`, `FlockingDemo`, and `NeuroSimDemo`

These demos have similar issues with incorrect API usage and type mismatches.

*   **Action:**
    1.  In `audio_visualizer.hpp`, `flocking_demo.hpp`, `neuro_sim.hpp`, and all other demo headers, change the `on_key_press` signature to `on_key_press(int key)`.
    2.  In `audio_visualizer.cpp`, correct the API calls to `audio::AudioCapture`. The methods `setCallback`, `init`, and `start` appear to be missing from the class definition in `src/audio/capture.h`. Add them.
    3.  In `flocking_demo.cpp`, fix the GLM type mismatch error (`vec4 += vec3`). Ensure you are adding vectors of the same dimension or correctly convert them.
    4.  In `neuro_sim.cpp`, resolve the undeclared `ConfigManager` and `MemoryTierManager` errors by including `core/manager.h` and `memory/memory_tier_manager.hpp` and using the correct namespaces.

#### Task 6: Resolve Abstract Class Instantiation Errors

The build log shows that several demos are failing to instantiate because they are abstract. This is due to the `on_key_press` pure virtual function not being correctly overridden. Task 1 and the actions above should resolve this, but this is a final check.

*   **Action:**
    1.  Ensure every demo class in `examples/workbench/demos/` has implemented all pure virtual functions from the base `Demo` class with the correct signatures.
    2.  In `examples/workbench/main.cpp`, uncomment the `manager.registerDemo(...)` calls once the derived classes are no longer abstract.

---

### Phase 3: Final Build and Linking

With the C++ source files fixed, the final step is to ensure the workbench executable is linked correctly.

#### Task 7: Update Workbench CMakeLists.txt

The `sep_workbench` executable needs to be linked against all the SEP static libraries.

*   **Action:**
    1.  Open `examples/workbench/CMakeLists.txt`.
    2.  Modify the `target_link_libraries` section to include all SEP static libraries: `sep_api`, `sep_core`, `sep_compat`, `sep_quantum`, `sep_memory`, `sep_blender`, and `sep_audio`.
    3.  Ensure it also links against `glfw`, `OpenGL::GL`, and `glew` for the windowing and rendering.

#### Task 8: Build and Run

Now you can compile and run the final application.

*   **Action:**
    1.  Execute the build from your build directory:
        ```bash
        cmake ..
        ninja -j$(nproc)
        ```
    2.  Run the workbench application:
        ```bash
        ./examples/workbench/sep_workbench
        ```
