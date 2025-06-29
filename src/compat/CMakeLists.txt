# src/compat/CMakeLists.txt
# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

# Create compat library with all sources
add_library(sep_compat STATIC
    core.cu
    cuda_api.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    utils.cu
    stream.cpp
    raii.cpp
    component_bridge.cpp
)

# Configure CUDA target properties using the helper function
configure_cuda_target(sep_compat)

# Link CUDA libraries
target_link_libraries(sep_compat PUBLIC ${CUDA_LIBRARIES})

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
