# Build Guidelines

The project requires C++17 across all targets. The root `CMakeLists.txt` sets
this globally with the following lines:

```cmake
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
```

All subdirectories inherit these settings so no other CMake file should
redefine `CMAKE_CXX_STANDARD`. Verify that `tests/` and external dependencies
(such as `extern/cycles`) do not override the standard.
