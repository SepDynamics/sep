add_executable(pattern_metric_example pattern_metric_example.cpp)

target_link_libraries(pattern_metric_example PRIVATE
    sep_quantum
    sep_core
    sep_compat
    fmt::fmt
    benchmark::benchmark
    TBB::tbb
)

add_executable(financial_demo financial_demo.cpp)

target_link_libraries(financial_demo PRIVATE
    sep_quantum
    sep_core
    sep_compat
    fmt::fmt
    TBB::tbb
)
add_subdirectory(sep_memory_verifier)