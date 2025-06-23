add_library(sep_compat STATIC core/stream.cpp core/core.cu raii.cpp)

# Explicitly expose the project's include directories to this target. This
# avoids issues where tools (linters, IDEs) fail to resolve headers such as
# "compat/raii.h" when they don't pick up the global include directories.
target_include_directories(sep_compat PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/crow/include
    ${CMAKE_SOURCE_DIR}/third_party/glm
    ${CMAKE_SOURCE_DIR}/third_party/tbb
)
