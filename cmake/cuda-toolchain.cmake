# cmake/cuda-toolchain.cmake - CORRECTED VERSION
set(CUDA_PATH "/usr/local/cuda-12.9")
set(CMAKE_CUDA_COMPILER "${CUDA_PATH}/bin/nvcc")
set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/clang++-15")

# CRITICAL FIX: Use /include not /src
set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES "${CUDA_PATH}/include")
set(CUDA_INCLUDE_DIRS "${CUDA_PATH}/include")
set(CUDA_TOOLKIT_ROOT_DIR "${CUDA_PATH}")

# Add --allow-unsupported-compiler flag and force compat includes
set(CMAKE_CUDA_FLAGS "--allow-unsupported-compiler -Xcompiler -isystem -Xcompiler ${CMAKE_SOURCE_DIR}/third_party/compat")
