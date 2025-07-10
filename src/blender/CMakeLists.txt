# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find necessary packages
find_package(CUDAToolkit REQUIRED)
find_package(OpenImageIO REQUIRED)
find_package(OSL REQUIRED)
find_package(TBB REQUIRED)
# Add other find_package() calls here for Embree, OpenVDB etc. as needed

# Find Alembic
find_package(Alembic REQUIRED)
message(STATUS "Alembic include dir: ${ALEMBIC_INCLUDE_DIR}")
message(STATUS "Alembic libraries: ${Alembic_LIBRARIES}")

# Find OpenPGL
find_package(OpenPGL REQUIRED)
message(STATUS "OpenPGL include dir: ${OPENPGL_INCLUDE_DIR}")
message(STATUS "OpenPGL libraries: ${OpenPGL_LIBRARIES}")

# Check for system-specific Alembic and OpenPGL paths
if(EXISTS "/usr/lib64/libAlembic.so")
  message(STATUS "Found system Alembic library at /usr/lib64/libAlembic.so")
endif()

if(EXISTS "/usr/lib64/libopenpgl.so")
  message(STATUS "Found system OpenPGL library at /usr/lib64/libopenpgl.so")
endif()


# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/blender/
        DESTINATION include/sep/blender
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/blender/
        DESTINATION include/sep/blender/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

        
# Define source files
set(BLENDER_SOURCES
    api.cpp
    blender_integration.cpp
    compression.cpp
    compression_utils.cpp
    profiler_stub.cpp
    factory.cpp
    gpu_context.cpp
    mesh_handler.cpp
    pattern_visualization_pipeline.cpp
)

# Add Cycles-specific files only when SEP_HAS_CYCLES is enabled
if(SEP_HAS_CYCLES)
    list(APPEND BLENDER_SOURCES
        cycles_renderer.cpp
    )
endif()

add_library(sep_blender SHARED ${BLENDER_SOURCES})

target_precompile_headers(sep_blender PRIVATE blender_pch.h)

# Define compile definitions in one place
target_compile_definitions(sep_blender PRIVATE
    SEP_HAS_BLENDER=${SEP_HAS_BLENDER}
    SEP_HAS_CYCLES=${SEP_HAS_CYCLES}
    SEP_HAS_CUDA=${SEP_HAS_CUDA}
    SEP_HAS_AUDIO=${SEP_HAS_AUDIO}
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
    WITH_CYCLES_DEVICE_CUDA
    "CCL_NAMESPACE_BEGIN=namespace ccl {"
    "CCL_NAMESPACE_END=}"
)

# Link all dependencies to the target
target_link_libraries(sep_blender PUBLIC
    sep_core
    sep_memory
    sep_quantum
    sep_compat
    ${OpenImageIO_LIBRARIES}
    ${OSL_LIBRARIES}
    ${TBB_LIBRARIES}
    
    # Use IMPORTED targets if available, otherwise use direct linking
    $<$<TARGET_EXISTS:Alembic::Abc>:Alembic::Abc>
    $<$<TARGET_EXISTS:Alembic::AbcGeom>:Alembic::AbcGeom>
    $<$<TARGET_EXISTS:Alembic::AbcCoreAbstract>:Alembic::AbcCoreAbstract>
    $<$<TARGET_EXISTS:Alembic::AbcCoreFactory>:Alembic::AbcCoreFactory>
    $<$<TARGET_EXISTS:OpenPGL::openpgl>:OpenPGL::openpgl>
    
    # Direct linking as fallback
    $<$<NOT:$<TARGET_EXISTS:Alembic::Abc>>:-Wl,--whole-archive -lAlembic -Wl,--no-whole-archive>
    $<$<NOT:$<TARGET_EXISTS:OpenPGL::openpgl>>:-Wl,--whole-archive -lopenpgl -Wl,--no-whole-archive>
    
    # Other dependencies
    CUDA::cudart
    CUDA::cuda_driver
    ${CMAKE_DL_LIBS}
    Threads::Threads
)

# Add rpath for the shared libraries
set_target_properties(sep_blender PROPERTIES
    INSTALL_RPATH "/usr/lib64"
    BUILD_WITH_INSTALL_RPATH TRUE
)

# Only link Cycles libraries when SEP_HAS_CYCLES is enabled
if(SEP_HAS_CYCLES)
    target_link_libraries(sep_blender PUBLIC
        cycles_device
        cycles_kernel
        cycles_scene
        cycles_util
    )
endif()

# Include directories
target_include_directories(sep_blender
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/cycles/src>
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${OpenImageIO_INCLUDE_DIRS}
        ${OSL_INCLUDE_DIRS}
        ${Alembic_INCLUDE_DIRS}
        ${OpenPGL_INCLUDE_DIRS}
)

# Properties for building a shared library
set_target_properties(sep_blender PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
