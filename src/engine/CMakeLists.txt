add_sep_library(sep_engine
    SOURCES
        facade/facade.cpp
        internal/allocation_metrics.cpp
        internal/compression.cpp
        internal/dag_graph.cpp
        internal/dag_graph_quant.cpp
        internal/data_parser.cpp
        internal/error_handler.cpp
        internal/file_system_watcher.cpp
        internal/logging.cpp
        internal/manager.cpp
        internal/metrics_collector.cpp
        internal/prometheus_exporter.cpp
        internal/socket_listener.cpp
        internal/tracing.cpp
        internal/engine.cpp
    CUDA_SOURCES
        internal/cuda_impl.cu
        internal/raii.cu
        internal/stream.cu
        internal/pattern_kernels.cu
        internal/cuda_api.cu
        internal/event.cu
        internal/utils.cu
        internal/kernels.cu
        internal/core.cu
        internal/quantum_kernels.cu
        internal/memory.cu
    DEPENDENCIES
        sep_core_types
        sep_common
        sep_connectors
        sep_memory
        sep_quantum
        CUDA::cudart
        CUDA::cuda_driver
)

target_include_directories(sep_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/facade
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/internal
    ${CMAKE_SOURCE_DIR}/src/common
)

# json_config.h can be included manually in source files that need it
