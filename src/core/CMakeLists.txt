add_library(sep_core
  STATIC
    tracing.cpp
    allocation_metrics.cpp
    dag_graph.cpp
    prometheus_exporter.cpp
    error_handler.cpp
    manager.cpp
    metrics_collector.cpp
    engine.cpp
    logging.cpp
    error_handler.cpp
    dag_graph.cpp
)

# Enable CUDA and handle exception specs
target_compile_definitions(sep_core PRIVATE
    SEP_HAS_CUDA=1
    _GLIBCXX_USE_NOEXCEPT_SPEC=0
    SEP_CUDACC_DISABLE_EXCEPTION_SPEC_CHECKS=1)

# Set library properties
set_target_properties(sep_core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(sep_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

target_link_libraries(sep_core
    PUBLIC
        Threads::Threads
        sep_compat
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/
        DESTINATION include/sep/core
        FILES_MATCHING PATTERN "*.h")
