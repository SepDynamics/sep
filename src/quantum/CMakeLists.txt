add_library(sep_quantum STATIC
        evolution.cpp
    quantum_processor_qfh.cpp
    qbsa_qfh.cpp
    qbsa.cpp
    qfh.cpp
    quantum_processor_qfh_common.cpp
    types_serialization.cpp
    pattern_processor.cpp
    pattern_evolution_bridge.cpp
    quantum_processor.cpp
    pattern_processor_interface.cpp
    pattern_evolution.cpp
    processor.cpp
    quantum_manifold_optimizer.cpp
)

target_include_directories(sep_quantum
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(sep_quantum
    PUBLIC
        sep_core
        sep_compat
)

# Export include directories for dependent targets
target_include_directories(sep_quantum PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# CUDA is required for quantum operations
target_compile_definitions(sep_quantum PRIVATE SEP_USE_CUDA=1)

# Set library properties
set_target_properties(sep_quantum PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)
