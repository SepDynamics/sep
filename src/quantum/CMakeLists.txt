add_library(sep_quantum STATIC
    qbsa.cpp
    qbsa_qfh.cpp
    qfh.cpp
    quantum_processor.cpp
    quantum_processor_qfh.cpp
    quantum_processor_qfh_common.cpp
    evolution.cpp
    processor.cpp
    types_serialization.cpp
    pattern_processor.cpp
    pattern_processor_interface.cpp
    quantum_manifold_optimizer.cpp
)
target_include_directories(sep_quantum
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
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
