# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/tests/
        DESTINATION src/tests
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

# Find GoogleTest (includes GMock in modern versions)
find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()
        
add_library(sep_tests STATIC
    memory_tier_test.cpp
    simple_embedding_model.cpp
    dataset_loader.cpp
    ingestion_test.cpp
    pattern_metric_engine_test.cpp)

# Set up include directories for sep_tests
target_include_directories(sep_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Link dependencies for sep_tests
target_link_libraries(sep_tests PRIVATE
    GTest::gtest
    fmt::fmt
    glm::glm
    nlohmann_json::nlohmann_json
)

# Set C++ standard
# Find required packages
find_package(CUDAToolkit REQUIRED)
find_package(fmt REQUIRED)

# Create test executables
# add_executable(memory_tier_test memory_tier_test.cpp)
add_executable(pattern_metric_test pattern_metric_engine_test.cpp)

# Add CUDA test if CUDA is enabled
if(SEP_USE_CUDA)
    add_executable(cuda_test cuda_test.cu)
    set_target_properties(cuda_test PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON
        LINKER_LANGUAGE CXX
    )
    # Add CUDA flags to avoid sin/cos issues
    target_compile_options(cuda_test PRIVATE
        $<$<COMPILE_LANGUAGE:CUDA>:
            --expt-relaxed-constexpr
            --extended-lambda
            -Xcompiler -fPIC
        >
    )
    target_link_libraries(cuda_test PRIVATE
        ${CUDA_LIBRARIES}
        ${CUDA_CUDART_LIBRARY}
    )
    add_test(NAME cuda_test COMMAND cuda_test)
    set_tests_properties(cuda_test PROPERTIES
        LABELS "unit;cuda"
    )
endif()

# Link dependencies
# target_link_libraries(cycles_test PRIVATE
#     GTest::gtest
#     GTest::gtest_main
#     GTest::gmock
#     fmt::fmt
#     sep_core
#     sep_compat
#     CUDA::cudart
#     CUDA::cuda_driver
# )

# target_link_libraries(memory_tier_test PRIVATE
#     GTest::gtest
#     GTest::gtest_main
#     GTest::gmock
#     fmt::fmt
#     sep_core
#     sep_memory
#     sep_compat
# )

# # Set up include directories for memory test
# target_include_directories(memory_tier_test PRIVATE
#     ${CMAKE_SOURCE_DIR}
#     ${CMAKE_SOURCE_DIR}/src
# )


# add_test(NAME memory_tier_test COMMAND memory_tier_test)
# set_tests_properties(memory_tier_test PROPERTIES
#     LABELS "unit;memory"
# )

# Configure pattern metric test
target_link_libraries(pattern_metric_test PRIVATE
    GTest::gtest
    GTest::gtest_main
    fmt::fmt
    sep_engine
    sep_quantum
    TBB::tbb
)

target_include_directories(pattern_metric_test PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

add_test(NAME pattern_metric_test COMMAND pattern_metric_test)
set_tests_properties(pattern_metric_test PROPERTIES
    LABELS "unit;quantum;pattern"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
