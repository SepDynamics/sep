add_library(sep_memory STATIC memory_tier.cpp memory_tier_manager.cpp manager.cpp redis_manager.cpp)

target_include_directories(sep_memory
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
