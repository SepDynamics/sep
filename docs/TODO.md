### **High-Level Goal**

Create a standalone executable named `sep_workbench` that opens a window and allows interactively running and switching between all the engine's demos.

---

## **Phase 1: Foundation & Compilation Fixes**

**Objective:** Get a minimal `sep_workbench` executable to compile and link successfully. This phase addresses the errors found in the `build_log.txt`.

*   **Task 1.1: Set up the Workbench Executable**
    *   Create a new directory: `examples/workbench`.
    *   Move the demo source files (`src/demos/*.cpp`, `src/demos/*.hpp`) into `examples/workbench/demos/`.
    *   Create a new `examples/workbench/CMakeLists.txt` file.
    *   Define a new executable target `sep_workbench` in this new `CMakeLists.txt`. This target should compile a new `main.cpp` and all the demo files.
    *   In the root `CMakeLists.txt`, add `add_subdirectory(examples/workbench)`.

*   **Task 1.2: Fix Namespace and Include Errors**
    *   **File:** `examples/workbench/demos/digital_physics_demo.hpp`
        *   **Problem:** `sep::pattern` is not a valid namespace for `PatternData`.
        *   **Action:** Include the correct header `quantum/data.hpp` and change `std::vector<sep::pattern::PatternData> grid_;` to use the correct type. Based on `quantum/data.hpp`, the type is `sep::pattern::PatternData`. The error is likely a missing include. Ensure `#include "quantum/data.hpp"` is present.
    *   **File:** `examples/workbench/demos/memory_garden.hpp`
        *   **Problem:** `MemoryTierManager` and `QuantumCoherenceManager` are not declared.
        *   **Action:** Add the necessary includes:
            ```cpp
            #include "memory/memory_tier_manager.hpp"
            #include "memory/quantum_coherence_manager.h"
            ```
    *   **File:** `examples/workbench/demos/audio_visualizer.hpp`
        *   **Problem:** Incomplete types for `AudioCapture` and `AudioPipeline`.
        *   **Action:** Replace the local forward declarations in the `workbench` namespace with includes for the real engine components:
            ```cpp
            #include "audio/capture.h"
            #include "audio/pipeline.h"
            ```
        *   This will also resolve the `std::unique_ptr` assignment errors, as the types will now match.

*   **Task 1.3: Fix GLM Experimental Extension Errors**
    *   **Problem:** Demos like `cosmo_sim.cpp` use GLM features that require an explicit opt-in.
    *   **Action:** In `examples/workbench/CMakeLists.txt`, add a compile definition for the `sep_workbench` target:
        ```cmake
        target_compile_definitions(sep_workbench PRIVATE GLM_ENABLE_EXPERIMENTAL)
        ```

*   **Task 1.4: Resolve Ambiguous Type `MemoryTierEnum`**
    *   **Problem:** `MemoryTierEnum` is defined or used in multiple headers, causing ambiguity.
    *   **Action:** Throughout the codebase (especially in `blender` and `memory` headers/sources), change all usages of `MemoryTierEnum` to the fully qualified name `sep::memory::MemoryTierEnum`.

*   **Task 1.5: Fix `DemoManager` Singleton Access**
    *   **Problem:** The `DemoManager` has a private constructor, but `main.cpp` tries to instantiate it directly.
    *   **Action:** Modify `main.cpp` to access the `DemoManager` via its `getInstance()` static method.
        ```cpp
        // Change this:
        // sep::workbench::DemoManager demo_manager;
        // To this:
        auto& demo_manager = sep::workbench::DemoManager::getInstance();
        ```

## **Phase 2: Window and Application Loop**

**Objective:** Create the main application entry point and get a window to open.

*   **Task 2.1: Create the Main Entry Point**
    *   Create `examples/workbench/main.cpp`.
    *   This file will contain the `main()` function.

*   **Task 2.2: Implement Window Creation**
    *   In `examples/workbench/main.cpp`, use the existing `sep::workbench::Window` class (`src/window.h`, `src/window.cpp`) to create and manage the main application window.
    *   Initialize GLFW and GLEW.

*   **Task 2.3: Create the Main Loop**
    *   Implement a standard `while (!window.shouldClose())` loop.
    *   Inside the loop, call `glfwPollEvents()`, clear the screen (`glClear`), and swap buffers (`window.swapBuffers()`).
    *   This will give you a blank window that can be closed.

## **Phase 3: Demo Management Framework**

**Objective:** Implement the core logic to load, run, and switch between demos.

*   **Task 3.1: Refine the `Demo` Base Class**
    *   **File:** `examples/workbench/demos/demo_manager.hpp`
    *   **Action:** Solidify the `Demo` abstract base class. It should have the following pure virtual methods:
        ```cpp
        class Demo {
        public:
            virtual ~Demo() = default;
            virtual void on_load() = 0;      // Called when the demo becomes active
            virtual void on_unload() = 0;    // Called when the demo is switched out
            virtual void on_update(float dt) = 0;
            virtual void on_render() = 0;
            virtual void on_key_press(int key) = 0; // For keyboard input
        };
        ```

*   **Task 3.2: Implement the `DemoManager`**
    *   **File:** `examples/workbench/demos/demo_manager.cpp`
    *   **Action:** Implement the logic for `registerDemo`, `switchToDemo`, and `update`/`render` methods. `switchToDemo` should call `on_unload()` on the old demo and `on_load()` on the new one. The main loop will call `on_update` and `on_render` on the currently active demo.

*   **Task 3.3: Register Demos**
    *   **File:** `examples/workbench/main.cpp`
    *   **Action:** After initializing the `DemoManager`, call `registerDemo` for each demo you want to include. Use a unique string key (e.g., "genesis_pattern") and a lambda that creates an instance of the demo class.

## **Phase 4: UI Integration (Dear ImGui)**

**Objective:** Add a UI overlay for controls and metrics.

*   **Task 4.1: Add Dear ImGui Dependency**
    *   Add the Dear ImGui source files to your `third_party` directory.
    *   Update `examples/workbench/CMakeLists.txt` to compile and link the ImGui sources and its GLFW/OpenGL3 backends.

*   **Task 4.2: Integrate ImGui into the Main Loop**
    *   In `main.cpp`, initialize ImGui after the GLFW window is created.
    *   In the main loop, call `ImGui_ImplOpenGL3_NewFrame()`, `ImGui_ImplGlfw_NewFrame()`, and `ImGui::NewFrame()` at the beginning.
    *   After rendering the demo, call `ImGui::Render()` and `ImGui_ImplOpenGL3_RenderDrawData()`.

*   **Task 4.3: Create the Main UI Panel**
    *   In the main loop, create an ImGui window (`ImGui::Begin()`).
    *   Inside this window, add buttons for each registered demo. When a button is clicked, call `DemoManager::getInstance().switchToDemo("demo_name")`.
    *   Add a section to display real-time metrics (FPS, memory usage, etc.).

## **Phase 5: Port and Implement Demos**

**Objective:** Adapt each existing demo to the new framework. For each demo:

*   **Task 5.1: Create Demo Class**
    *   Create a new class (e.g., `GenesisPatternDemo`) that inherits from `sep::workbench::Demo`.
    *   Move the logic from the old `demos/*.cpp` file into the new class's methods (`on_load`, `on_update`, `on_render`, etc.).

*   **Task 5.2: The "Genesis Pattern" Demo**
    *   **Action:** Implement `GenesisPatternDemo`.
    *   `on_load()`: Initialize the patterns and quantum state.
    *   `on_update(dt)`: Run the evolution logic.
    *   `on_render()`: Get the pattern data and pass it to the main `Renderer` to be drawn.
    *   Use ImGui to add sliders for `evolution_rate` and toggles for `auto_evolve`.

*   **Task 5.3: The "Audio-Visual Synthesizer" Demo**
    *   **Action:** Implement `AudioVisualizerDemo`.
    *   `on_load()`: Initialize the `sep::audio::AudioCapture` and `sep::audio::AudioPipeline` components.
    *   `on_update(dt)`: Process the audio data from the pipeline into visual patterns.
    *   `on_render()`: Render the visual patterns.

*   **Task 5.4: The "Memory Garden" Demo**
    *   **Action:** Implement `MemoryGardenDemo`.
    *   `on_load()`: Create initial patterns in different memory tiers (STM, MTM, LTM).
    *   `on_render()`: Visualize the three tiers as distinct spatial regions. Render patterns within their respective regions. Draw lines between related patterns.

*   **Task 5.5: Port Remaining Demos**
    *   **Action:** Repeat the process for all other demos (`Annealing`, `Flocking`, `Neural`, etc.), creating a dedicated class for each and registering it with the `DemoManager`.