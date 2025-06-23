add_library(sep_core STATIC allocation_metrics.cpp dag_graph.cpp engine.cpp error_handler.cpp manager.cpp metrics_collector.cpp prometheus_exporter.cpp tracing.cpp)

target_include_directories(sep_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
