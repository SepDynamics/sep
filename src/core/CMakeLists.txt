add_library(sep_core STATIC allocation_metrics.cpp dag_graph.cpp engine.cpp error_handler.cpp manager.cpp metrics_collector.cpp prometheus_exporter.cpp tracing.cpp)

target_include_directories(sep_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Link against the compat library so that CUDA wrappers
# and other compatibility utilities are available when
# building the core library. This resolves linker errors
# for symbols defined in the compat module (e.g. CudaCore).
target_link_libraries(sep_core PUBLIC sep_compat)
