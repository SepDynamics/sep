add_library(sep_core STATIC
    tracing.cpp
    allocation_metrics.cpp
    dag_graph.cpp
    prometheus_exporter.cpp
    error_handler.cpp
    metrics_collector.cpp
    logging.cpp
    error_handler.cpp
    compression.cpp
    dag_graph.cpp
)

if(SEP_BUILD_ENGINE)
    target_sources(sep_core PRIVATE engine.cpp)
endif()



# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/core/
        DESTINATION include/sep/core
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/
        DESTINATION include/sep/core/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


# Enable CUDA and handle exception specs
target_compile_definitions(sep_core PRIVATE
    SEP_HAS_CUDA=1
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
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/glm>
)

target_link_libraries(sep_core
    PUBLIC
        Threads::Threads
        sep_compat
)

# Install headers from include directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/core/
        DESTINATION include/sep/core
        FILES_MATCHING PATTERN "*.h")

# Install compat headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/compat/
        DESTINATION include/sep/compat
        FILES_MATCHING PATTERN "*.h")

# Install headers from src directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/core/
        DESTINATION include/sep/core/internal
        FILES_MATCHING PATTERN "*.h")
