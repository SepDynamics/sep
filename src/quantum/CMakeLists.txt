add_library(sep_quantum STATIC
    evolution.cpp
    pattern_evolution.cpp
    quantum_processor_qfh.cpp
    qbsa_qfh.cpp
    qbsa.cpp
    qfh.cpp
    quantum_processor_qfh_common.cpp
    types_serialization.cpp
    pattern_processor.cpp
    pattern_processor_interface.cpp
    processor.cpp
    quantum_manifold_optimizer.cpp
    pattern_evolution_bridge.cpp
    quantum_processor.cpp
)

# Link dependencies
target_link_libraries(sep_quantum
    PUBLIC
        sep_core
    PRIVATE
        sep_compat # Link compat once, it handles CUDA internally
)

# Configure basic properties
set_target_properties(sep_quantum PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

# Configure CUDA options if available
if(SEP_CUDA_AVAILABLE)
    target_compile_definitions(sep_quantum PRIVATE SEP_HAS_CUDA=1)
    # CUDA includes and device symbols are handled by sep_compat
endif()

target_include_directories(sep_quantum
    PUBLIC
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

target_sources(sep_quantum PRIVATE
  
)

# Link against Eigen if available
find_package(Eigen3 QUIET)
if(Eigen3_FOUND)
    target_link_libraries(sep_quantum PRIVATE Eigen3::Eigen)
    target_compile_definitions(sep_quantum PRIVATE -DHAS_EIGEN)
else()
    # Use bundled Eigen
    target_include_directories(sep_quantum PRIVATE ${CMAKE_SOURCE_DIR}/extern/eigen)
endif()
