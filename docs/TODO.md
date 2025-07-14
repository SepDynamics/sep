# Workbench ImGui Segfault Fix TODO

## Background
Segmentation fault during ImGui initialization in workbench application, caused by:
- Runtime crash in ImGuiIO constructor (stack overflow)
- Failed Wayland decoration plugin initialization
- Invalid OpenGL/GLFW context state

## Action Items

### High Priority
1. [ ] Test XWayland Backend Solution
   - Run: `export GLFW_USE_WAYLAND=0 && ./src/sep_workbench`
   - If successful, add to ~/.bashrc permanently
   - **Files**: N/A (environment change)
   - **Impact**: Quickest fix, bypasses Wayland issues

2. [ ] Add Explicit GL Context Assignment
   - Add `glfwMakeContextCurrent(window);` before ImGui::CreateContext
   - **Files**: `/sep/src/workbench/core/workbench_core.cpp`
   - **Location**: In initializeImGui() after glfwCreateWindow
   - **Impact**: Ensures valid GL context for ImGui initialization

3. [ ] Implement Decoration Fallback
   - Add `glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);`
   - **Files**: `/sep/src/workbench/workbench_main.cpp`
   - **Location**: Before window creation
   - **Impact**: Alternative fix if other solutions fail

### Debug/Verification
4. [ ] Verify Session Type
   - Run: `echo $XDG_SESSION_TYPE`
   - Document result for future reference
   - **Impact**: Confirms if Wayland is the root cause

5. [ ] Collect Extended Debug Info (if needed)
   ```bash
   gdb ./src/sep_workbench
   run
   bt full  # on crash
   ```

## Technical Notes

### Affected Files
- `/sep/third_party/imgui/imgui.cpp`: Stack overflow in ImGuiIO constructor
- `/sep/src/workbench/core/workbench_core.cpp`: Crash in initializeImGui()

### Root Cause
- GLFW's Wayland backend fails to load GTK decoration plugin
- Window creates successfully but GL context is invalid
- Cascades into ImGui initialization failure

### References
- GLFW Issue #2290: Wayland initialization segfault
- ImGui Issue #8205: CreateContext crashes with invalid backend state
- Stack Overflow (2018): Similar ImGui + GLAD segfault resolution
- Reddit r/cpp_questions (2022): Context current fix confirmation

### Not Required
- CMake/GTK linking changes (confirmed build issue not the cause)
- FindGTK.cmake modifications