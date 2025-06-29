#CustomCUDA.cmake
#Custom CMake module for CUDA compilation that bypasses CMake's built-in CUDA language support
# Find CUDA toolkit and set paths
find_package(CUDAToolkit REQUIRED)
if(NOT CUDAToolkit_FOUND)
    message(FATAL_ERROR "CUDA Toolkit not found")
endif()

# Set CUDA paths based on found toolkit
set(CUDA_PATH ${CUDAToolkit_ROOT_DIR} CACHE PATH "Path to CUDA installation")
message(STATUS "Found CUDA Toolkit at: ${CUDA_PATH}")

# Ensure CUDA runtime library exists
if(NOT EXISTS "${CUDAToolkit_LIBRARY_DIR}/libcudart.so")
    message(FATAL_ERROR "CUDA runtime library not found in ${CUDAToolkit_LIBRARY_DIR}")
endif()
# Find CUDA compiler with expanded search paths
find_program(CUDA_NVCC
    NAMES nvcc
    PATHS
        ${CUDA_PATH}/bin
        /usr/local/cuda*/bin
        /usr/local/cuda/bin
        /opt/cuda/bin
    DOC "CUDA compiler (nvcc)"
    NO_DEFAULT_PATH
)

if(NOT CUDA_NVCC)
    message(FATAL_ERROR "CUDA nvcc compiler not found. Searched in: ${CUDA_PATH}/bin, /usr/local/cuda*/bin")
endif()

message(STATUS "Found CUDA compiler: ${CUDA_NVCC}")

# Get CUDA version and set architectures
execute_process(
    COMMAND ${CUDA_NVCC} --version
    OUTPUT_VARIABLE NVCC_OUT
    ERROR_VARIABLE NVCC_ERR
    RESULT_VARIABLE NVCC_RES
)

if(NVCC_RES EQUAL 0)
    string(REGEX MATCH "release ([0-9]+)\\.([0-9]+)" CUDA_VERSION_MATCH "${NVCC_OUT}")
    set(CUDA_VERSION_MAJOR "${CMAKE_MATCH_1}")
    set(CUDA_VERSION_MINOR "${CMAKE_MATCH_2}")
    
    # Set architectures based on CUDA version
    if(CUDA_VERSION_MAJOR GREATER_EQUAL 11)
        set(CUDA_ARCHITECTURES "70;75;80;86" CACHE STRING "CUDA architectures to compile for")
    else()
        set(CUDA_ARCHITECTURES "60;70;75" CACHE STRING "CUDA architectures to compile for")
    endif()
else()
    message(WARNING "Failed to detect CUDA version, defaulting to common architectures")
    set(CUDA_ARCHITECTURES "70;75" CACHE STRING "CUDA architectures to compile for")
endif()

#--- CUDA include directories ---
set(CUDA_INCLUDE_DIRS "${CUDA_PATH}/include" CACHE PATH "CUDA include directories")

#--- CUDA libraries ---
# Use full paths to CUDA libraries instead of just names to fix linking issues
# Set CUDA library paths based on actual installation
# Dynamically find CUDA library directories
set(CUDA_LIBRARY_DIRS
    "${CUDA_PATH}/targets/x86_64-linux/lib"
    "${CUDA_PATH}/lib64"
    "${CUDA_PATH}/lib"
    CACHE STRING "CUDA library directories"
)

# Find CUDA runtime library
find_library(CUDA_CUDART_LIBRARY
    NAMES cudart
    PATHS ${CUDAToolkit_LIBRARY_DIR}
    NO_DEFAULT_PATH
)

if(NOT CUDA_CUDART_LIBRARY)
    message(FATAL_ERROR "Could not find CUDA runtime library (libcudart.so) in ${CUDA_LIBRARY_DIRS}")
endif()

message(STATUS "Found CUDA Runtime Library: ${CUDA_CUDART_LIBRARY}")

set(CUDA_LIBRARIES
    ${CUDA_CUDART_LIBRARY}
    CACHE STRING "CUDA libraries" FORCE
)

# Add runtime path for dynamic linking
set(CMAKE_BUILD_RPATH ${CUDA_LIBRARY_DIRS})

# Add CUDA library directories and include directories globally
link_directories(${CUDA_LIBRARY_DIRS})
include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})



# Find GCC 14
find_program(GCC_14 NAMES g++-14 g++)
if(NOT GCC_14)
    message(FATAL_ERROR "GCC 14 compiler not found")
endif()

#--- CUDA compilation flags ---
set(CUDA_NVCC_FLAGS)

# Ensure CUDA uses GCC 14
set(CUDA_HOST_COMPILER ${GCC_14})
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
