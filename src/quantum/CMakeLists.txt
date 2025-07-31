add_subdirectory(bitspace)

add_sep_library(sep_quantum
    SOURCES
        pattern_evolution_bridge.cpp
        qbsa_qfh.cpp
        quantum_processor_qfh.cpp
        types_serialization.cpp
        pattern_evolution.cpp
        quantum_manifold_optimizer.cpp
        manifold_config.cpp
        pattern_processor_interface.cpp
        coherence_manager.cpp
        evolution.cpp
        pattern_metric_engine.cpp
        pattern_processor.cpp
        processor.cpp
        quantum_processor_qfh_common.cpp
        quantum_processor.cpp
    CUDA_SOURCES
        qbsa_cuda.cu
        quantum_processor_cuda.cu
    DEPENDENCIES
    sep_core_types
    sep_quantum_bitspace
    sep_memory
    sep_common
    fmt::fmt
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    CUDA::cudart
    CUDA::cuda_driver
    tbb
)

add_dependencies(sep_quantum sep_memory)

target_include_directories(sep_quantum PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
