cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
project(SEPEngine LANGUAGES CXX)

# Ensure RTTI is disabled consistently across all targets when building with
# the Crow isolation headers.
add_compile_definitions(CROW_DISABLE_RTTI=1)

# Include CUDA configuration for library paths
include(${CMAKE_SOURCE_DIR}/cmake/CustomCUDA.cmake)

# Locate external dependencies required by the engine
find_package(CUDAToolkit QUIET)
find_package(CURL QUIET)
find_package(CROW QUIET)
find_package(TBB QUIET)
find_package(glm QUIET)
find_package(fmt QUIET)
find_package(spdlog QUIET)

# NEW: Find OpenPGL (Open Path Guiding Library)
find_package(OpenPGL QUIET)
if(NOT OpenPGL_FOUND)
    # Manual search for OpenPGL if find_package fails
    find_library(OpenPGL_LIBRARY NAMES OpenPGL pgl)
    if(OpenPGL_LIBRARY)
        message(STATUS "Manually found OpenPGL library: ${OpenPGL_LIBRARY}")
    else()
        message(WARNING "OpenPGL library not found. Path guiding features will be disabled.")
        set(OpenPGL_LIBRARY "") # Ensure it's empty if not found
    endif()
endif()

include(FetchContent)

# Use C++17 across all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define Crow compile-time configuration globally to avoid
# redundant macro definitions in individual compilation units
add_compile_definitions(CROW_DISABLE_RTTI=1)

# Locate external dependencies required for the final executable
find_package(CURL REQUIRED)
# Hiredis doesn't ship a CMake package file in many distributions,
# so fall back to a simple find_library/find_path approach.
find_library(HIREDIS_LIBRARIES NAMES hiredis)
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
if(HIREDIS_LIBRARIES AND HIREDIS_INCLUDE_DIR)
    set(SEP_HAS_HIREDIS 1)
else()
    set(SEP_HAS_HIREDIS 0)
    set(HIREDIS_LIBRARIES "")
    set(HIREDIS_INCLUDE_DIR "")
endif()
add_compile_definitions(SEP_HAS_HIREDIS=${SEP_HAS_HIREDIS})

# Enable Blender support
set(SEP_HAS_BLENDER 1)
add_compile_definitions(SEP_HAS_BLENDER=${SEP_HAS_BLENDER})


# This ensures our fixed headers are used instead of the problematic ones
include_directories(
  BEFORE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/blender
    ${CMAKE_SOURCE_DIR}/include/compat
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/crow
    ${CMAKE_SOURCE_DIR}/include/memory
    ${CMAKE_SOURCE_DIR}/include/quantum
)

# Add our fixed Crow headers subdirectory
add_subdirectory(include/crow)

include_directories(
  ${CMAKE_SOURCE_DIR}/extern
  ${CMAKE_SOURCE_DIR}/extern/glm
  ${CMAKE_SOURCE_DIR}/extern/tbb
)

# Optional Cycles renderer
if(EXISTS "${CMAKE_SOURCE_DIR}/extern/cycles/CMakeLists.txt")
    set(SEP_HAS_CYCLES 1)
    # Use our custom FindCycles module
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    include(FindCycles) # This module is expected to set CYCLES_LIBRARIES, CYCLES_UTIL_LIBRARY, etc.
else()
    set(SEP_HAS_CYCLES 0)
endif()
add_compile_definitions(SEP_HAS_CYCLES=${SEP_HAS_CYCLES})

# Fetch the Crow web framework if it is not present
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/extern/crow/include")
    message(STATUS "Fetching Crow framework...")
    FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG v1.2.1.2
    )
    FetchContent_MakeAvailable(crow)
    include_directories(${crow_SOURCE_DIR}/include)
else()
    include_directories(${CMAKE_SOURCE_DIR}/extern/crow/include)
endif()

set(CUDA_SOURCES
    compat/core.cu
    compat/cuda_api.cu
    compat/event.cu
    compat/pattern_kernels.cu
    compat/quantum_kernels.cu
    compat/utils.cu
)
add_executable(sep_engine src/main.cpp)

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

target_link_libraries(sep_engine
    sep_api
    sep_memory
    sep_quantum
    sep_core
    sep_audio
    sep_blender
    sep_compat
    
    # USD_LIBRARIES should be found by find_package(USD) if used, but it's not explicitly in this snippet.
    # Assuming it's handled by a custom module or provided externally.
    ${USD_LIBRARIES}
    # Link all general Boost libraries found by find_package(Boost)
    # The `find_library` calls above are specific to OpenVDB's dependencies.
    # You might have a `find_package(Boost COMPONENTS ...)` that sets `Boost_LIBRARIES`.
    ${BOOST_LIBRARIES} # Ensure this is linked if Boost is used generally.
    ${CYCLES_ESSENTIAL_LIBS} # Consolidated list of Cycles libraries
    ${OSL_LIBRARIES} # OSL libraries (now explicitly listing known good components)
    ${OpenPGL_LIBRARY} # OpenPGL library
    # Link OpenVDB and its direct dependencies in order
    ${OPENVDB_LIBRARY}
    ${OPENVDB_DEPS}
    # OpenImageIO often needs to be linked after OpenVDB
    ${OPENIMAGEIO_LIBRARY}
    ${OPENIMAGEIO_UTIL_LIBRARY}
    # Embree for ray tracing
    ${EMBREE_LIBRARY}
    # Alembic with its dependencies
    ${ALEMBIC_LIBRARY}
    ${ALEMBIC_DEPS}
    # Additional libraries
    ${ZSTD_LIBRARY}
    ${OPENCOLORIO_LIBRARY}
    ${OPENEXR_LIBRARY}
    # GFLAGS_LIBRARIES is the standard variable set by find_package(GFlags)
    # Added GFLAGS if it was found
    ${GFLAGS_LIBRARIES}
    ${CURL_LIBRARIES}
    ${CUDA_LIBRARIES} # General CUDA libraries (cudart, cudadevrt etc.)
    ${HIREDIS_LIBRARIES}
    ${OPENSUBDIV_CPU_LIBRARY}
    ${OPENSUBDIV_GPU_LIBRARY}
    ${TBB_OPENSUBDIV_LIBRARY}
    ${TBB_LIBRARY}
    fmt::fmt
    spdlog::spdlog
    # Standard system libraries - usually last
    -lpthread
    -ldl
    -lm
)