add_library(sep_quantum STATIC qbsa.cpp qbsa_qfh.cpp qfh.cpp quantum_processor.cpp quantum_processor_qfh.cpp quantum_processor_qfh_common.cpp evolution.cpp processor.cpp types_serialization.cpp pattern_processor.cpp pattern_processor_interface.cpp quantum_manifold_optimizer.cpp)

target_include_directories(sep_quantum
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
