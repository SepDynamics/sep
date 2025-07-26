add_library(sep_engine STATIC
    allocation_metrics.cpp
    compression.cpp
    dag_graph.cpp
    dag_graph_quant.cpp
    data_parser.cpp
    error_handler.cpp
    file_system_watcher.cpp
    logging.cpp
    manager.cpp
    metrics_collector.cpp
    prometheus_exporter.cpp
    socket_listener.cpp
    tracing.cpp
)

target_sources(sep_engine PRIVATE
    engine.cu
    cuda_impl.cu
    raii.cu
    stream.cu
    pattern_kernels.cu
    cuda_api.cu
    event.cu
    utils.cu
    kernels.cu
    core.cu
    quantum_kernels.cu
    memory.cu
)

target_include_directories(sep_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/common
)

if(MSVC)
    target_compile_options(sep_engine PRIVATE /FI"${CMAKE_CURRENT_SOURCE_DIR}/json_config.h")
else()
    target_compile_options(sep_engine PRIVATE -include "${CMAKE_CURRENT_SOURCE_DIR}/json_config.h")
endif()

target_link_libraries(sep_engine PUBLIC sep_memory sep_quantum)

