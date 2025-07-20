# Find CUDA if enabled
if(SEP_USE_CUDA)
    find_package(CUDAToolkit REQUIRED)
endif()



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
    pattern_metric_engine.cpp
)

target_include_directories(sep_quantum
    PUBLIC
        ${CMAKE_SOURCE_DIR}/src
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add CUDA include directories if CUDA is enabled
if(SEP_USE_CUDA)
    target_include_directories(sep_quantum
        PUBLIC
            ${CUDAToolkit_INCLUDE_DIRS}
    )
endif()

target_link_libraries(sep_quantum
    PUBLIC
        sep_engine
)

# Set modern CUDA policy
# cmake_policy(SET CMP0146 NEW)

# Enable CUDA and handle exception specs
target_compile_definitions(sep_quantum PRIVATE
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1
    SEP_BUILD_QUANTUM=1
    SEP_ENABLE_CUDA=1
    SEP_ENGINE_HAS_CUDA=1)

# Add CUDA files
target_sources(sep_quantum PRIVATE
    qbsa_cuda.cu
    quantum_processor_cuda.cu)

# Set CUDA architecture
set_property(TARGET sep_quantum PROPERTY CUDA_ARCHITECTURES 70 75 80)

# Link CUDA runtime
if(SEP_USE_CUDA)
    target_link_libraries(sep_quantum PRIVATE
        ${CUDA_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
        ${CUDA_CUFFT_LIBRARIES})
endif()

# Set library properties
set_target_properties(sep_quantum PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
