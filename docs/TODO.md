### TODO List: Fix Compilation & Runtime Issues

Prioritized by severity (CRITICAL first), grouped by file. Focus on analyzer defects causing build failures. Ignore LOW/MEDIUM unless blocking (e.g., dead stores). Use sed for batch namespace fixes: `sed -i 's/\bconfig\b/sep::config/g' file.cpp` (test first).

#### CRITICAL (Compilation Errors: Fix These First)
| File | Issue | Fix |
|------|-------|-----|
| blender/api.cpp | Unexpected namespace 'config'; undeclared 'config' (x7) | Replace 'config' with 'sep::pattern::PatternConfig'. sed: `sed -i 's/\bconfig\b/sep::pattern::PatternConfig/g' blender/api.cpp` |
| workbench/demos/demo_base.hpp | #include nested too deeply; no type 'Engine'/'CyclesRenderer' in 'sep' (x3) | Break circular includes: Remove self-include in demo_base.hpp; forward-declare sep::Engine/CyclesRenderer. |
| blender/blender_integration.cpp | Unknown type 'PatternConfig'; no member 'coherence'/'stability'/'entropy'/'mutation_count' in 'sep::Pattern' (x7) | Update to new Pattern struct (add members); qualify PatternConfig as sep::pattern::. sed: `sed -i 's/PatternConfig/sep::pattern::PatternConfig/g' blender/blender_integration.cpp` |
| workbench/renderer.cpp | No member 'scale'/'color'/'memory_tier' in 'sep::Pattern'; extraneous '}' (x5) | Add missing members to sep::Pattern; remove extra brace at L255. |
| Multiple demos/*.cpp (e.g., genesis_pattern.cpp) | Circular includes in demo_base.hpp (x20 instances across files) | Centralize includes in workbench_demo_adapter.hpp; remove recursive #includes. sed: `sed -i '/#include "workbench\/demos\/demo_base.hpp"/d' demos/*.hpp` (then add to adapter). |

#### HIGH (Runtime Crashes: Fix After Build)
| File | Issue | Fix |
|------|-------|-----|
| imgui.cpp | Garbage value in '^'; null reference; null deref (x3) | Init vars before use (e.g., crc in ImHashData); null-check window->InnerRect, apply_focus_window. |
| imgui_draw.cpp | Null object pointer call | Null-check font in ImDrawList::AddText. |
| cetintrin.h | Uninitialized arg in builtin (x2) | Init 't' before __builtin_ia32_rdsspd/q. |
| parser.h | Null deref in 'pod->type' | Null-check 'pod' before access. |

#### MEDIUM (Bugs/Safety: Fix for Stability)
| File | Issue | Fix |
|------|-------|-----|
| imgui.cpp/imgui_draw.cpp | Undefined memset on non-trivial (x61) | Use constructors/default-init instead of memset. sed: `sed -i 's/memset(this, 0, sizeof(\*this));/ /g' *.cpp` (manual review). |
| imgui_demo.cpp | Ignored sprintf return (x55) | Cast to void: `(void)sprintf(...);` sed: `sed -i 's/sprintf(/(void)sprintf(/g' imgui_demo.cpp` |
| Multiple (e.g., cuew.c) | Buffer overflow in strncat (x2) | Use `sizeof(command) - strlen(command) - 1`. |

#### Build/Run Steps After Fixes
1. Run `cmake .` in build dir.
2. `ninja clean; ninja -j` (watch for GLEW errors).
3. Launch: `./sep_workbench` (debug GLEW with `gdb --args ./sep_workbench`).
4. If GLEW persists: Set `GLEW_NO_GLU=1` env; link `-lGLU`.

Compress codebase: Merge namespaces (e.g., sed 's/sep::pattern/sep::/g' *.cpp); remove duplicates. Test incrementally.