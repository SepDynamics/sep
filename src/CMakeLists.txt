# Use our custom CUDA module instead of CMake's built-in CUDA support
# The custom module is included in the top-level CMakeLists.txt

# Add modular build targets in dependency order
add_subdirectory(config)
add_subdirectory(metrics)
add_subdirectory(dag)
add_subdirectory(memory)
add_subdirectory(context)
add_subdirectory(quantum)
add_subdirectory(core)
add_subdirectory(api)
add_subdirectory(cuda)
add_subdirectory(blender)

# Main executable

# No need for CUDA-specific compile options as they're handled by our custom module

# Install headers
install(DIRECTORY include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h*"
)

# Install source
install(DIRECTORY src/
  DESTINATION src
  FILES_MATCHING PATTERN "*.c*"
)
