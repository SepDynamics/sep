add_library(crow INTERFACE)

# Configure Crow include directories
target_include_directories(crow INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add preprocessor definition for Crow
target_compile_definitions(crow INTERFACE CROW_ENABLE_SSL)
