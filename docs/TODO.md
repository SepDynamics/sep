## Phase 1: Get the Workbench Running

The build should pass now. Next, we get the `sep_workbench` application functional as described in your checklist.

### Task 1: Wire Up the Demo Manager in `workbench_main.cpp`

**Problem:** The checklist describes a demo-centric application. Your `src/workbench/main.cpp` needs to reflect this. It needs to initialize the engine, the renderer, and the `DemoManager`, then run a loop that lets the manager do its job.

**Action:**

1.  **Open `src/workbench/main.cpp`**.
2.  **Verify the `main` function structure.** It should look roughly like this:
    *   Initialize GLFW and create a window.
    *   Initialize GLEW.
    *   Initialize ImGui.
    *   Create your `sep::Engine` instance and `sep::workbench::Renderer`.
    *   Create the `CyclesRendererAdapter` to bridge your simple renderer with the demo interface.
    *   Get the `DemoManager` singleton.
    *   **Call `demoManager.initialize(engine.get(), cycles_adapter.get());`**
    *   **Call `sep::workbench::registerDemos();`** (from `register_demos.cpp`).
    *   **Call `demoManager.switchToDemo("genesis");`** to start with a default demo.
    *   Enter the main loop (`while (!glfwWindowShouldClose(window))`).
3.  **Inside the main loop:**
    *   Poll events (`glfwPollEvents()`).
    *   Start a new ImGui frame.
    *   **Call `demoManager.on_update(dt);`**
    *   **Call `demoManager.on_render();`**
    *   Render the ImGui UI.
    *   Swap buffers (`glfwSwapBuffers(window)`).
4.  **Implement input forwarding.**
    *   Create a `key_callback` function for GLFW.
    *   Inside the callback, call `demoManager.on_key(key);`. The manager will then forward the key press to the *active* demo.
    *   Wire it up: `glfwSetKeyCallback(window, key_callback);`.

### Task 2: Implement the Demo Selection UI

**Problem:** You need a way to switch between demos as per your checklist.

**Action:**

1.  **Open `src/workbench_main.cpp`**.
2.  **Inside the main loop, within the ImGui frame block, create a window for demo selection.**

    ```cpp
    ImGui::Begin("Demo Controls");
    
    auto& manager = sep::workbench::DemoManager::getInstance();

    if (ImGui::Button("Genesis Pattern")) { manager.switchToDemo("genesis"); }
    if (ImGui::Button("Audio Visualizer")) { manager.switchToDemo("audio"); }
    if (ImGui::Button("Memory Garden")) { manager.switchToDemo("memory"); }
    // ... add buttons for all registered demos
    
    ImGui::Text("Current Demo: %s", manager.getCurrentDemo().c_str());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
    ```

### Task 3: Connect the Rendering Pipeline

**Problem:** The demos need to draw something. The `Demo` interface has `on_render()`, and it's given a `CyclesRenderer` pointer. Your `CyclesRendererAdapter` needs to correctly forward these calls to your actual `sep::workbench::Renderer`.

**Action:**

1.  **Open `src/cycles_renderer_adapter.h`**.
2.  **Inspect the `renderPatternState` method.** This is the crucial link. The `Demo` classes will call this.
    ```cpp
    // This is from your snapshot and looks correct.
    void renderPatternState(const std::vector<glm::vec3>& patterns) override {
        if (!renderer_) return;
        
        std::vector<Pattern> workbench_patterns;
        for (const auto& p : patterns) {
            Pattern pattern;
            pattern.values = {p.x, p.y, p.z}; // Example conversion
            pattern.quantum_state.coherence = p.z; // Example conversion
            workbench_patterns.push_back(pattern);
        }
        
        renderer_->render(workbench_patterns); // Call the actual renderer
    }
    ```
3.  **Verify that individual demos use `renderer_->renderPatternState(...)`**.
    *   Open `src/workbench/demos/genesis_pattern.cpp`. In `on_render()`, it should call the renderer. Your snapshot shows it's missing this call. **Fix it.**
    *   **Do this for every demo file.** Each demo's `on_render` method must call the `renderer_` member to draw its state.
