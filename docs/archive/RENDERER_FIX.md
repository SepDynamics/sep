# CyclesRenderer Fix Guide

## Issue Summary
The workbench build is failing because code references `sep::CyclesRenderer` which doesn't exist. This is blocking all visualization demos.

## Quick Fix (5 minutes)

### Option 1: Type Alias (Recommended)
Create a new file `src/workbench/cycles_compat.h`:

```cpp
#pragma once
#include "renderer.h"

namespace sep {
    // Temporary alias until Cycles integration is complete
    using CyclesRenderer = workbench::Renderer;
}
```

Then add to all files that use CyclesRenderer:
```cpp
#include "workbench/cycles_compat.h"
```

### Option 2: Global Search & Replace
Replace all occurrences:
- Find: `sep::CyclesRenderer`
- Replace: `sep::workbench::Renderer`

Files affected (46 locations):
- All files in `src/workbench/demos/`
- `src/workbench/core/demo_orchestrator.*`
- `src/workbench/core/workbench_core.cpp`

### Option 3: Stub Implementation
Create `src/workbench/cycles_renderer.h`:

```cpp
#pragma once
#include "simple_renderer.h"

namespace sep {
    class CyclesRenderer : public SimpleRenderer {
    public:
        CyclesRenderer() = default;
        ~CyclesRenderer() = default;
    };
}
```

## Build Commands After Fix

```bash
# Clean build
rm -rf build
mkdir build
cd build

# Configure with workbench
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DSEP_BUILD_WORKBENCH=ON

# Build
ninja

# Test workbench
./sep_workbench
```

## Long-term Solution

The project appears to have planned integration with Blender's Cycles renderer (see `third_party/cycles/`). This would provide:
- Ray-traced visualization
- GPU-accelerated rendering
- Advanced material system
- Production-quality output

However, for immediate functionality, using the existing OpenGL renderer is sufficient.

## Verification

After applying the fix:
1. Build should complete without errors
2. Workbench should launch
3. Demos should display patterns visually
4. GPU acceleration should work via OpenGL

## Notes

- The existing `sep::workbench::Renderer` is fully functional
- It supports OpenGL rendering with shaders
- Pattern visualization is implemented
- Mouse interaction works
- The SimpleRenderer alternative also exists but is less tested