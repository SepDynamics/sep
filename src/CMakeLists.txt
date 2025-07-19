
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)


# Configure include paths for all targets
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
)

# Add minimal build configuration
if(SEP_MINIMAL)
    add_definitions(-DSEP_MINIMAL=1)
endif()

# Add component subdirectories - only core components needed for quant processing
add_subdirectory(core)
add_subdirectory(compat)
# add_subdirectory(embeddings)
add_subdirectory(memory)
add_subdirectory(gpu)
add_subdirectory(quantum)

# Add tests subdirectory
add_subdirectory(tests)



