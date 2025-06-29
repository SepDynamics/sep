#CustomCUDA.cmake
#Custom CMake module for CUDA compilation that bypasses CMake's built-in CUDA language support
set(LANGUAGES CUDA)
# Force apply CUDA flags early for compiler identification

find_package(CUDAToolkit QUIET)

#--- CUDA paths and settings ---
set(CUDA_PATH "/usr/local/cuda-12.9" CACHE PATH "Path to CUDA installation")

#--- CUDA architectures ---
set(CUDA_ARCHITECTURES "70;75;80;86;89" CACHE STRING "CUDA architectures to compile for")

#--- CUDA include directories ---
set(CUDA_INCLUDE_DIRS "${CUDA_PATH}/include" CACHE PATH "CUDA include directories")

#--- CUDA libraries ---
# Use full paths to CUDA libraries instead of just names to fix linking issues
set(CUDA_LIBRARIES
    "${CUDA_PATH}/targets/x86_64-linux/lib/libcudart.so"
    "${CUDA_PATH}/targets/x86_64-linux/lib/libcudadevrt.a"
    "${CUDA_PATH}/targets/x86_64-linux/lib/libcudart_static.a"
    CACHE STRING "CUDA libraries" FORCE
)
set(CUDA_LIBRARY_DIRS
    "${CUDA_PATH}/lib64"
    "${CUDA_PATH}/targets/x86_64-linux/lib"
    "${CUDA_PATH}/targets/x86_64-linux/lib/stubs"
    CACHE STRING "CUDA library directories"
)

# Check if the expected CUDA library path exists; if not, disable CUDA support.
if(NOT EXISTS "${CUDA_PATH}/lib64")
    message(WARNING "CUDA path ${CUDA_PATH} not found; disabling CUDA support")
    set(CUDAToolkit_FOUND OFF CACHE BOOL "CUDA toolkit found" FORCE)
    set(CUDA_LIBRARIES "" CACHE STRING "CUDA libraries" FORCE)
    set(CUDA_LIBRARY_DIRS "" CACHE STRING "CUDA library directories" FORCE)
else()
    # Add CUDA library directories and include directories globally
    link_directories(${CUDA_LIBRARY_DIRS})
    include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
endif()

#--- Host compiler flags ---
# Enforce C++20 for GCC 14
set(HOST_CXX_FLAGS "-std=c++17")

# Make sure CMake uses the GCC 14 host compiler for CUDA
set(CMAKE_CUDA_HOST_COMPILER ${CUDA_HOST_COMPILER})

#--- CUDA compilation flags ---
set(CUDA_NVCC_FLAGS)
list(APPEND CUDA_NVCC_FLAGS
    "-allow-unsupported-compiler"
    "-use_fast_math"
    "-expt-relaxed-constexpr"
    "-extended-lambda"
    "-ccbin=${CUDA_HOST_COMPILER}"
    "-D__CUDACC__"
    "-DSEP_NV_NO_HOST_COMPILER_CHECK=1"
    "-D__CUDACC_EXTENDED_LAMBDA__"
    "-DSEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1"
    "-D__CUDA_NO_FP8_CONVERSIONS__"
    "-D__CUDA_NO_HALF_OPERATORS__"
    "-D__CUDA_NO_HALF2_OPERATORS__"
    "-D__CUDA_NO_BFLOAT16_CONVERSIONS__"
    "-DCURAND_SKIP_HOST_KERNELS=1"
    "-I${CMAKE_SOURCE_DIR}/include"
    "-isystem"
    "${CUDA_PATH}/include"
    "-std=c++17"
)

# Check if real OSL libraries are available
if(OSL_OSLCOMP_LIBRARY AND OSL_OSLEXEC_LIBRARY AND OSL_OSLQUERY_LIBRARY)
    message(STATUS "Using real OSL libraries: ${OSL_OSLCOMP_LIBRARY}")
    # Create a proxy library that forwards to the real OSL libraries
    add_library(cycles_osl INTERFACE)
    target_include_directories(cycles_osl INTERFACE
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${OSL_INCLUDE_DIR}
    )
    target_link_libraries(cycles_osl INTERFACE
        ${OSL_OSLCOMP_LIBRARY}
        ${OSL_OSLEXEC_LIBRARY}
        ${OSL_OSLQUERY_LIBRARY}
        ${OSL_OSLNOISE_LIBRARY}
    )
    set_target_properties(cycles_osl PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    )
    message(STATUS "Added cycles_osl real library forwarding target")
endif()

# Pass host compiler flags through -Xcompiler
list(APPEND CUDA_NVCC_FLAGS "-Xcompiler" "${HOST_CXX_FLAGS}")

#--- Function to configure common CUDA target properties ---
function(configure_cuda_target target_name)
    set_target_properties(
        ${target_name} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        LINKER_LANGUAGE CXX
        CUDA_SEPARABLE_COMPILATION ON
        CUDA_ARCHITECTURES "${CUDA_ARCHITECTURES}"
    )

    target_include_directories(${target_name} PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src
        ${CUDA_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${target_name} PRIVATE
        $<$<COMPILE_LANGUAGE:CUDA>:
            __CUDACC__
            __CUDA_ARCH__
            __CUDA_NO_HALF_OPERATORS__
            __CUDA_NO_HALF2_OPERATORS__
            __CUDA_NO_BFLOAT16_CONVERSIONS__
            CUDA_API_PER_THREAD_DEFAULT_STREAM
        >
    )

    # The original --pre-include flag was malformed. Corrected and added other flags.
    target_compile_options(
        ${target_name} PRIVATE
        $<$<COMPILE_LANGUAGE:CUDA>:
            --pre-include=${CMAKE_SOURCE_DIR}/include/cuda_unified_fix.h
            --use_fast_math
            --expt-relaxed-constexpr
            --extended-lambda
        >
    )
endfunction()

#--- Print CUDA configuration ---
message(STATUS "Using custom CUDA compilation pipeline")
message(STATUS "CUDA Path: ${CUDA_PATH}")
message(STATUS "CUDA NVCC: ${CUDA_NVCC}")
message(STATUS "CUDA Host Compiler: ${CUDA_HOST_COMPILER}")
message(STATUS "CUDA Architectures: ${CUDA_ARCHITECTURES}")
message(STATUS "CUDA Libraries: ${CUDA_LIBRARIES}")
message(STATUS "CUDA Library Dirs: ${CUDA_LIBRARY_DIRS}")
