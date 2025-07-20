set(SEP_CORE_SOURCES
    tracing.cpp
    allocation_metrics.cpp
    dag_graph.cpp
    prometheus_exporter.cpp
    error_handler.cpp
    metrics_collector.cpp
    logging.cpp
    data_parser.cpp
    dag_graph_quant.cpp
)

# Manager functionality is excluded when building the testbed stubs
if(NOT SEP_TESTBED_STUBS)
    list(APPEND SEP_CORE_SOURCES manager.cpp)
endif()

# Create the static core library
add_library(sep_core STATIC ${SEP_CORE_SOURCES})

# The core library does not require Redis for these tests. Explicitly
# disable Redis support so headers guarded by SEP_NO_REDIS do not
# trigger missing include errors when building in minimal mode.
set(SEP_HAS_REDIS 0)
add_compile_definitions(SEP_NO_REDIS)

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
    $<$<BOOL:${SEP_BUILD_QUANTUM}>:SEP_BUILD_QUANTUM=1>
    $<$<NOT:$<BOOL:${SEP_BUILD_QUANTUM}>>:SEP_BUILD_QUANTUM=0>)

# Set library properties
set_target_properties(sep_core PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)

target_include_directories(sep_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include/sep/core>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/utils>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/third_party/glm>
)

# Add the necessary subdirectories to the public interface of sep_core
target_include_directories(sep_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/api>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/compat>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/core>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/crow>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/gpu>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/memory>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/quantum>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/tests>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/utils>
)
target_link_libraries(sep_core
    PUBLIC
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
        glm::glm
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
