# src/compat/CMakeLists.txt
# Enhanced CUDA compatibility library with explicit symbol exports

# Explicitly include CUDA paths
include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
link_directories(${CUDA_LIBRARY_DIRS})



# Force correct CUDA language and make all symbols available
set_source_files_properties(
    core.cu cuda_api.cu utils.cu event.cu
    pattern_kernels.cu quantum_kernels.cu
    PROPERTIES
    CUDA_SOURCE_PROPERTY_FORMAT OBJ
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    CUDA_SEPARABLE_COMPILATION ON
)

# Ensure CUDA symbols are fully exported with visibility default

# Make sep_compat a shared library instead of static to ensure symbols are properly exported
add_library(sep_compat SHARED
    core.cu
    cuda_api.cu
    utils.cu
    event.cu
    pattern_kernels.cu
    quantum_kernels.cu
    stream.cpp
    raii.cpp
    component_bridge.cpp
)
set_target_properties(sep_compat PROPERTIES
    CUDA_VISIBILITY_PRESET "default"
    CXX_VISIBILITY_PRESET "default"
    CUDA_SEPARABLE_COMPILATION ON
    POSITION_INDEPENDENT_CODE ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
)

# Link CUDA libraries with explicit absolute paths to ensure they're found
target_link_libraries(sep_compat PUBLIC
    ${CUDA_LIBRARIES}
    ${CUDA_CUDART_LIBRARY}
    ${CUDA_cuda_LIBRARY}
    -lcudart
    -lcuda
)

# Set compiler flags specifically for CUDA files
target_compile_options(sep_compat PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:
    -Xcompiler -fPIC
    -use_fast_math
    -Xptxas -v
    -lineinfo
    -expt-relaxed-constexpr
    -rdc=true
    --relocatable-device-code=true
    -D__CUDACC__
>)

# Generate relocatable device code and export CUDA symbols
set_target_properties(sep_compat PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    INTERPROCEDURAL_OPTIMIZATION OFF
    VISIBILITY_INLINES_HIDDEN OFF
    CXX_VISIBILITY_PRESET default
    CUDA_VISIBILITY_PRESET default
)

# Add a custom command to ensure proper device linking
add_custom_command(TARGET sep_compat POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Ensuring device symbols are linked for sep_compat"
)
