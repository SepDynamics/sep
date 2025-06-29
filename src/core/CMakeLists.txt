add_library(sep_core STATIC allocation_metrics.cpp dag_graph.cpp engine.cpp error_handler.cpp logging.cpp manager.cpp metrics_collector.cpp prometheus_exporter.cpp tracing.cpp)

target_include_directories(sep_core
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
        ${CMAKE_SOURCE_DIR}/extern/nlohmann
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)
