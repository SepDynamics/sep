## **Phase 1: Fix the Goddamn Build**

The build is failing because of `-Werror`, which treats every warning as a fatal error. We're not disabling it. We're fixing the code so it compiles cleanly. The errors are all in the `src/workbench/demos/` directory.

### **Task 1.1: Fix Undeclared Member Variables in Demos**

Multiple demo `.cpp` files use member variables for UI controls that are never declared in their corresponding header files. Add the missing private member variables to the class definitions in the `.hpp` files.

**1. File: `src/workbench/demos/annealing_sim.hpp`**
Add these private members to the `AnnealingSimDemo` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

**2. File: `src/workbench/demos/audio_visualizer_simple.hpp`**
Add these private members to the `AudioVisualizerDemo` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

**3. File: `src/workbench/demos/cosmo_sim.hpp`**
Add these private members to the `CosmoSim` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

**4. File: `src/workbench/demos/digital_physics_demo.hpp`**
Add these private members to the `DigitalPhysicsDemo` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

**5. File: `src/workbench/demos/drug_discovery_demo.hpp`**
Add these private members to the `DrugDiscoveryDemo` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

**6. File: `src/workbench/demos/drug_optimizer.hpp`**
Add these private members to the `DrugOptimizerDemo` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

**7. File: `src/workbench/demos/memory_garden.hpp`**
Add these private members to the `MemoryGardenDemo` class:
```cpp
private:
    // ... existing members ...
    float threshold_{1.0f};
    float decay_{0.1f};
    float input_strength_{0.5f};
    float connection_opacity_{0.5f}; // Fix the typo from connection_prob_
```
*Note: In `memory_garden.cpp`, the variable used is `connection_prob_`, but the class member seems intended to be `connection_opacity_`. I've used `connection_opacity_`. Fix the `.cpp` file to use `connection_opacity_` to match the checklist's intent.*

**8. File: `src/workbench/demos/neural_demo.hpp`**
Add these private members to the `NeuralDemo` class:
```cpp
private:
    // ... existing members ...
    float input_strength_{0.5f};
    float learning_rate_{0.05f};
    float connection_prob_{0.2f};
```

### **Task 1.2: Fix Unused Parameter Warnings**

The compiler is flagging unused `engine` and `renderer` pointers in the `on_load` methods. Silence these warnings by explicitly marking the parameters as unused.

Modify the `on_load` function in each of the following files:
- `src/workbench/demos/annealing_sim.cpp`
- `src/workbench/demos/audio_visualizer_simple.cpp`
- `src/workbench/demos/cosmo_sim.cpp`
- `src/workbench/demos/digital_physics_demo.cpp`
- `src/workbench/demos/drug_discovery_demo.cpp`
- `src/workbench/demos/drug_optimizer.cpp`
- `src/workbench/demos/flocking_demo.cpp`
- `src/workbench/demos/memory_garden.cpp`
- `src/workbench/demos/neural_demo.cpp`
- `src/workbench/demos/neuro_sim.cpp`

**Example Change (apply to all listed files):**

**Original:**
```cpp
void SomeDemo::on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) {
    // ...
}
```

**Corrected:**
```cpp
void SomeDemo::on_load(sep::Engine* engine, sep::CyclesRenderer* renderer) {
    (void)engine;   // Mark as unused
    (void)renderer; // Mark as unused
    // ... rest of the function
}
```
*Note: For the demos that actually use `engine` or `renderer` (like `GenesisPatternDemo`), you don't need to add this. Only add it for the ones failing in the build log.*

---

## **Phase 2: Wire Up the Workbench Application**

With the build errors fixed, we need to ensure `workbench_main.cpp` correctly initializes the system and runs the demo loop. The checklist is your guide.

### **Task 2.1: Consolidate the Main Entry Point**

Your `src/` directory contains `main.cpp` and `workbench_main.cpp`. The build log shows `workbench_main.cpp` is being used for the `sep_workbench` target. **Delete `src/main.cpp`** to avoid confusion. `src/workbench_main.cpp` is now the single entry point.

### **Task 2.2: Structure the Main Loop**

Review `src/workbench_main.cpp`. It's a mess of fallback code. Gut the main `while` loop and replace it with a clean, focused loop that follows the checklist's intent.

**Replace the entire `while (!glfwWindowShouldClose(window))` loop in `src/workbench_main.cpp` with this:**

```cpp
// 9. Main Loop
float lastFrameTime = static_cast<float>(glfwGetTime());

while (!glfwWindowShouldClose(window)) {
    // Poll and handle events
    glfwPollEvents();

    // Calculate delta time
    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Clear the screen
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- Main UI and Demo Controls ---
    ImGui::Begin("SEP Workbench Controls");
    ImGui::Text("Current Demo: %s", manager.getCurrentDemo().c_str());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    
    // Demo selection buttons
    if (ImGui::Button("Genesis Pattern (1)")) manager.switchToDemo("genesis");
    if (ImGui::Button("Audio Visualizer (2)")) manager.switchToDemo("audio");
    if (ImGui::Button("Memory Garden (3)")) manager.switchToDemo("memory");
    if (ImGui::Button("Flocking Sim (4)")) manager.switchToDemo("flocking");
    if (ImGui::Button("Cosmology Sim (5)")) manager.switchToDemo("cosmo_sim");
    if (ImGui::Button("Neural Network (6)")) manager.switchToDemo("neural");
    // Add buttons for other demos as needed...
    
    ImGui::End();

    // --- Update and Render Current Demo ---
    if (manager.getCurrentDemo() != "") {
        manager.on_update(deltaTime);
        manager.on_render();      // This calls the demo's on_render()
        manager.on_ui_render();   // This calls the demo's ImGui code
    }

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap buffers
    glfwSwapBuffers(window);
}
```

### **Task 2.3: Verify Demo Registration**

Make sure that `src/workbench_main.cpp` calls the function to register the demos before the main loop starts. Add this line after the demo manager is initialized:

```cpp
// In main() of workbench_main.cpp
auto& manager = sep::workbench::DemoManager::getInstance();
manager.initialize(engine.get(), cycles_adapter.get());

// Add this call:
sep::workbench::registerDemos(); 

manager.switchToDemo("genesis"); // Start with a default demo
```

The function `registerDemos()` is defined in `src/workbench/demos/register_demos.cpp`. This will populate the `DemoManager` with all the demo classes.

---

## **Phase 3: Compile, Run, and Verify**

Now, put it all together.

### **Task 3.1: Build the Executable**

Navigate to your build directory and run the build command. It should now compile without errors.

```bash
cd cmake-make
cmake .. -DSEP_BUILD_WORKBENCH=ON
make -j$(nproc)
```

The executable will be `cmake-make/sep_workbench`.

### **Task 3.2: Run the Workbench**

Execute it from the build directory.

```bash
./sep_workbench
```

**Expected Outcome:**
1.  A window titled "SEP Workbench" appears.
2.  The window has a dark background.
3.  An ImGui window titled "SEP Workbench Controls" is visible.
4.  The "Genesis Pattern" demo should be running by default, rendering something in the window.
5.  Clicking the other buttons in the ImGui window should switch to the corresponding demos, and they should each render their respective visualizations.
6.  Each demo's specific ImGui control window should appear when it is active.
