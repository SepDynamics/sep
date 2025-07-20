# cmake/cuda-override-toolchain.cmake
set(CUDA_PATH "/usr/local/cuda-12.9")
set(CMAKE_CUDA_COMPILER "${CUDA_PATH}/bin/nvcc")
set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/clang++-15")

# CRITICAL FIX: Unset default include directories
unset(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES CACHE)
unset(CUDA_INCLUDE_DIRS CACHE)

# Set new include directories with compat layer first
set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/third_party/compat" "${CUDA_PATH}/include")
set(CUDA_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/third_party/compat" "${CUDA_PATH}/include")
set(CUDA_TOOLKIT_ROOT_DIR "${CUDA_PATH}")

# Add --allow-unsupported-compiler flag
set(CMAKE_CUDA_FLAGS "--allow-unsupported-compiler")