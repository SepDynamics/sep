add_sep_library(sep_memory
    SOURCES
        memory_tier_manager_serialization.cpp
        memory_tier.cpp
        memory_tier_manager.cpp
        quantum_coherence_manager.cpp
        redis_manager.cpp
    DEPENDENCIES
        sep_quantum
        fmt::fmt
        Threads::Threads
        TBB::tbb
)

target_include_directories(sep_memory PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/engine
    $<TARGET_PROPERTY:sep_engine,INTERFACE_INCLUDE_DIRECTORIES>
)