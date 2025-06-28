# src/compat/CMakeLists.txt
# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

add_library(sep_compat STATIC
    stream.cpp
    raii.cpp
    component_bridge.cpp
    # Add all .cu files here to be compiled by NVCC
    core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
    cycles_osl_stub.cpp
)

if(NOT SEP_CUDA_AVAILABLE)
  target_sources(sep_compat PRIVATE core_stub.cpp)
endif()

# Comment out CUDA configuration
set_source_files_properties(
     core.cu
     cuda_api.cu
     event.cu
     pattern_kernels.cu
     quantum_kernels.cu
     utils.cu
)

# Add include directories needed by compat library sources
target_include_directories(sep_compat
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/extern
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
else()
    # Fall back to stub if real libraries aren't available
    message(STATUS "Real OSL libraries not found, building stub instead")
    add_library(cycles_osl STATIC cycles_osl_stub.cpp)
    target_include_directories(cycles_osl PUBLIC
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
    )
    set_target_properties(cycles_osl PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    )
    message(STATUS "Added cycles_osl stub library build target")
endif()
