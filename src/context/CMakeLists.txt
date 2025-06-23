add_library(sep_context STATIC processor.cpp relationship.cpp)

target_include_directories(sep_context
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
