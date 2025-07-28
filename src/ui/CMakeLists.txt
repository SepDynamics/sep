add_library(sep_ui INTERFACE)

# Expose header-only UI helpers

target_include_directories(sep_ui INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
