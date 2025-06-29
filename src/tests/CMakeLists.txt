# CMakeLists.txt for Cycles integration tests

# Include CUDA configuration
include(${CMAKE_SOURCE_DIR}/cmake/cuda-toolchain.cmake)

# Configure CUDA compiler and flags
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -allow-unsupported-compiler")
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Add CUDA source files
set(CUDA_SOURCES
    ${CMAKE_SOURCE_DIR}/src/memory/memory.cu
    ${CMAKE_SOURCE_DIR}/src/compat/cuda_api.cu
    ${CMAKE_SOURCE_DIR}/src/compat/core.cu
    ${CMAKE_SOURCE_DIR}/src/compat/pattern_kernels.cu
    ${CMAKE_SOURCE_DIR}/src/compat/quantum_kernels.cu
    ${CMAKE_SOURCE_DIR}/src/compat/utils.cu
    ${CMAKE_SOURCE_DIR}/src/compat/event.cu
)

# Add main test executable
add_executable(cycles_test
    cycles_test.cpp
    ${CUDA_SOURCES}
)

# Configure include directories
target_include_directories(cycles_test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${CUDA_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/api
        ${CMAKE_SOURCE_DIR}/include/audio
        ${CMAKE_SOURCE_DIR}/include/blender
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/include/crow
        ${CMAKE_SOURCE_DIR}/include/memory
        ${CMAKE_SOURCE_DIR}/include/quantum
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)

# Link against the same libraries as the main executable to ensure consistency
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_libraries(cycles_test
        PRIVATE
        # First link CUDA implementation to resolve sep::cuda symbols
        ${CUDA_LIBRARIES}
        ${CYCLES_CUDA_LIBRARIES}
        # Then link all Cycles components grouped for dependency resolution
        -Wl,--start-group
        ${CYCLES_LINK_ORDER}
        ${CYCLES_SCENE_LIBRARIES}
        ${CYCLES_KERNEL_LIBRARIES}
        ${CYCLES_DEVICE_LIBRARIES}
        ${CYCLES_RENDER_LIBRARIES}
        ${CYCLES_RENDER_LIB}
        -Wl,--end-group

        # Link OSL implementation with group to avoid duplicate symbols
        -Wl,--start-group
        ${CYCLES_OSL_LIBRARY}
        -Wl,--end-group

        # Then link system OSL libraries in correct order
        ${OSL_COMP_LIBRARY}
        ${OSL_EXEC_LIBRARY}
        ${OSL_QUERY_LIBRARY}
        
        # Then OpenImageIO which OSL depends on
        ${OPENIMAGEIO_LIBRARY}
        ${OPENIMAGEIO_UTIL_LIBRARY}
        ${LLVM_LIBRARIES}
        ${PUGIXML_LIBRARIES}
        # Link compat library for CUDA implementation
        sep_compat
        # Then link internal libraries that might depend on Cycles
        sep_api
        sep_blender
        sep_audio
        sep_quantum
        sep_memory
        sep_core
        ${BLENDER_LINK_ORDER}
        ${EXTERNAL_LINK_ORDER}
        ${CURL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        ${CUDA_LIBRARIES}
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
        dl
        m
    )
else()
    # Fallback for other compilers
    target_link_libraries(cycles_test
        PRIVATE
        ${CYCLES_LIBRARIES}
        # OSL libraries
        ${OSL_COMP_LIBRARY}
        ${OSL_EXEC_LIBRARY}
        ${OSL_QUERY_LIBRARY}
        ${SEP_INTERNAL_LIBS}
        ${BLENDER_LINK_ORDER}
        ${EXTERNAL_LINK_ORDER}
        ${CURL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        CUDAToolkit::cudart
        CUDAToolkit::cuda_driver
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
        dl
        m
    )
endif()

add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)

