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

## Initializing External Dependencies

Before configuring the project, pull the Cycles submodule:

```bash
git submodule update --init extern/cycles
```

This ensures the build scripts can locate the Cycles sources.

## Required System Packages

The build relies on the system `http_parser` library and the CUDA toolkit.
On Ubuntu-based distributions install them with:

```bash
sudo apt-get install libhttp-parser-dev nvidia-cuda-toolkit
```

If the CUDA toolkit is installed in a non-standard location set `CUDAToolkit_ROOT`
so CMake can locate `nvcc`.
