# Circular Dependency Resolution Strategy

## Understanding Your Dependency Crisis

Your codebase has 23 circular dependencies creating an "include maze" that makes compilation order undefined and causes:
- Compilation failures
- Increased build times
- Maintenance nightmares
- Impossible-to-test modules

### Current Circular Dependency Patterns

```
1. Self-inclusion (!!!):
   common.h → common.h

2. CUDA Runtime Loop:
   cuda_runtime.h → cuda_runtime.h → gpu_context.h → cuda_runtime.h

3. PipeWire Implementation Tangles:
   impl.h ↔ control.h
   context.h ↔ core.h ↔ proxy.h ↔ protocol.h
   
4. Complex Multi-hop Cycles:
   global.h → impl.h → impl-*.h → global.h
```

## Detection and Analysis Tools

### Step 1: Dependency Visualization

```python
#!/usr/bin/env python3
# tools/analyze_includes.py

import os
import re
import networkx as nx
import matplotlib.pyplot as plt
from collections import defaultdict

class IncludeAnalyzer:
    def __init__(self, src_dir):
        self.src_dir = src_dir
        self.graph = nx.DiGraph()
        self.cycles = []
        
    def analyze(self):
        # Parse all headers
        for root, dirs, files in os.walk(self.src_dir):
            for file in files:
                if file.endswith(('.h', '.hpp')):
                    filepath = os.path.join(root, file)
                    self._parse_includes(filepath)
        
        # Find cycles
        self.cycles = list(nx.simple_cycles(self.graph))
        return self
    
    def _parse_includes(self, filepath):
        relative_path = os.path.relpath(filepath, self.src_dir)
        self.graph.add_node(relative_path)
        
        with open(filepath, 'r') as f:
            content = f.read()
            
        # Find #include statements
        include_pattern = r'#include\s*[<"]([^>"]+)[>"]'
        includes = re.findall(include_pattern, content)
        
        for include in includes:
            # Normalize the include path
            if not include.startswith('/'):
                include_path = os.path.normpath(
                    os.path.join(os.path.dirname(relative_path), include)
                )
                if os.path.exists(os.path.join(self.src_dir, include_path)):
                    self.graph.add_edge(relative_path, include_path)
    
    def report_cycles(self):
        print(f"Found {len(self.cycles)} circular dependencies:\n")
        for i, cycle in enumerate(self.cycles, 1):
            print(f"Cycle {i}:")
            for j in range(len(cycle)):
                print(f"  {cycle[j]} → {cycle[(j+1) % len(cycle)]}")
            print()
    
    def visualize(self, output_file='dependency_graph.png'):
        plt.figure(figsize=(20, 20))
        
        # Color nodes involved in cycles
        node_colors = []
        cyclic_nodes = set()
        for cycle in self.cycles:
            cyclic_nodes.update(cycle)
        
        for node in self.graph.nodes():
            if node in cyclic_nodes:
                node_colors.append('red')
            else:
                node_colors.append('lightblue')
        
        pos = nx.spring_layout(self.graph, k=2, iterations=50)
        nx.draw(self.graph, pos, node_colors=node_colors, 
                with_labels=True, node_size=100, font_size=8,
                arrows=True, edge_color='gray')
        
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()

if __name__ == '__main__':
    analyzer = IncludeAnalyzer('src/')
    analyzer.analyze().report_cycles()
    analyzer.visualize()
```

### Step 2: Dependency Metrics

```bash
#!/bin/bash
# tools/dependency_metrics.sh

echo "=== Include Statistics ==="
echo "Total includes:"
find src -name "*.h" -o -name "*.hpp" | xargs grep -h "^#include" | wc -l

echo -e "\nMost included files:"
find src -name "*.h" -o -name "*.hpp" | xargs grep -h "^#include" | \
    sed 's/#include.*[<"]\(.*\)[>"].*/\1/' | sort | uniq -c | sort -nr | head -20

echo -e "\nFiles with most includes:"
for file in $(find src -name "*.h" -o -name "*.hpp"); do
    count=$(grep -c "^#include" "$file" 2>/dev/null || echo 0)
    echo "$count $file"
done | sort -nr | head -20
```

## Resolution Strategies

### Strategy 1: Forward Declaration

**Before (Circular)**:
```cpp
// a.h
#pragma once
#include "b.h"
class A {
    B b_instance;  // Requires full definition
};

// b.h
#pragma once
#include "a.h"
class B {
    A* a_ptr;  // Only needs forward declaration
};
```

**After (Fixed)**:
```cpp
// a_fwd.h
#pragma once
class A;

// b_fwd.h
#pragma once
class B;

// a.h
#pragma once
#include "b.h"  // Still needs full definition
class A {
    B b_instance;
};

// b.h
#pragma once
#include "a_fwd.h"  // Only forward declaration
class B {
    A* a_ptr;
};
```

### Strategy 2: Interface Extraction

**Pattern for Breaking Cycles**:
```cpp
// interfaces/i_quantum_processor.h
#pragma once
#include <memory>

namespace sep::quantum {

class IQuantumProcessor {
public:
    virtual ~IQuantumProcessor() = default;
    virtual void process() = 0;
    virtual size_t get_qubit_count() const = 0;
};

using IQuantumProcessorPtr = std::unique_ptr<IQuantumProcessor>;

} // namespace

// quantum/processor.h
#pragma once
#include "interfaces/i_quantum_processor.h"
// No need to include gpu_context.h here!

namespace sep::quantum {

class QuantumProcessor : public IQuantumProcessor {
public:
    QuantumProcessor();
    ~QuantumProcessor();
    
    void process() override;
    size_t get_qubit_count() const override;
    
private:
    class Impl;  // PIMPL pattern
    std::unique_ptr<Impl> pImpl;
};

} // namespace

// quantum/processor.cpp
#include "processor.h"
#include "gpu_context.h"  // Include heavy dependencies here

class QuantumProcessor::Impl {
    GpuContext context;  // Full implementation details hidden
public:
    void process() { /* ... */ }
    size_t get_qubit_count() const { /* ... */ }
};

QuantumProcessor::QuantumProcessor() : pImpl(std::make_unique<Impl>()) {}
QuantumProcessor::~QuantumProcessor() = default;  // Must be in .cpp for PIMPL

void QuantumProcessor::process() { pImpl->process(); }
size_t QuantumProcessor::get_qubit_count() const { return pImpl->get_qubit_count(); }
```

### Strategy 3: Dependency Inversion

**Create Abstract Layers**:
```
src/
├── core/
│   └── interfaces/        # Pure virtual interfaces
├── modules/
│   ├── quantum/          # Depends only on interfaces
│   ├── gpu/              # Implements interfaces
│   └── memory/           # Implements interfaces
└── integration/          # Wires everything together
```

### Strategy 4: Module Boundaries

```cpp
// modules/quantum/public/quantum_api.h
// This is the ONLY header other modules can include
#pragma once

namespace sep::quantum {
    // Only forward declarations and interfaces
    class IQuantumProcessor;
    class IQuantumState;
    
    // Factory functions
    std::unique_ptr<IQuantumProcessor> create_processor();
}

// modules/quantum/CMakeLists.txt
add_library(sep_quantum STATIC
    # Implementation files
    src/processor.cpp
    src/state.cpp
)

target_include_directories(sep_quantum
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
```

## Specific Resolution Patterns

### Pattern 1: The Self-Include (common.h → common.h)

**Diagnosis**: Likely a typo or misunderstanding

**Fix**:
```cpp
// crow/common.h
#pragma once  // Ensure this is present
// #include <crow/common.h>  // DELETE THIS LINE!
#include <crow/utility.h>     // Include what you actually need
```

### Pattern 2: The CUDA Tangle

**Current Problem**:
```
cuda_runtime.h → cuda_runtime.h → gpu_context.h → cuda_runtime.h
```

**Resolution Steps**:

1. **Create a minimal CUDA types header**:
```cpp
// cuda/cuda_types_fwd.h
#pragma once

// Forward declare CUDA types without including CUDA headers
typedef struct CUstream_st* cudaStream_t;
typedef enum cudaError cudaError_t;

namespace sep::cuda {
    class Stream;
    class Context;
}
```

2. **Layer your includes properly**:
```cpp
// cuda/cuda_includes.h
#pragma once

// System CUDA headers (guarded)
#ifdef SEP_HAS_CUDA
  #include <cuda_runtime_api.h>
#endif

// Our forward declarations
#include "cuda_types_fwd.h"

// Our wrappers (no circular deps possible)
#include "cuda_wrapper_decl.h"
```

### Pattern 3: The PipeWire Maze

**Problem**: External library with circular dependencies

**Solution**: Isolate with a facade
```cpp
// audio/pipewire_facade.h
#pragma once
#include <memory>

namespace sep::audio {

// Hide all PipeWire types
class PipeWireContext {
public:
    PipeWireContext();
    ~PipeWireContext();
    
    bool initialize();
    void shutdown();
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace

// audio/pipewire_facade.cpp
#include "pipewire_facade.h"

// Include all the circular PipeWire headers here
extern "C" {
#include <pipewire/pipewire.h>
#include <pipewire/impl.h>
// ... etc
}

class PipeWireContext::Impl {
    pw_main_loop* loop = nullptr;
    // Implementation using PipeWire
};
```

## Implementation Phases

### Phase 1: Quick Fixes (Day 1-2)
1. Remove self-includes
2. Add missing include guards
3. Fix obvious typos

### Phase 2: Forward Declarations (Day 3-5)
1. Create `*_fwd.h` headers for major classes
2. Replace includes with forward declarations where possible
3. Move method implementations to .cpp files

### Phase 3: Interface Extraction (Week 2)
1. Identify tight coupling points
2. Extract interfaces for major components
3. Use dependency injection

### Phase 4: Module Reorganization (Week 3-4)
1. Define clear module boundaries
2. Create public API headers
3. Enforce include restrictions

## Automated Enforcement

### CMake Include Scanners

```cmake
# cmake/IncludeScanner.cmake
function(check_circular_dependencies TARGET)
    get_target_property(SOURCES ${TARGET} SOURCES)
    
    foreach(SOURCE ${SOURCES})
        execute_process(
            COMMAND ${Python3_EXECUTABLE} 
                    ${CMAKE_SOURCE_DIR}/tools/check_includes.py 
                    ${SOURCE}
            RESULT_VARIABLE result
            OUTPUT_VARIABLE output
        )
        
        if(NOT result EQUAL 0)
            message(FATAL_ERROR "Circular dependency found in ${SOURCE}:\n${output}")
        endif()
    endforeach()
endfunction()

# Use in your CMakeLists.txt
add_library(sep_quantum ...)
check_circular_dependencies(sep_quantum)
```

### Pre-commit Hooks

```yaml
# .pre-commit-config.yaml
repos:
  - repo: local
    hooks:
      - id: check-circular-deps
        name: Check for circular dependencies
        entry: python tools/check_includes.py
        language: python
        files: \.(cpp|h|hpp|cc|cxx)$
        additional_dependencies: [networkx]
```

## Testing Your Fixes

### Include What You Use (IWYU)

```bash
# Install IWYU
sudo apt-get install iwyu

# Run on your codebase
iwyu_tool.py -p build/ -- -Xiwyu --mapping_file=iwyu.imp

# Create mapping file (iwyu.imp) for your project
[
  { include: ["<cuda_runtime_api.h>", "private", "\"cuda/cuda_types.h\"", "public"] },
  { include: ["@<pipewire/.*>", "private", "\"audio/pipewire_facade.h\"", "public"] }
]
```

### Compilation Order Test

```bash
#!/bin/bash
# tools/test_include_order.sh

# Try to compile each header in isolation
for header in $(find src -name "*.h" -o -name "*.hpp"); do
    echo "Testing $header..."
    
    # Create a minimal source file that just includes the header
    cat > test_include.cpp << EOF
#include "$header"
int main() { return 0; }
EOF
    
    # Try to compile it
    if ! g++ -std=c++17 -I src/ -c test_include.cpp -o /dev/null 2>/dev/null; then
        echo "FAIL: $header cannot be included independently"
    fi
    
    rm -f test_include.cpp
done
```

## Success Metrics

Track your progress:

| Metric | Before | Target | Current |
|--------|--------|--------|---------|
| Circular dependencies | 23 | 0 | ? |
| Average includes/file | 15 | 5 | ? |
| Compilation time | ? | -50% | ? |
| Independent headers | 20% | 95% | ? |

## Common Mistakes to Avoid

1. **Don't just add more includes** - This makes the problem worse
2. **Don't use `#pragma once` AND include guards** - Pick one (prefer `#pragma once`)
3. **Don't forward declare templates** - It's usually not worth the complexity
4. **Don't put implementations in headers** - Unless they're templates or inline

This systematic approach will untangle your include web and create a maintainable, fast-compiling codebase.