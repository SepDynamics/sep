

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/quantum/
        DESTINATION include/sep/quantum
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/quantum/
        DESTINATION include/sep/quantum/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


add_library(sep_quantum STATIC
    evolution.cpp
    quantum_processor_qfh.cpp
    qbsa_qfh.cpp
    qbsa.cpp
    qfh.cpp
    quantum_processor_qfh_common.cpp
    manifold_config.cpp
    types_serialization.cpp
    pattern_processor.cpp
    pattern_evolution_bridge.cpp
    quantum_processor.cpp
    pattern_processor_interface.cpp
    pattern_evolution.cpp
    processor.cpp
    manifold_config.cpp
    quantum_manifold_optimizer.cpp
)

target_include_directories(sep_quantum
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

target_link_libraries(sep_quantum
    PUBLIC
        sep_manifold
        sep_core
        sep_compat
)

# Enable CUDA and handle exception specs
target_compile_definitions(sep_quantum PRIVATE
    SEP_HAS_CUDA=1
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
    SEP_BUILD_QUANTUM=1)

# Set library properties
set_target_properties(sep_quantum PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)
