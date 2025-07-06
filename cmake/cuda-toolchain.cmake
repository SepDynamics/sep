#Custom CUDA toolchain file to bypass CMake's CUDA detection

# Set CUDA paths and compiler
set(CUDA_PATH "/usr/local/cuda")
set(CMAKE_CUDA_COMPILER "${CUDA_PATH}/bin/nvcc")
set(CMAKE_CUDA_HOST_COMPILER "/usr/bin/g++-14")

# Configure CUDA compiler settings
set(CMAKE_CUDA_ARCHITECTURES "60;61;70;75" CACHE STRING "CUDA architectures")
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Let CMake detect CUDA compiler naturally
unset(CMAKE_CUDA_COMPILER_FORCED)
unset(CMAKE_CUDA_COMPILER_WORKS CACHE)

#Set CUDA toolkit paths - use absolute paths without quotes
set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES ${CUDA_PATH}/include CACHE STRING "CUDA include dirs" FORCE) 
set(CUDA_TOOLKIT_ROOT_DIR ${CUDA_PATH} CACHE PATH "CUDA Toolkit location" FORCE)
set(CUDAToolkit_LIBRARY_DIR ${CUDA_PATH}/lib64 CACHE PATH "CUDA library directory" FORCE)
set(CUDA_TOOLKIT_TARGET_DIR ${CUDA_PATH}/targets/x86_64-linux CACHE PATH "CUDA Toolkit target location" FORCE) 
set(CUDA_INCLUDE_DIRS ${CUDA_PATH}/include CACHE PATH "CUDA Include location" FORCE)

#Set CUDA implicit information - critical for link line extraction
set(CMAKE_CUDA_IMPLICIT_INCLUDE_DIRECTORIES "${CUDA_PATH}/include" CACHE STRING "CUDA implicit includes" FORCE)

set(CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES "${CUDA_PATH}/lib64;${CUDA_PATH}/targets/x86_64-linux/lib;${CUDA_PATH}/targets/x86_64-linux/lib/stubs" CACHE STRING "CUDA implicit link directories" FORCE)

set(CMAKE_CUDA_IMPLICIT_LINK_LIBRARIES "cudart;cudart_static;cudadevrt;cuda;rt;pthread;dl" CACHE STRING "CUDA implicit libraries" FORCE)

set(CMAKE_CUDA_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "" CACHE STRING "CUDA implicit framework directories" FORCE)

# Add CUDA runtime library path to rpath
set(CMAKE_BUILD_RPATH "${CUDA_PATH}/lib64" CACHE STRING "Build rpath" FORCE)

#Set CUDA implicit link information
set(CMAKE_CUDA_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "" CACHE STRING "CUDA implicit framework directories" FORCE)

#Set CUDA host compiler - use absolute path without quotes
set(CMAKE_CUDA_HOST_COMPILER /usr/bin/g++-14 CACHE FILEPATH "Host compiler for CUDA" FORCE)

# Set CUDA flags for compiler identification
set(CMAKE_CUDA_FLAGS_INIT "-D__STRICT_ANSI__ -D_GLIBCXX_USE_CXX11_ABI=1")
set(CMAKE_CUDA_FLAGS_INIT "${CMAKE_CUDA_FLAGS_INIT} --allow-unsupported-compiler")
set(CMAKE_CUDA_FLAGS_INIT "${CMAKE_CUDA_FLAGS_INIT} -Xcompiler -fno-gnu-unique")
set(CMAKE_CUDA_FLAGS_INIT "${CMAKE_CUDA_FLAGS_INIT} -Xcompiler -fno-noexcept-type")

# Set CUDA flags for actual compilation
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS_INIT}")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1")

# Set CUDA flags for compiler identification
set(CMAKE_CUDA_FLAGS_INIT "-D__STRICT_ANSI__ -D_GLIBCXX_USE_CXX11_ABI=1")
set(CMAKE_CUDA_FLAGS_INIT "${CMAKE_CUDA_FLAGS_INIT} --allow-unsupported-compiler")
set(CMAKE_CUDA_FLAGS_INIT "${CMAKE_CUDA_FLAGS_INIT} -DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1")

# Set CUDA flags for actual compilation
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS_INIT}")

# Allow CMake to detect CUDA compiler
unset(CMAKE_CUDA_COMPILER_FORCED)
unset(CMAKE_CUDA_COMPILER_WORKS CACHE)

#Explicitly set CUDA libraries - use absolute paths without quotes
set(CUDA_LIBRARIES 
    ${CUDA_PATH}/lib64/libcudart.so
    ${CUDA_PATH}/lib64/libcudadevrt.a
    ${CUDA_PATH}/lib64/libcudart_static.a
  CACHE STRING "CUDA libraries" FORCE
)

#Set CUDA runtime library - use absolute path without quotes
set(CUDA_CUDART_LIBRARY ${CUDA_PATH}/lib64/libcudart.so CACHE FILEPATH "CUDA Runtime library" FORCE)

#Set archive creation command for Ninja
set(CMAKE_CUDA_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <OBJECTS>" CACHE STRING "CUDA archive creation command" FORCE)
set(CMAKE_CUDA_ARCHIVE_APPEND "<CMAKE_AR> q <TARGET> <OBJECTS>" CACHE STRING "CUDA archive append command" FORCE)
set(CMAKE_CUDA_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>" CACHE STRING "CUDA archive finish command" FORCE)
