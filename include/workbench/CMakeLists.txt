cmake_minimum_required(VERSION 3.15)

# Load CUDA toolchain file before project declaration
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/cuda-toolchain.cmake)
set(SEP_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/sep-targets.cmake)
# Set C++ standard before project declaration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SEPWorkbench CXX)

# Set CUDA compiler flags
set(CMAKE_CUDA_FLAGS "-Xcompiler -fno-gnu-unique -Wno-deprecated-gpu-targets")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} --use_fast_math --expt-relaxed-constexpr")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -include ${CMAKE_SOURCE_DIR}/include/compat/cuda_unified_fix.h")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -D__STRICT_ANSI__ -D_GLIBCXX_USE_CXX11_ABI=1")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -D_GLIBCXX_USE_NOEXCEPT_SPEC=0")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1")

# Add global definitions needed for CUDA compatibility
add_definitions(-DCUDA_UNIFIED_FIX_ENABLE_MATH_STUBS=1)
add_definitions(-DCUDA_UNIFIED_FIX_ENABLE_FUNCTION_RENAMING=1)
add_definitions(-D__STRICT_ANSI__)
add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
add_definitions(-D_GLIBCXX_USE_NOEXCEPT_SPEC=0)
add_definitions(-DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)

# Add custom module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# Use Cycles' custom CMake modules for dependencies like OSL and Alembic
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extern/cycles/src/cmake/Modules")


# Find required packages
find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenVDB REQUIRED)
find_package(OpenSubdiv REQUIRED)
find_package(OpenPGL REQUIRED)
find_package(OpenColorIO REQUIRED)
find_package(Imath REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(embree REQUIRED)
find_package(OSL REQUIRED)
find_package(Alembic REQUIRED)
find_package(Threads REQUIRED)
find_package(CUDAToolkit REQUIRED)

# Workbench executable
add_executable(sep_workbench
    main.cpp
    demo_manager.cpp
    demos/genesis_pattern.cpp
    demos/audio_visualizer.cpp
    demos/memory_garden.cpp
    demos/cosmo_demo.cpp
)

# Include directories
target_include_directories(sep_workbench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/imgui
    ${CMAKE_SOURCE_DIR}/extern/glm
)

# Add preprocessor definitions
target_compile_definitions(sep_workbench PRIVATE
    SEP_INCLUDE_DIR="${CMAKE_SOURCE_DIR}/include"
    SEP_HAS_AUDIO=1
    SEP_HAS_BLENDER=1
)

# Add preprocessor definitions
target_compile_definitions(sep_workbench PRIVATE
    SEP_INCLUDE_DIR="${CMAKE_SOURCE_DIR}/include"
)

# Link libraries
target_link_libraries(sep_workbench PRIVATE
    # 1. SEP Engine libraries
    sep_core
    sep_quantum
    sep_memory
    sep_blender
    sep_audio
    sep_compat

    # 2. Cycles and its dependencies
    cycles_session
    
    cycles_device
    cycles_kernel_osl
    cycles_subd
    cycles_util
    extern_sky
    extern_cuew
    extern_hipew

    # 3. External dependencies
    OpenImageIO
    ${OPENVDB_LIBRARIES}
    ${OPENSUBDIV_LIBRARIES}
    openvdb
    bf_intern_opensubdiv
    ${OSL_LIBRARIES}
    ${TBB_LIBRARIES}
    ${IMATH_LIBRARIES}
    ${CUDAToolkit_LIBRARIES}

    # 4. System libraries
    ${EMBREE_LIBRARIES}
    Imath
    ${OpenPGL_LIBRARIES}
    ${OpenColorIO_LIBRARIES}
    ${ALEMBIC_LIBRARIES}
    ${CMAKE_DL_LIBS}
    ${Threads_LIBRARIES}
    ${CUDAToolkit_LIBRARIES}
    pthread
    dl
    rt
    m
    z
)

# Set C++ standard
set_target_properties(sep_workbench PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

# Install target
install(TARGETS sep_workbench
    RUNTIME DESTINATION bin
)