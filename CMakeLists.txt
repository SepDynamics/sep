cmake_minimum_required(VERSION 3.16)
project(SEPEngine LANGUAGES CXX CUDA)

# === Professional Project Configuration ===
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Module Path Configuration ===
list(APPEND CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_SOURCE_DIR}/cmake/modules
)

# === Compilation Definitions ===
add_compile_definitions(
    CROW_DISABLE_RTTI=1
    CCL_NAMESPACE_BEGIN=namespace\ ccl\ {
    CCL_NAMESPACE_END=}
)

# === Core Dependencies Discovery ===
find_package(CUDAToolkit REQUIRED)
find_package(CURL REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig QUIET)

# === Redis Integration ===
find_library(HIREDIS_LIBRARIES NAMES hiredis)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
if(HIREDIS_LIBRARIES AND HIREDIS_INCLUDE_DIR)
    set(SEP_HAS_HIREDIS 1)
    message(STATUS "Redis support: ENABLED")
else()
    set(SEP_HAS_HIREDIS 0)
    message(STATUS "Redis support: DISABLED")
endif()
add_compile_definitions(SEP_HAS_HIREDIS=${SEP_HAS_HIREDIS})

# === Blender/Cycles Configuration ===
set(SEP_HAS_BLENDER 1)
set(SEP_HAS_CYCLES 1)
add_compile_definitions(
    SEP_HAS_BLENDER=${SEP_HAS_BLENDER}
    SEP_HAS_CYCLES=${SEP_HAS_CYCLES}
)

# === Critical: Cycles Root Directory Setup ===
set(CYCLES_ROOT_DIR "${CMAKE_SOURCE_DIR}/extern/cycles")
if(NOT EXISTS "${CYCLES_ROOT_DIR}")
    message(FATAL_ERROR "Cycles not found at ${CYCLES_ROOT_DIR}")
endif()

# === Library Discovery Macro with Enhanced Diagnostics ===
macro(find_required_library var_name lib_names paths)
    find_library(${var_name} 
        NAMES ${lib_names}
        PATHS ${paths}
        PATH_SUFFIXES lib lib64
        NO_DEFAULT_PATH
    )
    if(${var_name})
        message(STATUS "[FOUND] ${var_name}: ${${var_name}}")
        get_filename_component(${var_name}_DIR ${${var_name}} DIRECTORY)
    else()
        message(WARNING "[MISSING] ${var_name} (${lib_names}) - Build may fail")
        set(${var_name} "")
    endif()
endmacro()

# === Comprehensive Cycles Library Discovery ===
set(CYCLES_LIB_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/lib
    ${CYCLES_ROOT_DIR}/lib
    ${CYCLES_ROOT_DIR}/build/lib
    /usr/local/lib/cycles
)

# Core Cycles libraries in dependency order
find_required_library(CYCLES_UTIL_LIBRARY "cycles_util libcycles_util.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_GRAPH_LIBRARY "cycles_graph libcycles_graph.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_KERNEL_LIBRARY "cycles_kernel libcycles_kernel.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_BVH_LIBRARY "cycles_bvh libcycles_bvh.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_SUBD_LIBRARY "cycles_subd libcycles_subd.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_DEVICE_LIBRARY "cycles_device libcycles_device.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_SCENE_LIBRARY "cycles_scene libcycles_scene.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_SESSION_LIBRARY "cycles_session libcycles_session.a" "${CYCLES_LIB_SEARCH_PATHS}")
find_required_library(CYCLES_INTEGRATOR_LIBRARY "cycles_integrator libcycles_integrator.a" "${CYCLES_LIB_SEARCH_PATHS}")

# OSL support (critical for resolving your error)
find_required_library(CYCLES_OSL_LIBRARY "cycles_osl libcycles_osl.a" "${CYCLES_LIB_SEARCH_PATHS}")

# === OSL Dependencies (System) ===
find_library(OSL_COMP_LIBRARY NAMES oslcomp PATHS /usr/lib64 /usr/local/lib)
find_library(OSL_EXEC_LIBRARY NAMES oslexec PATHS /usr/lib64 /usr/local/lib)
find_library(OSL_QUERY_LIBRARY NAMES oslquery PATHS /usr/lib64 /usr/local/lib)
find_path(OSL_INCLUDE_DIR OSL/oslcomp.h PATHS /usr/include /usr/local/include)

# === Blender Internal Libraries ===
set(BLENDER_LIB_PATHS ${CMAKE_SOURCE_DIR}/lib)
find_required_library(BF_INTERN_GUARDEDALLOC "bf_intern_guardedalloc" "${BLENDER_LIB_PATHS}")
find_required_library(BF_INTERN_ATOMIC "bf_intern_atomic" "${BLENDER_LIB_PATHS}")
find_required_library(BF_INTERN_MEMUTIL "bf_intern_memutil" "${BLENDER_LIB_PATHS}")
find_required_library(BF_INTERN_SKY "bf_intern_sky" "${BLENDER_LIB_PATHS}")
find_required_library(BF_INTERN_OPENSUBDIV "bf_intern_opensubdiv" "${BLENDER_LIB_PATHS}")
find_required_library(BF_BLENLIB "bf_blenlib" "${BLENDER_LIB_PATHS}")
find_required_library(BF_BLENKERNEL "bf_blenkernel" "${BLENDER_LIB_PATHS}")
find_required_library(BF_GPU "bf_gpu" "${BLENDER_LIB_PATHS}")
find_required_library(BF_DRAW "bf_draw" "${BLENDER_LIB_PATHS}")

# === External Wrapper Libraries ===
find_required_library(EXTERN_CUEW "extern_cuew" "${BLENDER_LIB_PATHS}")
find_required_library(EXTERN_HIPEW "extern_hipew" "${BLENDER_LIB_PATHS}")
find_required_library(EXTERN_CLEW "extern_clew" "${BLENDER_LIB_PATHS}")

# === Third-Party Dependencies ===
# OpenVDB and dependencies
find_package(OpenVDB QUIET)
if(NOT OpenVDB_FOUND)
    find_library(OPENVDB_LIBRARY openvdb)
    find_path(OPENVDB_INCLUDE_DIR openvdb/openvdb.h)
endif()

# TBB (multiple versions for compatibility)
find_library(TBB_LIBRARY NAMES tbb)
find_library(TBB_MALLOC_LIBRARY NAMES tbbmalloc)
find_library(TBB2_LIBRARY NAMES tbb.so.2 PATHS /usr/lib64 /usr/local/lib)

# OpenSubdiv
find_library(OPENSUBDIV_CPU_LIBRARY NAMES osdCPU)
find_library(OPENSUBDIV_GPU_LIBRARY NAMES osdGPU)

# Other critical dependencies
find_library(OPENIMAGEIO_LIBRARY OpenImageIO)
find_library(OPENIMAGEIO_UTIL_LIBRARY OpenImageIO_Util)
find_library(OPENCOLORIO_LIBRARY OpenColorIO)
find_library(OPENEXR_LIBRARY OpenEXR)
find_library(IMATH_LIBRARY Imath)
find_library(ALEMBIC_LIBRARY Alembic)
find_library(EMBREE_LIBRARY embree4)
find_library(GLOG_LIBRARY glog)
find_library(GFLAGS_LIBRARY gflags)
find_library(BLOSC_LIBRARY blosc)
find_library(ZSTD_LIBRARY zstd)
find_library(PUGIXML_LIBRARY pugixml)

# === Include Directories Configuration ===
set(SEP_INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${CYCLES_ROOT_DIR}/src
    ${OSL_INCLUDE_DIR}
    ${OPENVDB_INCLUDE_DIR}
    ${HIREDIS_INCLUDE_DIR}
)

include_directories(${SEP_INCLUDE_DIRS})

# === CRITICAL: Library Linking Order Resolution ===
# This order resolves cyclic dependencies by ensuring symbols are available when needed
set(CYCLES_LINK_ORDER
    # Level 1: Foundation (no Cycles dependencies)
    ${CYCLES_UTIL_LIBRARY}
    
    # Level 2: Core abstractions
    ${CYCLES_GRAPH_LIBRARY}
    
    # Level 3: Kernel and acceleration
    ${CYCLES_KERNEL_LIBRARY}
    ${CYCLES_BVH_LIBRARY}
    
    # Level 4: Subdivision and geometry
    ${CYCLES_SUBD_LIBRARY}
    
    # Level 5: Device abstraction
    ${CYCLES_DEVICE_LIBRARY}
    
    # Level 6: Scene management (depends on kernel, device)
    ${CYCLES_SCENE_LIBRARY}
    
    # Level 7: Integration and session
    ${CYCLES_INTEGRATOR_LIBRARY}
    ${CYCLES_SESSION_LIBRARY}
    
    # Level 8: OSL support (if available)
    ${CYCLES_OSL_LIBRARY}
)

# Blender libraries in dependency order
set(BLENDER_LINK_ORDER
    # Foundation libraries
    ${BF_INTERN_GUARDEDALLOC}
    ${BF_INTERN_ATOMIC}
    ${BF_INTERN_MEMUTIL}
    
    # Core libraries
    ${BF_BLENLIB}
    
    # Specialized libraries
    ${BF_INTERN_SKY}
    ${BF_INTERN_OPENSUBDIV}
    
    # Higher-level libraries
    ${BF_GPU}
    ${BF_BLENKERNEL}
    ${BF_DRAW}
)

# External dependencies in order
set(EXTERNAL_LINK_ORDER
    # CUDA/GPU wrappers
    ${EXTERN_CUEW}
    ${EXTERN_HIPEW}
    ${EXTERN_CLEW}
    
    # OSL libraries
    ${OSL_COMP_LIBRARY}
    ${OSL_EXEC_LIBRARY}
    ${OSL_QUERY_LIBRARY}
    
    # OpenSubdiv with its TBB dependency
    ${OPENSUBDIV_CPU_LIBRARY}
    ${OPENSUBDIV_GPU_LIBRARY}
    ${TBB2_LIBRARY}  # Specific version for OpenSubdiv
    
    # Core graphics libraries
    ${OPENIMAGEIO_LIBRARY}
    ${OPENIMAGEIO_UTIL_LIBRARY}
    ${OPENCOLORIO_LIBRARY}
    ${OPENEXR_LIBRARY}
    ${IMATH_LIBRARY}
    
    # Scene and geometry libraries
    ${ALEMBIC_LIBRARY}
    ${OPENVDB_LIBRARY}
    ${EMBREE_LIBRARY}
    
    # Utility libraries
    ${TBB_LIBRARY}
    ${TBB_MALLOC_LIBRARY}
    ${BLOSC_LIBRARY}
    ${ZSTD_LIBRARY}
    ${PUGIXML_LIBRARY}
    ${GLOG_LIBRARY}
    ${GFLAGS_LIBRARY}
    
    # System libraries
    ${CMAKE_DL_LIBS}
    m
    pthread
    z
)

# === Consolidated Link Libraries ===
set(SEP_ALL_LINK_LIBS
    ${CYCLES_LINK_ORDER}
    ${BLENDER_LINK_ORDER}
    ${EXTERNAL_LINK_ORDER}
)

# Remove empty entries
list(REMOVE_ITEM SEP_ALL_LINK_LIBS "")

# === Internal SEP Libraries ===
set(SEP_INTERNAL_LIBS
    sep_api
    sep_memory
    sep_quantum
    sep_core
    sep_audio
    sep_blender
    sep_compat
)

# === Add Subdirectories ===
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/tests)

# === Main Executable Configuration ===
add_executable(sep_engine src/main.cpp)

# Configure include directories
target_include_directories(sep_engine
    PUBLIC ${SEP_INCLUDE_DIRS}
)

# CRITICAL: Use WHOLE ARCHIVE for static Cycles libraries to ensure all symbols are included
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_libraries(sep_engine
        PRIVATE
        ${SEP_INTERNAL_LIBS}
        -Wl,--whole-archive
        ${CYCLES_LINK_ORDER}
        -Wl,--no-whole-archive
        ${BLENDER_LINK_ORDER}
        ${EXTERNAL_LINK_ORDER}
        ${CURL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        CUDAToolkit::cudart
        CUDAToolkit::cuda_driver
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
    )
else()
    # Fallback for other compilers
    target_link_libraries(sep_engine
        PRIVATE
        ${SEP_INTERNAL_LIBS}
        ${SEP_ALL_LINK_LIBS}
        ${CURL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        CUDAToolkit::cudart
        CUDAToolkit::cuda_driver
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
    )
endif()

# === Installation Configuration ===
install(TARGETS sep_engine
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# === Build Summary ===
message(STATUS "=== SEP Engine Build Configuration ===")
message(STATUS "Cycles support: ${SEP_HAS_CYCLES}")
message(STATUS "Blender support: ${SEP_HAS_BLENDER}")
message(STATUS "Redis support: ${SEP_HAS_HIREDIS}")
message(STATUS "Cycles root: ${CYCLES_ROOT_DIR}")
message(STATUS "OSL libraries: ${OSL_COMP_LIBRARY} ${OSL_EXEC_LIBRARY} ${OSL_QUERY_LIBRARY}")
message(STATUS "Critical Cycles OSL: ${CYCLES_OSL_LIBRARY}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "=====================================")