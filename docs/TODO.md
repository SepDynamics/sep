Alright, let's cut the bullshit and get this thing working. You've got a solid foundation laid out in `SEP_WORKBENCH_CHECKLIST.md` and a repository full of code that's *almost* there. The `build_log.txt` shows exactly what's breaking. We're not throwing anything out. We're fixing the build errors, then plumbing the demos into the workbench executable.

Here is the plan. Follow these tasks in order.

---

## Task 1: Fix the Build-Stopping Interface Mismatches

**Goal:** Get the project to compile successfully by fixing the C++ class interface errors shown in the build log.

The build is failing because your `Demo` classes (e.g., `AnnealingDemo`, `CosmoDemo`) do not correctly implement the virtual functions defined in the `Demo` base class (`src/workbench_demo_adapter.hpp`).

### Action Steps:

1.  **Modify the `Demo` base class in `src/workbench_demo_adapter.hpp`:**
    The `on_load` function needs to be a pure virtual function so the compiler enforces its implementation in all demo classes. Change its signature to:
    ```cpp
    // In src/workbench_demo_adapter.hpp, inside class Demo
    virtual void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) = 0;
    ```

2.  **Update ALL Demo Classes to Match the `Demo` Interface:**
    For **each** demo header file in `src/workbench/demos/` (e.g., `annealing_demo.hpp`, `cosmo_demo.hpp`, etc.), you must:

    *   **Fix the `on_load` signature:** Change `void on_load() override;` to match the base class.
        ```cpp
        // Example for a demo's .hpp file
        void on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) override;
        ```
    *   **Fix the `on_load` implementation:** In the corresponding `.cpp` file, update the function definition.
        ```cpp
        // Example for a demo's .cpp file
        void AnnealingDemo::on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) {
            engine_ = engine;       // Store the engine pointer
            renderer_ = renderer;   // Store the renderer pointer
            // ... rest of your existing on_load code ...
        }
        ```
    *   **Implement the missing `on_ui_render` function:** The build fails because `on_ui_render` is a pure virtual function that isn't implemented. Add a stub implementation to each demo's `.cpp` file to satisfy the compiler. We will fill this in later.
        ```cpp
        // Add this to each demo's .cpp file
        void AnnealingDemo::on_ui_render() {
            // Intentionally empty for now.
        }
        ```
    *   **Implement the missing `on_mouse` function:** Several demos might be missing this. Add a stub implementation to each demo that needs it.
        ```cpp
        // Add this to each demo's .cpp file if it's missing
        void AnnealingDemo::on_mouse(int x, int y, int button) {
            // Unused in this demo
            (void)x; (void)y; (void)button;
        }
        ```

    > **Execute:** Make these changes for every demo class that is causing a build error: `AnnealingDemo`, `AnnealingSimDemo`, `CosmoDemo`, `AudioVisualizerDemo`, etc. After this, run `make -j$(nproc)` again. The build should now pass.

---

## Task 2: Implement the Demo Management and UI

**Goal:** Get the demos to be selectable and have their specific UI controls appear in the workbench window.

### Action Steps:

1.  **Refine `workbench_main.cpp`:** This is your application's entry point. It needs to drive the whole system.
    *   **Remove Fallback Rendering:** Delete all the raw OpenGL code (the red square, the "START" text drawn with lines). We're past that.
    *   **Initialize `DemoManager`:** Make sure the `DemoManager` is initialized and you call `registerDemos()` to load all the demo factories.
    *   **Implement the Main Loop:** The loop should look like this:
        ```cpp
        while (!window->shouldClose()) {
            glfwPollEvents();

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Update and Render the current demo
            float dt = ...; // Calculate delta time
            manager.on_update(dt);
            manager.on_render();

            // Render the main UI window for selecting demos
            ImGui::Begin("SEP Workbench");
            // ... (add buttons for each demo) ...
            if (ImGui::Button("Genesis Pattern")) { manager.switchToDemo("genesis"); }
            if (ImGui::Button("Neural Network")) { manager.switchToDemo("neural"); }
            // ... etc for all demos
            ImGui::End();

            // Render the UI for the currently active demo
            manager.on_ui_render();

            // Render ImGui draw data
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
        ```

2.  **Flesh out the Demo-Specific UI:**
    *   For each demo, go to its `.cpp` file and fill in the `on_ui_render()` method you added in Task 1.
    *   Use the `SEP_WORKBENCH_CHECKLIST.md` as your guide. For example, in `genesis_pattern.cpp`:
        ```cpp
        // In genesis_pattern.cpp
        void GenesisPatternDemo::on_ui_render() {
            ImGui::Begin("Genesis Pattern Controls");
            ImGui::Checkbox("Auto Evolve", &auto_evolve_);
            ImGui::SliderFloat("Evolution Rate", &evolution_rate_, 0.01f, 1.0f);
            ImGui::SliderFloat("Coherence Threshold", &coherence_threshold_, 0.1f, 0.9f);
            // ... etc ...
            ImGui::End();
        }
        ```
    *   Do this for each demo, adding the specific sliders, buttons, and toggles outlined in the checklist.

---

## Task 3: Final Integration and Cleanup

**Goal:** Ensure data flows correctly from the demos to the renderer and the application runs smoothly.

### Action Steps:

1.  **Verify Data Flow:**
    *   Confirm that the `on_render()` method in each demo calls `renderer_->renderPatternState(...)`.
    *   The `CyclesRendererAdapter` you have in `cycles_renderer_adapter.h` correctly translates the call to the `Renderer::render` method. No changes should be needed there, but be aware of this connection point if rendering is blank.

2.  **Input Handling:**
    *   The `key_callback` in `workbench_main.cpp` needs to call `manager.on_key(key);`.
    *   The `DemoManager::on_key` method should in turn call `current_demo_->on_key_press(key)`. This ensures keyboard input is routed to the active demo.

3.  **Final Cleanup of `workbench_main.cpp`:**
    *   Once the UI and demos are rendering correctly, remove any remaining "emergency" or debug code from the main loop. The loop should be clean and focused on updating the demo manager and rendering ImGui.

---
