add_library(blender_tests STATIC
    api_test.cpp
    compression_test.cpp
    gpu_context_perf_test.cpp
    gpu_context_test.cpp
    mesh_handler_benchmark.cpp
    mesh_handler_perf_test.cpp
    mesh_handler_stress_test.cpp
    mesh_handler_test.cpp
    feature_flags_test.cpp
    pattern_data_test.cpp
    scene_load_test.cpp
    shader_test.cpp
    gpu_context_integration_test.cpp
)

target_include_directories(blender_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/extern/pipewire/spa/include
    ${CMAKE_SOURCE_DIR}/extern/blender/include
    ${CMAKE_SOURCE_DIR}/extern/cuda/include
    ${CMAKE_SOURCE_DIR}/include/api
    ${CMAKE_SOURCE_DIR}/include/audio
    ${CMAKE_SOURCE_DIR}/include/blender
    ${CMAKE_SOURCE_DIR}/include/compat
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/crow
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/quantum
    ${CMAKE_SOURCE_DIR}/include/quantum/api
    ${CMAKE_SOURCE_DIR}/include/quantum/audio
    ${CMAKE_SOURCE_DIR}/include/quantum/blender
    ${CMAKE_SOURCE_DIR}/include/quantum/compat
    ${CMAKE_SOURCE_DIR}/include/quantum/core
    ${CMAKE_SOURCE_DIR}/include/quantum/crow
    ${CMAKE_SOURCE_DIR}/include/quantum/memory
    ${CMAKE_SOURCE_DIR}/include/quantum/quantum
)

target_link_libraries(blender_tests PRIVATE
    GTest::gtest
    GTest::gmock
    sep_core
    sep_blender
    sep_quantum
    sep_memory
    sep_compat
)

target_compile_definitions(blender_tests PRIVATE
    GTEST_LINKED_AS_SHARED_LIBRARY=1
    SEP_TEST_BUILD=1
    SEP_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/test_data"
)

add_test(NAME blender_tests COMMAND blender_tests)
