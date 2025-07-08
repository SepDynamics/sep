add_executable(memory_minimal_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../tests/memory/memory_tier_manager_test.cpp
)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../src)

set(MEMORY_SRC
    ${SRC_DIR}/memory/memory_tier.cpp
    ${SRC_DIR}/memory/memory_tier_manager.cpp
)

target_sources(memory_minimal_tests PRIVATE ${MEMORY_SRC})

target_include_directories(memory_minimal_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
)

target_link_libraries(memory_minimal_tests PRIVATE
    GTest::GTest
    GTest::Main
)

add_test(NAME memory_minimal_tests COMMAND memory_minimal_tests)
