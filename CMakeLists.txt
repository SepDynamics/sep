cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(SEPEngine LANGUAGES CXX)
add_executable(sep_engine src/main.cpp)

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

# Internal libraries built as part of this project. Having them in a
# separate variable lets us easily link the same set of libraries in
# tests and executables.
set(SEP_INTERNAL_LIBS
    sep_api sep_memory sep_quantum sep_core sep_audio sep_blender sep_compat
)

# Find and add all static Cycles component libraries to our link list
find_library(CYCLES_SESSION_LIBRARY NAMES cycles_session libcycles_session.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_SCENE_LIBRARY NAMES cycles_scene libcycles_scene.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_GRAPH_LIBRARY NAMES cycles_graph libcycles_graph.a PATHS ${CMAKE_SOURCE_DIR}/lib) # The critical fix for ccl::Node
find_library(CYCLES_KERNEL_LIBRARY NAMES cycles_kernel libcycles_kernel.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_DEVICE_LIBRARY NAMES cycles_device libcycles_device.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_UTIL_LIBRARY NAMES cycles_util libcycles_util.a PATHS ${CMAKE_SOURCE_DIR}/lib)
find_library(CYCLES_BVH_LIBRARY NAMES cycles_bvh libcycles_bvh.a PATHS ${CMAKE_SOURCE_DIR}/lib)


# Find Cycles OSL library with fallback
find_library(CYCLES_OSL_LIBRARY NAMES cycles_osl libcycles_osl.a PATHS ${CMAKE_SOURCE_DIR}/lib /usr/lib /usr/lib64 /usr/local/lib)
if(CYCLES_OSL_LIBRARY)
  message(STATUS "Found Cycles OSL library: ${CYCLES_OSL_LIBRARY}")
else()
  message(WARNING "Cycles OSL library not found. Setting empty placeholder.")
  set(CYCLES_OSL_LIBRARY "")
endif()

# Ensure it's added exactly once
if(CYCLES_OSL_LIBRARY)
    list(APPEND SEP_LINK_LIBS ${CYCLES_OSL_LIBRARY})
else()
    message(WARNING "Cycles OSL library not found; OSL features will be disabled.")
endif()


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

# Consolidate internal libraries and all dependencies for reuse across targets
set(SEP_INTERNAL_LIBS
    sep_api sep_memory sep_quantum sep_core sep_audio sep_blender sep_compat
)
set(SEP_ENGINE_LIBS
    ${SEP_INTERNAL_LIBS}
    ${SEP_LINK_LIBS}
    ${CURL_LIBRARIES} ${HIREDIS_LIBRARIES} ${CUDA_LIBRARIES}
    Threads::Threads fmt::fmt spdlog::spdlog
    -ldl -lm
)

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



target_include_directories(sep_engine
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/glm
        ${CMAKE_SOURCE_DIR}/cycles/src
)
if(HIREDIS_INCLUDE_DIR)
    target_include_directories(sep_engine PRIVATE ${HIREDIS_INCLUDE_DIR})
endif()

# Find OSL libraries directly (similar to tests approach)
find_library(OSL_COMP_LIBRARY NAMES oslcomp)
find_library(OSL_EXEC_LIBRARY NAMES oslexec)
find_library(OSL_QUERY_LIBRARY NAMES oslquery)

message(STATUS "Found OSL Comp: ${OSL_COMP_LIBRARY}")
message(STATUS "Found OSL Exec: ${OSL_EXEC_LIBRARY}")
message(STATUS "Found OSL Query: ${OSL_QUERY_LIBRARY}")

# Create a consolidated OSL_LIBRARIES variable that matches what the test file uses
set(OSL_LIBRARIES "")
if(OSL_COMP_LIBRARY)
  list(APPEND OSL_LIBRARIES ${OSL_COMP_LIBRARY})
endif()
if(OSL_EXEC_LIBRARY)
  list(APPEND OSL_LIBRARIES ${OSL_EXEC_LIBRARY})
endif()
if(OSL_QUERY_LIBRARY)
  list(APPEND OSL_LIBRARIES ${OSL_QUERY_LIBRARY})
endif()

if(OSL_LIBRARIES)
  message(STATUS "Final OSL libraries to link: ${OSL_LIBRARIES}")
else()
  message(WARNING "OSL components not found. Cycles OSL features will be disabled.")
endif()

# Also make sure OSL include directories are known
find_path(OSL_INCLUDE_DIR NAMES OSL/oslcomp.h HINTS /usr/include /usr/local/include /opt/local/include)
if(OSL_INCLUDE_DIR)
  target_include_directories(sep_engine PUBLIC ${OSL_INCLUDE_DIR})
  message(STATUS "OSL include directory set to: ${OSL_INCLUDE_DIR}")
else()
  message(WARNING "OSL include directory not found. Compilation issues may occur.")
endif()

# NEW: Manually ensure Cycles Graph library is found, as it contains ccl::Node symbols.
# This explicitly addresses the `ccl::Node::Node` and `ccl::NodeType` undefined references.
find_library(CYCLES_GRAPH_LIBRARY NAMES cycles_graph libcycles_graph.a PATHS
  ${CMAKE_SOURCE_DIR}/lib
  ${CYCLES_ROOT}/lib
  ${BLENDER_LIB_DIR}
  /usr/lib/cycles
  /usr/local/lib/cycles
)
if(CYCLES_GRAPH_LIBRARY)
  message(STATUS "Found Cycles Graph library: ${CYCLES_GRAPH_LIBRARY}")
else()
  message(WARNING "Cycles Graph library not found. This may cause linking issues.")
  set(CYCLES_GRAPH_LIBRARY "")
endif()

# NEW: Collect all essential Cycles libraries into a single list for linking.
# This variable will be used for both sep_engine and cycles_test to ensure consistency.
set(CYCLES_ESSENTIAL_LIBS "")
if(CYCLES_KERNEL_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_KERNEL_LIBRARY})
endif()
if(CYCLES_DEVICE_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_DEVICE_LIBRARY})
endif()
if(CYCLES_UTIL_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_UTIL_LIBRARY})
endif()
if(CYCLES_SCENE_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_SCENE_LIBRARY})
endif()
if(CYCLES_SESSION_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_SESSION_LIBRARY})
endif()
if(CYCLES_BVH_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_BVH_LIBRARY})
endif()
if(CYCLES_GRAPH_LIBRARY)
    list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_GRAPH_LIBRARY})
endif()
# Add the general CYCLES_LIBRARIES which might contain other components like Cycles core
# if find_package(CYCLES) manages to find a comprehensive set.
list(APPEND CYCLES_ESSENTIAL_LIBS ${CYCLES_LIBRARIES})

# OpenSubdiv libraries require an older TBB version. Mirror the setup used by the
# Cycles test target so the engine links against the correct combination.
find_library(OPENSUBDIV_CPU_LIBRARY NAMES
  osdCPU
  libosdCPU.so
  libosdCPU.so.3.6.0
  PATHS
  ${CMAKE_SOURCE_DIR}/lib
  /usr/lib64
  /usr/lib
  /usr/local/lib
)

find_library(OPENSUBDIV_GPU_LIBRARY NAMES
  osdGPU
  libosdGPU.so
  libosdGPU.so.3.6.0
  PATHS
  ${CMAKE_SOURCE_DIR}/lib
  /usr/lib64
  /usr/lib
  /usr/local/lib
)

find_library(TBB_OPENSUBDIV_LIBRARY NAMES
  tbb.so.2
  libtbb.so.2
  PATHS
  /usr/lib64
  /usr/lib
  /usr/local/lib
)

message(STATUS "Found TBB for OpenSubdiv: ${TBB_OPENSUBDIV_LIBRARY}")

find_library(TBB_LIBRARY NAMES tbb)

# Find OpenPGL library and its dependencies
find_library(OpenPGL_LIBRARY NAMES openpgl libopenpgl.so)
if(OpenPGL_LIBRARY)
  message(STATUS "Found OpenPGL library: ${OpenPGL_LIBRARY}")
else()
  message(WARNING "OpenPGL library not found, using default path /usr/lib64/libopenpgl.so")
  set(OpenPGL_LIBRARY "/usr/lib64/libopenpgl.so")
endif()

# PipeWire
find_library(PIPEWIRE_LIBRARY NAMES pipewire-0.3 libpipewire-0.3.so)
if(PIPEWIRE_LIBRARY)
  message(STATUS "Found PipeWire: ${PIPEWIRE_LIBRARY}")
else()
  message(WARNING "PipeWire library not found, defaulting to /usr/lib64/libpipewire-0.3.so")
  set(PIPEWIRE_LIBRARY "/usr/lib64/libpipewire-0.3.so")
endif()

# OpenSubdiv CPU
find_library(OPENSUBDIV_CPU_LIBRARY NAMES osdCPU libosdCPU.so)
if(OPENSUBDIV_CPU_LIBRARY)
  message(STATUS "Found OpenSubdiv CPU: ${OPENSUBDIV_CPU_LIBRARY}")
else()
  message(WARNING "OpenSubdiv CPU not found, defaulting to /usr/lib64/libosdCPU.so")
  set(OPENSUBDIV_CPU_LIBRARY "/usr/lib64/libosdCPU.so")
endif()

# OpenSubdiv GPU
find_library(OPENSUBDIV_GPU_LIBRARY NAMES osdGPU libosdGPU.so)
if(OPENSUBDIV_GPU_LIBRARY)
  message(STATUS "Found OpenSubdiv GPU: ${OPENSUBDIV_GPU_LIBRARY}")
else()
  message(WARNING "OpenSubdiv GPU not found, defaulting to /usr/lib64/libosdGPU.so")
  set(OPENSUBDIV_GPU_LIBRARY "/usr/lib64/libosdGPU.so")
endif()

# TBB
find_library(TBB_LIBRARY NAMES tbb libtbb.so)
if(TBB_LIBRARY)
  message(STATUS "Found TBB: ${TBB_LIBRARY}")
else()
  message(WARNING "TBB library not found, defaulting to /usr/lib64/libtbb.so")
  set(TBB_LIBRARY "/usr/lib64/libtbb.so")
endif()


# Find OpenVDB library and its dependencies
find_library(OPENVDB_LIBRARY NAMES openvdb libopenvdb.so.11.0 libopenvdb.so)
if(OPENVDB_LIBRARY)
  message(STATUS "Found OpenVDB library: ${OPENVDB_LIBRARY}")
else()
  message(WARNING "OpenVDB library not found, using default path /usr/lib64/libopenvdb.so.11.0")
  set(OPENVDB_LIBRARY "/usr/lib64/libopenvdb.so.11.0")
endif()

# Find OpenVDB dependencies (Boost, Blosc)
find_library(BLOSC_LIBRARY NAMES blosc)
find_library(BOOST_THREAD_LIBRARY NAMES boost_thread)
find_library(BOOST_SYSTEM_LIBRARY NAMES boost_system)
find_library(BOOST_IOSTREAMS_LIBRARY NAMES boost_iostreams)
find_library(BOOST_REGEX_LIBRARY NAMES boost_regex)
find_library(BOOST_FILESYSTEM_LIBRARY NAMES boost_filesystem)

# Set OpenVDB dependencies - IMPORTANT: Collect and link directly after OpenVDB
set(OPENVDB_DEPS "")
if(BLOSC_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BLOSC_LIBRARY})
endif()
if(BOOST_THREAD_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_THREAD_LIBRARY})
endif()
if(BOOST_SYSTEM_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_SYSTEM_LIBRARY})
endif()
if(BOOST_IOSTREAMS_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_IOSTREAMS_LIBRARY})
endif()
if(BOOST_REGEX_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_REGEX_LIBRARY})
endif()
if(BOOST_FILESYSTEM_LIBRARY)
  list(APPEND OPENVDB_DEPS ${BOOST_FILESYSTEM_LIBRARY})
endif()

# Find additional libraries needed by the main executable
find_library(OPENIMAGEIO_LIBRARY NAMES OpenImageIO)
find_library(OPENIMAGEIO_UTIL_LIBRARY NAMES OpenImageIO_Util)
find_library(ALEMBIC_LIBRARY NAMES Alembic) # This is usually a shared library
find_library(ZSTD_LIBRARY NAMES zstd)
find_library(OPENCOLORIO_LIBRARY NAMES OpenColorIO)
find_library(IMATH_LIBRARY NAMES Imath)
find_library(OPENEXR_LIBRARY NAMES OpenEXR)
find_library(EMBREE_LIBRARY NAMES embree4) # Version-specific, check your installation

# Print status of found libraries
message(STATUS "Found OpenImageIO: ${OPENIMAGEIO_LIBRARY}")
message(STATUS "Found OpenImageIO_Util: ${OPENIMAGEIO_UTIL_LIBRARY}")
message(STATUS "Found Alembic: ${ALEMBIC_LIBRARY}")
message(STATUS "Found ZSTD: ${ZSTD_LIBRARY}")
message(STATUS "Found OpenColorIO: ${OPENCOLORIO_LIBRARY}")
message(STATUS "Found Imath: ${IMATH_LIBRARY}")
message(STATUS "Found OpenEXR: ${OPENEXR_LIBRARY}")
message(STATUS "Found Embree: ${EMBREE_LIBRARY}")

# Check for Alembic (required by libcycles_scene.a)
if(NOT ALEMBIC_LIBRARY)
  # Explicit fallback for Alembic
  find_library(ALEMBIC_LIBRARY NAMES Alembic libAlembic.so.1.8 libAlembic.so.1 libAlembic.so)
  if(ALEMBIC_LIBRARY)
    message(STATUS "Found Alembic library: ${ALEMBIC_LIBRARY}")
  else()
    message(WARNING "Alembic library not found, using default path /usr/lib64/libAlembic.so.1.8")
    set(ALEMBIC_LIBRARY "/usr/lib64/libAlembic.so.1.8")
  endif()
endif()

# Add GFlags library (required by libcycles_util.a)
find_library(GFLAGS_LIBRARIES NAMES gflags libgflags.so.2.2 libgflags.so)
if(GFLAGS_LIBRARIES)
  message(STATUS "Found GFlags library: ${GFLAGS_LIBRARIES}")
else()
  message(WARNING "GFlags library not found, using default path /usr/lib64/libgflags.so.2.2")
  set(GFLAGS_LIBRARIES "/usr/lib64/libgflags.so.2.2")
endif()

# Alembic shared library and its dependencies
set(ALEMBIC_DEPS "")
if(BOOST_FILESYSTEM_LIBRARY)
  list(APPEND ALEMBIC_DEPS ${BOOST_FILESYSTEM_LIBRARY})
endif()
if(IMATH_LIBRARY)
  list(APPEND ALEMBIC_DEPS ${IMATH_LIBRARY})
endif()
if(OPENEXR_LIBRARY) # OpenEXR is a common dependency for Alembic
  list(APPEND ALEMBIC_DEPS ${OPENEXR_LIBRARY})
endif()
if(ZLIB_LIBRARY) # ZLib also often needed
  list(APPEND ALEMBIC_DEPS ${ZLIB_LIBRARY})
endif()

target_link_libraries(sep_engine PRIVATE
    sep_api sep_memory sep_quantum sep_core sep_audio sep_blender sep_compat

    # MUST BE THIS EXACT ORDER
    ${CYCLES_KERNEL_LIBRARY}   # libcycles_kernel.a
    ${CYCLES_DEVICE_LIBRARY}   # libcycles_device.a
    ${CYCLES_SESSION_LIBRARY}  # libcycles_session.a
    ${CYCLES_SCENE_LIBRARY}    # libcycles_scene.a
    ${CYCLES_GRAPH_LIBRARY}    # libcycles_graph.a
    ${CYCLES_BVH_LIBRARY}      # libcycles_bvh.a
    ${CYCLES_INTEGRATOR_LIBRARY} # <- CRITICAL! Verify this library.
    ${CYCLES_OSL_LIBRARY}      # libcycles_osl.a (after integrator/kernel)

    # OSL runtime libraries after Cycles OSL static lib
    ${OSL_LIBRARIES}

    # Blender and remaining deps after cycles
    ${BF_BLENKERNEL_LIBRARY}
    ${BF_BLENLIB_LIBRARY}
    ${BF_DRAW_LIBRARY}
    ${BF_GPU_LIBRARY}
    ${BF_GUARDEDALLOC_LIBRARY}
    ${BF_INTERN_SKY_LIBRARY}
    ${BF_INTERN_OPENSUBDIV_LIBRARY}

    # OpenSubdiv (legacy TBB first)
    ${OPENSUBDIV_CPU_LIBRARY}
    ${OPENSUBDIV_GPU_LIBRARY}
    ${TBB_OPENSUBDIV_LIBRARY}   # libtbb.so.2

    # OpenPGL (path-guiding)
    ${OpenPGL_LIBRARY}

    # OpenVDB and Boost dependencies
    ${OPENVDB_LIBRARY}
    ${OPENVDB_DEPS}

    # TBB current version (after legacy)
    ${TBB_LIBRARY}              # libtbb.so.12

    # Remaining dependencies
    ${OPENIMAGEIO_LIBRARY}
    ${OPENIMAGEIO_UTIL_LIBRARY}
    ${EMBREE_LIBRARY}
    ${ALEMBIC_LIBRARY}
    ${ALEMBIC_DEPS}
    ${GLOG_LIBRARY}
    ${GFLAGS_LIBRARIES}
    ${ZSTD_LIBRARY}
    ${OPENCOLORIO_LIBRARY}
    ${OPENEXR_LIBRARY}

    # Core system and utility deps
    ${CURL_LIBRARIES}
    ${CUDA_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    fmt::fmt
    spdlog::spdlog
    Threads::Threads
    -lpthread -ldl -lm
)

