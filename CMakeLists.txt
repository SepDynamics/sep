cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(SEPEngine LANGUAGES CXX)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_definitions(CROW_DISABLE_RTTI=1)

# --- Core Dependencies (CURL, Redis, Logging, etc.) ---
find_package(CUDAToolkit REQUIRED)
find_package(CURL REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

find_library(HIREDIS_LIBRARIES NAMES hiredis)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
if(HIREDIS_LIBRARIES AND HIREDIS_INCLUDE_DIR)
    set(SEP_HAS_HIREDIS 1)
else()
    set(SEP_HAS_HIREDIS 0)
endif()
add_compile_definitions(SEP_HAS_HIREDIS=${SEP_HAS_HIREDIS})

# --- Centralized Dependency Finding for Blender/Cycles ---
set(SEP_HAS_BLENDER 1)
add_compile_definitions(SEP_HAS_BLENDER=${SEP_HAS_BLENDER})
set(SEP_HAS_CYCLES 1)
add_compile_definitions(SEP_HAS_CYCLES=${SEP_HAS_CYCLES})

# Internal libraries used by all executables
set(SEP_INTERNAL_LIBS
    sep_api
    sep_memory
    sep_quantum
    sep_core
    sep_audio
    sep_blender
    sep_compat
)

# This list will hold all external libraries to be linked by the final targets.
set(SEP_LINK_LIBS "")

# Find and add all static Cycles component libraries to our link list
find_library(CYCLES_SESSION_LIBRARY NAMES cycles_session libcycles_session.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_SCENE_LIBRARY NAMES cycles_scene libcycles_scene.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_GRAPH_LIBRARY NAMES cycles_graph libcycles_graph.a PATHS ${CMAKE_SOURCE_DIR}/lib) # The critical fix for ccl::Node
find_library(CYCLES_KERNEL_LIBRARY NAMES cycles_kernel libcycles_kernel.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_DEVICE_LIBRARY NAMES cycles_device libcycles_device.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_UTIL_LIBRARY NAMES cycles_util libcycles_util.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_BVH_LIBRARY NAMES cycles_bvh libcycles_bvh.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_OSL_LIBRARY NAMES cycles_osl libcycles_osl.a PATHS ${CMAKE_SOURCE_DIR}/lib)
list(APPEND SEP_LINK_LIBS
    ${CYCLES_SESSION_LIBRARY}
    ${CYCLES_SCENE_LIBRARY}
    ${CYCLES_GRAPH_LIBRARY}
    ${CYCLES_KERNEL_LIBRARY}
    ${CYCLES_DEVICE_LIBRARY}
    ${CYCLES_UTIL_LIBRARY}
    ${CYCLES_OSL_LIBRARY}
    ${CYCLES_BVH_LIBRARY}
)

# Find and add Blender internal libraries
find_library(BF_BLENKERNEL_LIBRARY NAMES bf_blenkernel PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_BLENLIB_LIBRARY NAMES bf_blenlib PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_DRAW_LIBRARY NAMES bf_draw PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_GPU_LIBRARY NAMES bf_gpu PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_GUARDEDALLOC_LIBRARY NAMES bf_intern_guardedalloc PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_INTERN_SKY_LIBRARY NAMES bf_intern_sky PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(BF_INTERN_OPENSUBDIV_LIBRARY NAMES bf_intern_opensubdiv PATHS ${CMAKE_SOURCE_DIR}/lib)
list(APPEND SEP_LINK_LIBS
    ${BF_BLENKERNEL_LIBRARY} ${BF_BLENLIB_LIBRARY} ${BF_DRAW_LIBRARY} ${BF_GPU_LIBRARY}
    ${BF_GUARDEDALLOC_LIBRARY} ${BF_INTERN_SKY_LIBRARY} ${BF_INTERN_OPENSUBDIV_LIBRARY}
)

# Find and add external wrappers
find_library(EXTERN_CUEW_LIBRARY NAMES extern_cuew PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(EXTERN_HIPEW_LIBRARY NAMES extern_hipew PATHS ${CMAKE_SOURCE_DIR}/lib)
list(APPEND SEP_LINK_LIBS ${EXTERN_CUEW_LIBRARY} ${EXTERN_HIPEW_LIBRARY})

# Find and add all other external dependencies conditionally
macro(find_and_add_library var_name names)
    find_library(${var_name} NAMES ${names})
    if(${var_name})
        message(STATUS "Found ${var_name}: ${${var_name}}")
        list(APPEND SEP_LINK_LIBS ${${var_name}})
    else()
        message(WARNING "${var_name} (${names}) not found. Linking may fail if required.")
    endif()
endmacro()

find_and_add_library(OSL_COMP_LIBRARY "oslcomp")
find_and_add_library(OSL_EXEC_LIBRARY "oslexec")
find_and_add_library(OSL_QUERY_LIBRARY "oslquery")
find_and_add_library(OPENSUBDIV_CPU_LIBRARY "osdCPU libosdCPU.so.3.6.0")
find_and_add_library(OPENSUBDIV_GPU_LIBRARY "osdGPU libosdGPU.so.3.6.0")
find_and_add_library(TBB_OPENSUBDIV_LIBRARY "tbb.so.2 libtbb.so.2")
find_and_add_library(OpenPGL_LIBRARY "OpenPGL pgl")
find_and_add_library(OPENVDB_LIBRARY "openvdb")
find_and_add_library(BLOSC_LIBRARY "blosc")
find_and_add_library(TBB_LIBRARY "tbb")
find_and_add_library(ALEMBIC_LIBRARY "Alembic")
find_and_add_library(IMATH_LIBRARY "Imath")
find_and_add_library(GFLAGS_LIBRARY "gflags")
find_and_add_library(GLOG_LIBRARY "glog")
find_and_add_library(EMBREE_LIBRARY "embree4")
find_and_add_library(ZSTD_LIBRARY "zstd")
find_and_add_library(OPENCOLORIO_LIBRARY "OpenColorIO")
find_and_add_library(OPENEXR_LIBRARY "OpenEXR")
find_and_add_library(OPENIMAGEIO_LIBRARY "OpenImageIO")
find_and_add_library(OPENIMAGEIO_UTIL_LIBRARY "OpenImageIO_Util")
find_and_add_library(ZLIB_LIBRARY "z")

# --- Project Structure and Includes ---
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/glm
    ${HIREDIS_INCLUDE_DIR}
)
add_subdirectory(src/api)
add_subdirectory(src/audio)
add_subdirectory(src/blender)
add_subdirectory(src/compat)
add_subdirectory(src/core)
add_subdirectory(src/memory)
add_subdirectory(src/quantum)
add_subdirectory(src/tests)

# --- Main Executable Target ---
add_executable(sep_engine src/main.cpp)

target_link_libraries(sep_engine
    PRIVATE
    # Internal libraries first
    ${SEP_INTERNAL_LIBS}
    # Then all found external dependencies
    ${SEP_LINK_LIBS}
    # Then required package libraries
    ${CURL_LIBRARIES} ${HIREDIS_LIBRARIES} ${CUDA_LIBRARIES} Threads::Threads fmt::fmt spdlog::spdlog
    # Finally, standard system libs
    -ldl -lm
)
