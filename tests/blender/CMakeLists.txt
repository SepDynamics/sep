include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

if(NOT BUILD_TESTING)
    return()
endif()

# Find required packages
find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(CUDA QUIET)
pkg_check_modules(LZ4 REQUIRED liblz4)
pkg_check_modules(ZSTD REQUIRED libzstd)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)
# Try to find Google Benchmark
find_package(GoogleBenchmark QUIET)

# Option to enable/disable benchmarks
option(BUILD_BENCHMARKS "Build benchmark tests" ${GoogleBenchmark_FOUND})


# Add test executables
add_executable(blender_tests
    compression_test.cpp
    pattern_data_test.cpp
    api_test.cpp
    mesh_handler_test.cpp
    mesh_handler_perf_test.cpp
    mesh_handler_stress_test.cpp
    gpu_context_test.cpp
    gpu_context_perf_test.cpp
    scene_load_test.cpp
)

# Add benchmark executable if benchmarks are enabled
if(BUILD_BENCHMARKS)
    add_executable(blender_benchmarks
        mesh_handler_benchmark.cpp
    )
endif()

# Include directories
target_include_directories(blender_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/blender
        ${CMAKE_CURRENT_SOURCE_DIR}/include/pattern
        ${CMAKE_CURRENT_SOURCE_DIR}/include/compat
        ${CMAKE_CURRENT_SOURCE_DIR}/include/quantum
        ${CMAKE_CURRENT_SOURCE_DIR}/include/memory
        ${CMAKE_CURRENT_SOURCE_DIR}/include/context
        ${CMAKE_CURRENT_SOURCE_DIR}/include/core
        ${CMAKE_CURRENT_SOURCE_DIR}/include/api
        ${CMAKE_CURRENT_SOURCE_DIR}/include/persistence
        ${CMAKE_CURRENT_SOURCE_DIR}/include/dag
        ${CMAKE_CURRENT_SOURCE_DIR}/include/errors
        ${CMAKE_CURRENT_SOURCE_DIR}/include/logging
        ${CMAKE_CURRENT_SOURCE_DIR}/include/metrics
        ${CMAKE_CURRENT_SOURCE_DIR}/include/audio
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/pattern
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/memory
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/context
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/core
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/api
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/quantum
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/dag
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/audio
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/blender
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/compat
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/errors
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/logging
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sep/persistence
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PIPEWIRE_INCLUDE_DIRS}
        ${LZ4_INCLUDE_DIRS}
        ${ZSTD_INCLUDE_DIRS}
)
if(CUDA_FOUND)
    target_include_directories(blender_tests PRIVATE ${CUDA_INCLUDE_DIRS})
endif()

# Include directories for benchmarks if benchmarks are enabled
if(BUILD_BENCHMARKS)
    target_include_directories(blender_benchmarks
        PRIVATE
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/include
            ${PIPEWIRE_INCLUDE_DIRS}
            ${LZ4_INCLUDE_DIRS}
            ${ZSTD_INCLUDE_DIRS}
    )
    if(CUDA_FOUND)
        target_include_directories(blender_benchmarks PRIVATE ${CUDA_INCLUDE_DIRS})
    endif()
    
    # Link with GoogleBenchmark
    target_link_libraries(blender_benchmarks
        PRIVATE
            blender_core
            GoogleBenchmark::GoogleBenchmark
            ${PIPEWIRE_LIBRARIES}
            ${LZ4_LIBRARIES}
            ${ZSTD_LIBRARIES}
    )
    if(CUDA_FOUND)
        target_link_libraries(blender_benchmarks PRIVATE
            ${CUDA_LIBRARIES}
            ${CUDA_CUDA_LIBRARY}
            ${CUDA_CUDART_LIBRARY}
        )
    endif()
endif()

# Check for optional libraries
find_library(SEP_CORE_ENGINE_LIB sep_core_engine_lib PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(PATTERN_CUDA_LIB pattern_cuda PATHS ${CMAKE_SOURCE_DIR}/lib)

# Link dependencies
target_link_libraries(blender_tests
    PRIVATE
        blender_core
        GTest::GTest
        GTest::Main
        pthread
        ${PIPEWIRE_LIBRARIES}
        ${LZ4_LIBRARIES}
        ${ZSTD_LIBRARIES}
)
if(CUDA_FOUND)
    target_link_libraries(blender_tests PRIVATE
        ${CUDA_LIBRARIES}
        ${CUDA_CUDA_LIBRARY}
        ${CUDA_CUDART_LIBRARY}
    )
endif()

# Add optional libraries if found
# Find additional required libraries
find_library(MESH_HANDLER_LIB MeshHandler PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(PATTERN_COMPRESSION_LIB PatternCompression PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CUDA_WRAPPER_LIB sep_cuda_wrapper PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(METRICS_LIB sep_metrics PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(WRAPPER_FUNCTIONS_LIB sep_cuda_wrappers PATHS ${CMAKE_SOURCE_DIR}/lib)

# Mock implementations used only for the unit tests
set(TEST_MOCK_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_cuda_wrappers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_trace_span.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_pattern_compression.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_mesh_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_blender_bridge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks/mock_process.cpp
)
target_sources(blender_tests PRIVATE ${TEST_MOCK_SOURCES})

# Create a list of optional libraries
set(OPTIONAL_LIBS "")
if(SEP_CORE_ENGINE_LIB)
    list(APPEND OPTIONAL_LIBS ${SEP_CORE_ENGINE_LIB})
endif()
if(PATTERN_CUDA_LIB)
    list(APPEND OPTIONAL_LIBS ${PATTERN_CUDA_LIB})
endif()
if(MESH_HANDLER_LIB)
    list(APPEND OPTIONAL_LIBS ${MESH_HANDLER_LIB})
endif()
if(PATTERN_COMPRESSION_LIB)
    list(APPEND OPTIONAL_LIBS ${PATTERN_COMPRESSION_LIB})
endif()
if(CUDA_WRAPPER_LIB)
    list(APPEND OPTIONAL_LIBS ${CUDA_WRAPPER_LIB})
endif()
if(METRICS_LIB)
    list(APPEND OPTIONAL_LIBS ${METRICS_LIB})
endif()
if(WRAPPER_FUNCTIONS_LIB)
    list(APPEND OPTIONAL_LIBS ${WRAPPER_FUNCTIONS_LIB})
endif()

# Add all optional libraries to the target
target_link_libraries(blender_tests
    PRIVATE
    m  # For math functions
)

# Exclude the real process.cpp from sep_pattern when building tests
target_compile_definitions(blender_tests PRIVATE SEP_EXCLUDE_PROCESS=1)

# Now add the optional libraries
target_link_libraries(blender_tests
    PRIVATE
    ${OPTIONAL_LIBS}
)

# Create a directory for mock implementations
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mocks)

# Add a special target that only runs tests that don't require missing libraries
add_custom_target(run_available_tests
    COMMAND ${CMAKE_CTEST_COMMAND} -E "(MeshHandler|PatternCompression|CudaWrapper|GPUContext)"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests that don't require missing libraries"
)


# Link dependencies for benchmarks if benchmarks are enabled
if(BUILD_BENCHMARKS)
    target_link_libraries(blender_benchmarks
        PRIVATE
            blender_core
            benchmark::benchmark
            benchmark::benchmark_main
            pthread
            ${PIPEWIRE_LIBRARIES}
            ${LZ4_LIBRARIES}
            ${ZSTD_LIBRARIES}
            sep_core_engine_lib
            sep_cuda
            pattern_cuda
            sep_api
            m  # For math functions
    )
    if(CUDA_FOUND)
        target_link_libraries(blender_benchmarks PRIVATE
            ${CUDA_LIBRARIES}
            ${CUDA_CUDA_LIBRARY}
            ${CUDA_CUDART_LIBRARY}
        )
    endif()
endif()

target_link_directories(blender_tests
    PRIVATE
        ${PIPEWIRE_LIBRARY_DIRS}
        ${LZ4_LIBRARY_DIRS}
        ${ZSTD_LIBRARY_DIRS}
)
if(CUDA_FOUND)
    target_link_directories(blender_tests PRIVATE ${CUDA_LIBRARY_DIRS})
endif()

if(BUILD_BENCHMARKS)
    target_link_directories(blender_benchmarks
        PRIVATE
            ${PIPEWIRE_LIBRARY_DIRS}
            ${LZ4_LIBRARY_DIRS}
            ${ZSTD_LIBRARY_DIRS}
    )
    if(CUDA_FOUND)
        target_link_directories(blender_benchmarks PRIVATE ${CUDA_LIBRARY_DIRS})
    endif()
endif()

# Add compile definitions
target_compile_definitions(blender_tests
    PRIVATE
        TESTING=1
        SEP_RANDOMIZE=1
        _USE_MATH_DEFINES  # For M_PI
        SCENE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/scenes"
        GTEST_HAS_MAIN=1  # Ensure GTest knows we're using its main
)

# Add compile definitions for benchmarks if benchmarks are enabled
if(BUILD_BENCHMARKS)
    target_compile_definitions(blender_benchmarks
        PRIVATE
            TESTING=1
            SEP_RANDOMIZE=1
            _USE_MATH_DEFINES  # For M_PI
            SCENE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/scenes"
    )
endif()

# Enable OpenMP for parallel tests
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(blender_tests PRIVATE OpenMP::OpenMP_CXX)
    if(BUILD_BENCHMARKS)
        target_link_libraries(blender_benchmarks PRIVATE OpenMP::OpenMP_CXX)
    endif()
endif()

# Enable c++20 features
set_target_properties(blender_tests PROPERTIES
)
if(BUILD_BENCHMARKS)
    set_target_properties(blender_benchmarks PROPERTIES
    )
endif()

# Test Categories
## Functionality Tests

add_test(
    NAME compression_tests
    COMMAND blender_tests --gtest_filter="CompressionTest.*"
)

add_test(
    NAME pattern_data_tests
    COMMAND blender_tests --gtest_filter="PatternDataTest.*"
)

add_test(
    NAME api_tests
    COMMAND blender_tests --gtest_filter="APITest.*"
)

## Mesh Handling Tests
add_test(
    NAME mesh_handler_tests
    COMMAND blender_tests --gtest_filter="MeshHandlerTest.*"
)

## Performance Tests
add_test(
    NAME mesh_handler_perf_tests
    COMMAND blender_tests --gtest_filter="MeshHandlerPerfTest.*"
)

## Stress Tests
add_test(
    NAME mesh_handler_stress_tests
    COMMAND blender_tests --gtest_filter="MeshHandlerStressTest.*"
)

## GPU Tests
add_test(
    NAME gpu_context_tests
    COMMAND blender_tests --gtest_filter="GPUContextTest.*"
)

add_test(
    NAME gpu_context_perf_tests
    COMMAND blender_tests --gtest_filter="GPUContextPerfTest.*"
)

# Test Properties
## Functionality Tests Properties
set_tests_properties(
    compression_tests
    pattern_data_tests
    api_tests
    PROPERTIES
    LABELS "functionality"
    ENVIRONMENT "GTEST_COLOR=1"
    TIMEOUT 60
)

## Mesh Tests Properties
set_tests_properties(
    mesh_handler_tests
    PROPERTIES
        LABELS "mesh"
    ENVIRONMENT "GTEST_COLOR=1"
    TIMEOUT 60
)

## Performance Tests Properties
set_tests_properties(
    mesh_handler_perf_tests
    PROPERTIES
    LABELS "performance"
    ENVIRONMENT "GTEST_COLOR=1"
    TIMEOUT 300
    PROCESSORS 4
)

## Stress Tests Properties
set_tests_properties(
    mesh_handler_stress_tests
    PROPERTIES
    LABELS "stress"
    ENVIRONMENT "GTEST_COLOR=1;OMP_NUM_THREADS=8"
    TIMEOUT 600
    PROCESSORS 8
    RESOURCE_LOCK cpu_intensive
)

## GPU Tests Properties
set_tests_properties(
    gpu_context_tests
    gpu_context_perf_tests
    PROPERTIES
    LABELS "gpu"
    ENVIRONMENT "GTEST_COLOR=1;CUDA_VISIBLE_DEVICES=0"
    TIMEOUT 300
    PROCESSORS 1
    RESOURCE_LOCK gpu
)

# Install test executable (optional)
install(TARGETS blender_tests
    RUNTIME DESTINATION bin/tests
    OPTIONAL
)

# Install benchmark executable (optional) if benchmarks are enabled
if(BUILD_BENCHMARKS)
    install(TARGETS blender_benchmarks
        RUNTIME DESTINATION bin/benchmarks
        OPTIONAL
    )
endif()

# Create test data directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/test_data)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scenes
     DESTINATION ${CMAKE_BINARY_DIR}/test_data)

# Add sanitizers in debug mode (except for stress and gpu tests)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(blender_tests
        PRIVATE
            $<$<NOT:$<OR:$<STREQUAL:$<TARGET_PROPERTY:LABELS>,stress>,$<STREQUAL:$<TARGET_PROPERTY:LABELS>,gpu>>>:
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
            >
    )
    target_link_options(blender_tests
        PRIVATE
            $<$<NOT:$<OR:$<STREQUAL:$<TARGET_PROPERTY:LABELS>,stress>,$<STREQUAL:$<TARGET_PROPERTY:LABELS>,gpu>>>:
                -fsanitize=address,undefined
            >
    )
endif()

# Add coverage if enabled
if(ENABLE_COVERAGE)
    target_compile_options(blender_tests PRIVATE --coverage)
    target_link_options(blender_tests PRIVATE --coverage)
endif()

# Add custom targets for test categories
add_custom_target(test_functionality
    COMMAND ${CMAKE_CTEST_COMMAND} -L functionality --output-on-failure
    DEPENDS blender_tests
)

add_custom_target(test_audio
    COMMAND ${CMAKE_CTEST_COMMAND} -L audio --output-on-failure
    DEPENDS blender_tests
)

add_custom_target(test_mesh
    COMMAND ${CMAKE_CTEST_COMMAND} -L mesh --output-on-failure
    DEPENDS blender_tests
)

add_custom_target(test_performance
    COMMAND ${CMAKE_CTEST_COMMAND} -L performance --output-on-failure
    DEPENDS blender_tests
)

add_custom_target(test_stress
    COMMAND ${CMAKE_CTEST_COMMAND} -L stress --output-on-failure
    DEPENDS blender_tests
)

add_custom_target(test_gpu
    COMMAND ${CMAKE_CTEST_COMMAND} -L gpu --output-on-failure
    DEPENDS blender_tests
)

add_custom_target(test_all
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS blender_tests
)

# Generate test report
add_custom_target(test_report
    COMMAND ${CMAKE_COMMAND} -E echo "Generating test report..."
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --output-junit test_report.xml
    DEPENDS blender_tests
)
