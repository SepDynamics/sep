find_package(CUDAToolkit REQUIRED)
find_package(TBB REQUIRED)

add_library(sep_blender STATIC
    compression_utils.cpp
    gpu_context.cpp
    compression.cpp
    api.cpp
    pattern_visualization_pipeline.cpp
    blender_integration.cpp
    mesh_handler.cpp
    factory.cpp
)

# Define required features
target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_BLENDER=1
    PRIVATE
        SEP_HAS_CUDA=1
)

# Link dependencies
target_link_libraries(sep_blender
    PUBLIC
        sep_core
    PRIVATE
        sep_compat
        ${CMAKE_SOURCE_DIR}/libsep_compat.a
        TBB::tbb
)

# Enable CUDA support
if(SEP_HAS_CUDA)
    target_compile_definitions(sep_blender PRIVATE SEP_HAS_CUDA=1)
    target_link_libraries(sep_blender PRIVATE CUDA::cudart)
endif()

# Always build the Cycles renderer. When real Cycles is unavailable the
# implementation relies on stub types provided in `compat/cycles.h`.
target_sources(sep_blender PRIVATE cycles_renderer.cpp)

# Extra compiler flags are only needed when linking against the real Cycles
# libraries.
if(SEP_HAS_CYCLES)
  set_source_files_properties(cycles_renderer.cpp
    PROPERTIES
    COMPILE_FLAGS "-DWITH_CYCLES_DEVICE_CUDA"
  )
endif()

# Link directly against Blender's Cycles
set(BLENDER_VERSION "5.0")
set(BLENDER_USER_CONFIG_DIR "$ENV{HOME}/.config/blender/${BLENDER_VERSION}")

# Use bundled Cycles headers when available
if(SEP_HAS_CYCLES)
  target_include_directories(sep_blender PRIVATE
      ${CMAKE_SOURCE_DIR}/extern/cycles/src
  )
endif()

target_compile_definitions(sep_blender
    PUBLIC
        SEP_HAS_CYCLES=${SEP_HAS_CYCLES}
    PRIVATE
        WITH_CYCLES_DEVICE_CUDA
)

# Use bundled dependencies
target_include_directories(sep_blender PRIVATE
    ${CMAKE_SOURCE_DIR}/extern/nanovdb
    ${CMAKE_SOURCE_DIR}/extern/oidn
)

# Link against Cycles libraries and required dependencies
if(SEP_HAS_CYCLES)
  target_link_libraries(sep_blender PRIVATE
      ${CMAKE_SOURCE_DIR}/lib/libcycles_device.a
      ${CMAKE_SOURCE_DIR}/lib/libcycles_kernel.a
      ${CMAKE_SOURCE_DIR}/lib/libcycles_util.a
      ${CMAKE_SOURCE_DIR}/lib/libcycles_subd.a
      ${CMAKE_SOURCE_DIR}/lib/libbf_intern_guardedalloc.a # For MEM_* functions
      TBB::tbb  # For parallel execution
      CUDA::cudart  # For CUDA runtime
      CUDA::cuda_driver
      CUDA::nvrtc
  )
endif()


# Add Blender's include directories for memory allocation functions
target_include_directories(sep_blender PRIVATE
    ${BLENDER_INCLUDE_DIRS}
    ${BLENDER_INCLUDE_DIRS}/blender
    ${BLENDER_INCLUDE_DIRS}/blender/makesdna
    ${BLENDER_INCLUDE_DIRS}/blender/blenlib
)

# Set library properties
set_target_properties(sep_blender PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Include directories
target_include_directories(sep_blender
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/glm
        ${CMAKE_SOURCE_DIR}/extern/eigen
)
if(SEP_HAS_CYCLES)
  target_include_directories(sep_blender PUBLIC
      ${CMAKE_SOURCE_DIR}/extern/cycles/src
  )
endif()
