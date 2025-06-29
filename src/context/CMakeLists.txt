# src/context/CMakeLists.txt

# Create a placeholder for context library
# This can be expanded later when context module is implemented
add_library(sep_context INTERFACE)

target_include_directories(sep_context INTERFACE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(sep_context INTERFACE
    sep_quantum
    sep_memory
    sep_core
)