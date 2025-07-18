add_executable(pattern_metric_example pattern_metric_example.cpp)

target_link_libraries(pattern_metric_example PRIVATE
    sep_quantum
    sep_core
    sep_compat
    fmt::fmt
)