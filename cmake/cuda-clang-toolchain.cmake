# Custom CUDA toolchain for Clang
set(CMAKE_CUDA_HOST_COMPILER "clang++-15")
set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES ${CUDA_PATH}/include CACHE STRING "CUDA include dirs" FORCE)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")